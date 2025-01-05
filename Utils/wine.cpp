#include <windows.h>
#include <stdio.h>
#include "wine.h"


BOOL wine_add_dll_overrides()
{
    BOOL dd = wine_add_dll_override(L"ddraw");
    //BOOL ws = wine_add_dll_override(L"wsock32");
    
    return dd; // || ws;
}

BOOL wine_add_dll_override(const WCHAR* dll_name)
{
    if (!IS_WINE)
        return FALSE;

    WCHAR exe_path[MAX_PATH] = { 0 };
    if (!GetModuleFileNameW(NULL, exe_path, _countof(exe_path) - 1))
        return FALSE;

    WCHAR exe_file_name[MAX_PATH] = { 0 };
    WCHAR exe_file_ext[MAX_PATH] = { 0 };
    _wsplitpath(exe_path, NULL, NULL, exe_file_name, exe_file_ext);

    WCHAR reg_path[512] = { 0 };
    _snwprintf(
        reg_path, 
        _countof(reg_path) - 1,
        L"SOFTWARE\\Wine\\AppDefaults\\%s%s\\DllOverrides", 
        exe_file_name, 
        exe_file_ext);

    BOOL result = FALSE;
    HKEY hkey;
    LONG status =
        RegCreateKeyExW(
            HKEY_CURRENT_USER,
            reg_path,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_WRITE | KEY_READ,
            NULL,
            &hkey,
            NULL);

    if (status == ERROR_SUCCESS)
    {
        if (RegQueryValueExW(hkey, dll_name, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
        {
            WCHAR v[] = L"native, builtin";
            if (RegSetValueExW(hkey, dll_name, 0, REG_SZ, (LPCBYTE)v, sizeof(v)) == ERROR_SUCCESS && 
                RegQueryValueExW(hkey, dll_name, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
            {
                result = TRUE;
            }
        }

        RegCloseKey(hkey);
    }

    return result;
}
