#include "zdriver.hpp"
#include "process_utils.hpp"

namespace zdriver_client 
{
	const HANDLE driver = CreateFile(L"\\\\.\\ztdriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD process_id = 0;

	bool attach_to_process(const wchar_t* name)
	{
		const DWORD process_id = get_process_id(name);

		if (process_id == 0)
		{
			std::cout << "[zdriver_client][-] Some problems with the process\n";
			std::cin.get();
			return false;
		}

		zdriver_client::process_id = process_id;

		std::cout << "[zdriver_client][+] Process has ben finded\n";

		if (zdriver_client::driver == INVALID_HANDLE_VALUE)
		{
			std::cout << "[zdriver_client][-] Some problems with the handle\n";
			std::cin.get();
			return false;
		}

		std::cout << "[zdriver_client][+] User mode aplication has finded the driver\n";

		if (zdriver::attach_to_process(zdriver_client::driver, process_id) != true)
		{
			std::cout << "[zdriver_client][-] User mode aplication has some problems to attach the driver\n";
			return false;	
		}
		
		std::cout << "[zdriver_client][+] User mode aplication has attached the aplication\n";
		return true;
	}

	uintptr_t get_module_base(const wchar_t* name)
	{
		
		if (zdriver_client::process_id == 0)
			return 0;

		const uintptr_t r = get_client_base_module(zdriver_client::process_id, name);
		return r;
	}

	template <class T>
	T find_maaddy(T ptr, std::vector<unsigned int> offsets)
	{

		T addr = ptr;

		for (unsigned int i = 0; i < offsets.size(); i++)
		{
			addr = zdriver::read_memory<T>(zdriver_client::driver, addr);
			addr += offsets[i];
		}

		return addr;
	}

	template <class T>
	T read(const std::uintptr_t addr)
	{
		return zdriver::read_memory<T>(zdriver_client::driver, addr);
	}

	template <class T>
	void write(const std::uintptr_t addr, const T& value)
	{
		zdriver::write_memory(zdriver_client::driver, addr, value);
	}

	void detach_from_process()
	{
		CloseHandle(zdriver_client::driver);
	}
}