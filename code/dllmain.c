#include <windows.h>
#include <string.h>
#include "dll.h"
// 定义导出函数的宏

// 辅助函数：打开注册表键
static int OpenRegKeyInternal(HKEY hKey, LPCSTR subKey, PHKEY phKeyResult) {
	LONG status = RegOpenKeyExA(hKey, subKey, 0, KEY_READ | KEY_WRITE, phKeyResult);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_FILE_NOT_FOUND:
			return 1;  // 键不存在
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 4;  // 查询值时发生错误
	}
}

// 辅助函数：创建注册表键
static int CreateRegKeyInternal(HKEY hKey, LPCSTR subKey, PHKEY phKeyResult) {
	LONG status = RegCreateKeyExA(hKey, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, phKeyResult, NULL);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 7;  // 设置值时发生错误
	}
}

// 设置注册表值
DLLIMPORT int SetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPCSTR valueData) {
	HKEY hSubKey;
	DWORD dataSize = strlen(valueData) + 1;
	int status;

	// 打开或创建注册表键
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		status = CreateRegKeyInternal(hKey, subKey, &hSubKey);
		if (status != 0) {
			return status;  // 键不存在且创建失败
		}
	}

	// 设置注册表值
	LONG setStatus = RegSetValueExA(hSubKey, valueName, 0, REG_SZ, (const BYTE*)valueData, dataSize);

	// 关闭注册表键
	RegCloseKey(hSubKey);

	switch (setStatus) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 7;  // 设置值时发生错误
	}
}

// 查询注册表值
DLLIMPORT int GetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPSTR buffer, LPDWORD bufferSize) {
	HKEY hSubKey;
	DWORD dataType;
	int status;

	// 打开注册表键
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		return status;  // 键不存在
	}

	// 查询注册表值
	LONG queryStatus = RegQueryValueExA(hSubKey, valueName, NULL, &dataType, (LPBYTE)buffer, bufferSize);

	// 关闭注册表键
	RegCloseKey(hSubKey);

	switch (queryStatus) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_FILE_NOT_FOUND:
			return 1;  // 键不存在
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 4;  // 查询值时发生错误
	}
}

// 删除注册表值
DLLIMPORT int DeleteRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName) {
	HKEY hSubKey;
	int status;

	// 打开注册表键
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		return status;  // 键不存在
	}

	// 删除注册表值
	LONG deleteStatus = RegDeleteValueA(hSubKey, valueName);

	// 关闭注册表键
	RegCloseKey(hSubKey);

	switch (deleteStatus) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_FILE_NOT_FOUND:
			return 1;  // 键不存在
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 5;  // 删除值时发生错误
	}
}

// 删除注册表键
DLLIMPORT int DeleteRegKey(HKEY hKey, LPCSTR subKey) {
	LONG status = RegDeleteKeyA(hKey, subKey);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // 成功
		case ERROR_FILE_NOT_FOUND:
			return 1;  // 键不存在
		case ERROR_ACCESS_DENIED:
			return 2;  // 访问被拒绝
		case ERROR_INVALID_HANDLE:
			return 3;  // 无效的句柄
		default:
			return 5;  // 删除值时发生错误
	}
}

// DLL入口点
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			// DLL 被加载到进程的地址空间
			break;
		case DLL_THREAD_ATTACH:
			// 创建了一个新线程，该线程可以访问 DLL
			break;
		case DLL_THREAD_DETACH:
			// 线程退出，不再访问 DLL
			break;
		case DLL_PROCESS_DETACH:
			// DLL 从进程的地址空间卸载
			break;
	}
	return TRUE;
}

