/* Include header for tools */

int GetWindowPID(const char* windowname);
uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* modulename);
uintptr_t AddPointers(HANDLE hProcess, uintptr_t addr, std::vector<unsigned int> offsets);