// Symbolize a JA2 1.13 crash report: turn the runtime addresses the crash handler
// dumped into function names and source lines, against the matching build.
//
//   symbolize_crash <report.txt> [path\to\JA2.exe]
//
// The exe and the PDB beside it must be the exact build the report's `build <sha>`
// line names, or the addresses resolve to the wrong lines, silently. The default
// object is JA2.exe in the current directory.
//
// Frames print in call order: outermost caller first, descending to the fault.
//
// Addresses in a report are runtime VAs, and our executables are /DYNAMICBASE: the
// loader may put the image anywhere -- Wine keeps it at the preferred base,
// Windows ASLR does not. The report's module table records where it actually
// landed, and that is the base we hand DbgHelp, so the relocation arithmetic is
// its problem rather than ours. Reports predating the module table are assumed
// unrelocated, which is what they were.

#include <windows.h>
#include <dbghelp.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iterator>
#include <optional>
#include <print>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

namespace {

template <typename... Args>
[[noreturn]] void fatal(std::format_string<Args...> format, Args&&... args) {
	std::print(stderr, "symbolize_crash: ");
	std::println(stderr, format, std::forward<Args>(args)...);
	std::exit(1);
}

// --- colour ----------------------------------------------------------------

// Every field is empty when colour is off, so the format strings below never have
// to know which mode they are in.
struct Ink {
	std::string_view heading, label, code, function, location, address, fault,
		inlined, faded, off;
};

constexpr Ink kPlain{};
constexpr Ink kAnsi{
	.heading  = "\x1b[1;91m", // bright red
	.label    = "\x1b[90m",   // grey
	.code     = "\x1b[1;93m", // bright yellow
	.function = "\x1b[1;96m", // bright cyan
	.location = "\x1b[32m",   // green
	.address  = "\x1b[90m",
	.fault    = "\x1b[1;91m",
	.inlined  = "\x1b[35m",   // magenta
	.faded    = "\x1b[90m",
	.off      = "\x1b[0m",
};

Ink ink = kPlain;

// Colour only when standard output is a console we can put in ANSI mode: a report
// piped to a file or a pager should not collect escape sequences.
void enableColour() {
	if (std::getenv("NO_COLOR"))
		return;
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	if (!GetConsoleMode(out, &mode))
		return;
	if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING ||
		SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
		ink = kAnsi;
}

// --- the report ------------------------------------------------------------

struct Module {
	DWORD64 base = 0;
	DWORD64 size = 0;
	std::string path;
};

struct Frame {
	int index = 0;
	DWORD64 address = 0;
};

struct Report {
	std::string code = "????????";
	std::string accessViolation, time, build, handle;
	std::vector<Module> modules; // the loader lists the main image first
	std::vector<Frame> frames;
};

std::string slurp(const char* path) {
	std::ifstream file(path, std::ios::binary);
	if (!file)
		fatal("cannot read {}", path);
	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

// Reports are CRLF and ECMAScript treats CR as a line terminator, so neither "."
// nor "$" ever crosses one -- captures come back without the carriage return.
std::string capture(const std::string& text, const char* pattern) {
	std::smatch match;
	std::regex expression(pattern, std::regex::ECMAScript | std::regex::multiline);
	return std::regex_search(text, match, expression) ? match[1].str() : std::string();
}

std::vector<std::smatch> captureAll(const std::string& text, const char* pattern) {
	std::regex expression(pattern, std::regex::ECMAScript | std::regex::multiline);
	return { std::sregex_iterator(text.begin(), text.end(), expression),
	         std::sregex_iterator() };
}

DWORD64 fromHex(const std::string& digits) {
	return std::strtoull(digits.c_str(), nullptr, 16);
}

Report parse(const std::string& text) {
	Report report;
	if (auto code = capture(text, R"(code=(\w+))"); !code.empty())
		report.code = code;
	report.accessViolation = capture(text, R"(access violation: (.+))");
	report.time            = capture(text, R"(^\s+time (.+))");
	report.build           = capture(text, R"(^\s+build (.+))");
	report.handle          = capture(text, R"(^\s+handle (.+))");

	for (const auto& match : captureAll(text, R"(^ {4}([0-9A-Fa-f]{8}) ([0-9A-Fa-f]{8}) (.+)$)"))
		report.modules.push_back({ fromHex(match[1]), fromHex(match[2]), match[3] });
	for (const auto& match : captureAll(text, R"(^\s+\[(\d+)\] ([0-9A-Fa-f]{8}))"))
		report.frames.push_back({ std::stoi(match[1]), fromHex(match[2]) });
	return report;
}

// --- symbols ---------------------------------------------------------------

struct Location {
	std::string function = "<no symbol>";
	std::string file;
	DWORD line = 0;
};

// DbgHelp writes the name into the tail of the structure, hence the raw buffer.
struct SymbolBuffer {
	SYMBOL_INFO* get() {
		auto* symbol = reinterpret_cast<SYMBOL_INFO*>(storage);
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;
		return symbol;
	}
	alignas(SYMBOL_INFO) char storage[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {};
};

// Outermost first: the function that owns the address, then everything the
// optimizer inlined into it, down to the code the address really belongs to.
std::vector<Location> resolve(HANDLE process, DWORD64 address) {
	std::vector<Location> locations;

	SymbolBuffer buffer;
	DWORD64 offset = 0;
	DWORD displacement = 0;
	IMAGEHLP_LINE64 line{ sizeof(IMAGEHLP_LINE64) };

	Location outer;
	if (SymFromAddr(process, address, &offset, buffer.get()))
		outer.function = buffer.get()->Name;
	if (SymGetLineFromAddr64(process, address, &displacement, &line)) {
		outer.file = line.FileName;
		outer.line = line.LineNumber;
	}
	locations.push_back(std::move(outer));

	// Inline contexts run innermost first, so walk them backwards to keep the
	// caller-to-callee order the rest of the listing uses.
	DWORD inlined = SymAddrIncludeInlineTrace(process, address);
	DWORD context = 0, frameIndex = 0;
	if (inlined == 0 || !SymQueryInlineTrace(process, address, 0, address, address,
			&context, &frameIndex))
		return locations;

	for (DWORD i = inlined; i-- > 0;) {
		Location location;
		if (SymFromInlineContext(process, address, context + i, &offset, buffer.get()))
			location.function = buffer.get()->Name;
		line = { sizeof(IMAGEHLP_LINE64) };
		if (SymGetLineFromInlineContext(process, address, context + i, 0,
				&displacement, &line)) {
			location.file = line.FileName;
			location.line = line.LineNumber;
		}
		locations.push_back(std::move(location));
	}
	return locations;
}

std::string_view baseName(std::string_view path) {
	size_t separator = path.find_last_of("\\/");
	return separator == std::string_view::npos ? path : path.substr(separator + 1);
}

} // namespace

int main(int argc, char** argv) {
	if (argc < 2) {
		std::println(stderr, "usage: {} <report.txt> [JA2.exe]", baseName(argv[0]));
		return 2;
	}
	const char* objectPath = argc > 2 ? argv[2] : "JA2.exe";
	enableColour();

	const Report report = parse(slurp(argv[1]));
	if (report.frames.empty())
		fatal("no [n] ADDR frames in {}", argv[1]);

	// Only the main image can be symbolized against this exe's PDB. Sibling
	// modules are named with an offset, and addresses inside no module at all are
	// stack debris the frame-pointer walk picked up, so they are dropped.
	const Module* image = report.modules.empty() ? nullptr : &report.modules.front();
	const auto owner = [&](DWORD64 address) -> const Module* {
		auto found = std::ranges::find_if(report.modules, [&](const Module& module) {
			return module.base <= address && address < module.base + module.size;
		});
		return found == report.modules.end() ? nullptr : &*found;
	};

	HANDLE process = GetCurrentProcess();
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_NO_PROMPTS);
	// The PDB sits next to the exe, which is not necessarily the current directory.
	const std::string_view object(objectPath);
	const std::string_view directory =
		object.substr(0, object.size() - baseName(object).size());
	const std::string searchPath(directory.empty() ? "." : directory);
	if (!SymInitialize(process, searchPath.c_str(), FALSE))
		fatal("SymInitialize failed ({})", GetLastError());

	// A base of zero tells DbgHelp to use the image's own preferred base, which is
	// exactly right for a report from before the module table existed.
	const DWORD64 loadedAt = SymLoadModuleEx(process, nullptr, objectPath, nullptr,
		image ? image->base : 0, image ? static_cast<DWORD>(image->size) : 0, nullptr, 0);
	if (loadedAt == 0)
		fatal("cannot load {} ({})", objectPath, GetLastError());

	IMAGEHLP_MODULE64 moduleInfo{ sizeof(IMAGEHLP_MODULE64) };
	if (SymGetModuleInfo64(process, loadedAt, &moduleInfo) && moduleInfo.SymType != SymPdb)
		std::println("  {}no PDB beside {} -- expect export names and no source lines{}",
			ink.fault, objectPath, ink.off);

	std::println("\n  {}CRASH  {}{}{}{}{}", ink.heading, ink.code, report.code, ink.off,
		report.accessViolation.empty() ? "" : "   ",
		report.accessViolation.empty() ? std::string()
			: std::format("{}({}){}", ink.faded, report.accessViolation, ink.off));
	for (auto [label, value] : { std::pair{ "time", &report.time },
			std::pair{ "build", &report.build }, std::pair{ "handle", &report.handle } })
		if (!value->empty())
			std::println("  {}{:<6}{} {}", ink.label, label, ink.off, *value);
	std::println("  {}{:<6}{} {:08X}", ink.label, "base", ink.off, loadedAt);
	std::println("");

	// Call order: the outermost caller ([n]) first, the fault ([0]) last.
	auto frames = report.frames;
	std::ranges::sort(frames, std::ranges::greater{}, &Frame::index);

	int depth = 0;
	for (const Frame& frame : frames) {
		const Module* module = owner(frame.address);
		if (!report.modules.empty() && module == nullptr)
			continue;
		const std::string indent(2 * depth++, ' ');
		const std::string tag = frame.index == 0
			? std::format("   {}<-- fault{}", ink.fault, ink.off) : "";
		const std::string address =
			std::format("{}[{:08X}]{}", ink.address, frame.address, ink.off);

		if (module != nullptr && module != image) {
			std::println("  {}{}{}+0x{:X}{}{}   {}", indent, ink.function,
				baseName(module->path), frame.address - module->base, ink.off, tag, address);
			continue;
		}

		bool first = true;
		for (const Location& location : resolve(process, frame.address)) {
			std::println("  {}{}{}{}{}{}", indent, first ? "" : "  ",
				first ? ink.function : ink.inlined,
				first ? location.function : std::format("(inlined) {}", location.function),
				ink.off, first ? tag : "");
			if (!location.file.empty())
				std::println("  {}{}    {}{}:{}{}{}", indent, first ? "" : "  ",
					ink.location, location.file, location.line, ink.off,
					first ? std::format("   {}", address) : "");
			first = false;
		}
	}
	std::println("");

	SymCleanup(process);
	return 0;
}
