#ifndef WINE_H 
#define WINE_H 

#define IS_WINE (GetProcAddress(GetModuleHandleA("ntdll.dll"), "wine_get_version") != 0)

// ### types ###

// ### Variables ###

// ### Functions ###

BOOL wine_add_dll_overrides();
BOOL wine_add_dll_override(const WCHAR* dll_name);

#endif
