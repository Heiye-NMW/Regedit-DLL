#include <windows.h>
#include <string.h>
#include "dll.h"
// ���嵼�������ĺ�

// ������������ע����
static int OpenRegKeyInternal(HKEY hKey, LPCSTR subKey, PHKEY phKeyResult) {
	LONG status = RegOpenKeyExA(hKey, subKey, 0, KEY_READ | KEY_WRITE, phKeyResult);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_FILE_NOT_FOUND:
			return 1;  // ��������
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 4;  // ��ѯֵʱ��������
	}
}

// ��������������ע����
static int CreateRegKeyInternal(HKEY hKey, LPCSTR subKey, PHKEY phKeyResult) {
	LONG status = RegCreateKeyExA(hKey, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, phKeyResult, NULL);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 7;  // ����ֵʱ��������
	}
}

// ����ע���ֵ
DLLIMPORT int SetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPCSTR valueData) {
	HKEY hSubKey;
	DWORD dataSize = strlen(valueData) + 1;
	int status;

	// �򿪻򴴽�ע����
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		status = CreateRegKeyInternal(hKey, subKey, &hSubKey);
		if (status != 0) {
			return status;  // ���������Ҵ���ʧ��
		}
	}

	// ����ע���ֵ
	LONG setStatus = RegSetValueExA(hSubKey, valueName, 0, REG_SZ, (const BYTE*)valueData, dataSize);

	// �ر�ע����
	RegCloseKey(hSubKey);

	switch (setStatus) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 7;  // ����ֵʱ��������
	}
}

// ��ѯע���ֵ
DLLIMPORT int GetRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPSTR buffer, LPDWORD bufferSize) {
	HKEY hSubKey;
	DWORD dataType;
	int status;

	// ��ע����
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		return status;  // ��������
	}

	// ��ѯע���ֵ
	LONG queryStatus = RegQueryValueExA(hSubKey, valueName, NULL, &dataType, (LPBYTE)buffer, bufferSize);

	// �ر�ע����
	RegCloseKey(hSubKey);

	switch (queryStatus) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_FILE_NOT_FOUND:
			return 1;  // ��������
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 4;  // ��ѯֵʱ��������
	}
}

// ɾ��ע���ֵ
DLLIMPORT int DeleteRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName) {
	HKEY hSubKey;
	int status;

	// ��ע����
	status = OpenRegKeyInternal(hKey, subKey, &hSubKey);
	if (status != 0) {
		return status;  // ��������
	}

	// ɾ��ע���ֵ
	LONG deleteStatus = RegDeleteValueA(hSubKey, valueName);

	// �ر�ע����
	RegCloseKey(hSubKey);

	switch (deleteStatus) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_FILE_NOT_FOUND:
			return 1;  // ��������
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 5;  // ɾ��ֵʱ��������
	}
}

// ɾ��ע����
DLLIMPORT int DeleteRegKey(HKEY hKey, LPCSTR subKey) {
	LONG status = RegDeleteKeyA(hKey, subKey);
	switch (status) {
		case ERROR_SUCCESS:
			return 0;  // �ɹ�
		case ERROR_FILE_NOT_FOUND:
			return 1;  // ��������
		case ERROR_ACCESS_DENIED:
			return 2;  // ���ʱ��ܾ�
		case ERROR_INVALID_HANDLE:
			return 3;  // ��Ч�ľ��
		default:
			return 5;  // ɾ��ֵʱ��������
	}
}

// DLL��ڵ�
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			// DLL �����ص����̵ĵ�ַ�ռ�
			break;
		case DLL_THREAD_ATTACH:
			// ������һ�����̣߳����߳̿��Է��� DLL
			break;
		case DLL_THREAD_DETACH:
			// �߳��˳������ٷ��� DLL
			break;
		case DLL_PROCESS_DETACH:
			// DLL �ӽ��̵ĵ�ַ�ռ�ж��
			break;
	}
	return TRUE;
}

