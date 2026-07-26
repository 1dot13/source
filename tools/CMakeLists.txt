# Developer tools. Console programs, no game code and no Application preprocessor
# definitions, so each is built once no matter which applications are configured.

# Reads a crash report written by sgp::writeExceptionBacktrace and resolves its
# addresses against a build's PDB. Wants C++23 (std::print, std::format); the game
# itself is C++17, hence the per-target standard.
add_executable(symbolize_crash symbolize_crash.cpp)
target_link_libraries(symbolize_crash PRIVATE "dbghelp.lib")
set_target_properties(symbolize_crash PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
)
