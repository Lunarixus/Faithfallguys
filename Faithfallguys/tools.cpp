/* 
* Tools helper for Faithfallguys 
*/

#include "imports.h"

int GetWindowPID(const char *windowname) {
    int pid = 0;
    DWORD pidDw;
    HWND hWnd = FindWindowA(0, windowname);
    GetWindowThreadProcessId(hWnd, &pidDw);
    pid = (int)pidDw;
    return pid;
}

uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* modulename)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modulename))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t AddPointers(HANDLE hProcess, uintptr_t addr, std::vector<unsigned int> offsets)
{
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++bData, ++bMask) {
		if (*szMask == 'x' && *bData != *bMask) {
			return false;
		}
	}
	return (*szMask == NULL);
}

DWORD FindSignature(HANDLE hProcess, DWORD start, DWORD size, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[size];
	SIZE_T bytesRead;

	ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead);

	for (DWORD i = 0; i < size; i++)
	{
		if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
			return start + i;
		}
	}
	delete[] data;
	return NULL;
}