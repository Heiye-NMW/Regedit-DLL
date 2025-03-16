#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport) __stdcall
#else
#define DLLIMPORT __declspec(dllimport)
#endif

DLLIMPORT int SetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPCSTR valueData);
DLLIMPORT int GetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPSTR buffer, LPDWORD bufferSize);
DLLIMPORT int DeleteRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName);
DLLIMPORT int DeleteRegKey(HKEY hKey, LPCSTR subKey);

#endif
