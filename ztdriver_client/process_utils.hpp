#include "imports.hpp"


static DWORD get_process_id(const wchar_t* process_name)
{
	DWORD proc_id = 0;
	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snap_shot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 proc_entry;
		proc_entry.dwSize = sizeof(proc_entry);

		if (Process32FirstW(snap_shot, &proc_entry))
		{
			do
			{
				if (!_wcsicmp(proc_entry.szExeFile, process_name))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32NextW(snap_shot, &proc_entry));
		}
	}

	CloseHandle(snap_shot);
	return proc_id;
}

uintptr_t get_client_base_module(const DWORD process_id, const wchar_t* module_name)
{
	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	uintptr_t mod_base_addrs = 0;

	if (snap_shot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32 mod_entry;

	mod_entry.dwSize = sizeof(mod_entry);

	if (Module32FirstW(snap_shot, &mod_entry))
	{
		do
		{
			if (!_wcsicmp(mod_entry.szModule, module_name))
			{
				mod_base_addrs = (uintptr_t)mod_entry.modBaseAddr;
				break;
			} 
		} while (Module32NextW(snap_shot, &mod_entry));
	}


	CloseHandle(snap_shot);

	return mod_base_addrs;
}