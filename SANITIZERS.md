# AddressSanitizer

AddressSanitizer (asan) finds memory errors at runtime: out-of-bounds accesses,
use-after-free, and similar. It reports the exact faulting access.

## Requirements

asan requires `C++ Clang tools for Windows` (MSVC's AddressSanitizer does not
support -fsanitize-ignorelist).

It is NOT installed by default . You can install it via the Visual Studio
Installer.

## Build and run

Either use the included `clang-cl-asan` preset, or inherit from it in your own
`CMakeUserPresets.json`

## Debug from Visual Studio

asan needs `clang_rt.asan_dynamic-i386.dll` at run time. MSVC ships this DLL
with the x86 tools, but Visual Studio does not add that folder to the debugger
PATH. To start the exe with F5, add the tools folder to the
debugger PATH:

1. Access the CMake Targets View in Solution Explorer
2. Right-click the exe target (for example `JA2.exe`) and select `Add Debug Configuration`. 
   Visual Studio creates `launch.vs.json` in the `.vs` folder and adds a configuration with
   the correct `projectTarget`.
3. Add an `env` block to that configuration:

```json
      "env": {
        "PATH": "C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Tools\\MSVC\\14.51.36231\\bin\\Hostx64\\x86;${env.PATH}"
      }
```

Set the first path to the `bin\Hostx64\x86` folder of your MSVC tools. To find
it, search your Visual Studio installation for `clang_rt.asan_dynamic-i386.dll`.
Your edition (`Community`), MSVC version (`14.51.36231`), and host (`Hostx64` or
`Hostx86`) can differ.

## Reading the report

Every app is a GUI app with no console, so asan writes the report to a file by
default:

```
gamedir/asan.report.<pid>
```

When asan finds an error, the run stops and the report names the faulting access.
Set the `ASAN_OPTIONS` environment variable to change asan behaviour.
