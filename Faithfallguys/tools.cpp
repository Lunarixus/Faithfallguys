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