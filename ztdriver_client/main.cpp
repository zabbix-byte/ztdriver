#include "tests__run.hpp"



int main()
{
	// [TODO]: This need to be validated or developed
	/*
		zdriver_client::attach_to_process() - validated
		zdriver_client::detach_from_process() - validated
		zdriver_client::get_module_base() -  validated
		zdriver_client::read<YOUR_D_TYPE>() - validated
		zdriver_client::write<YOUR_D_TYPE>() - validated
		zdriver_client::find_maaddy() - validated
	*/

	std::cout << std::endl;

	if (!zdriver_client::attach_to_process(L"ac_client.exe"))
	{
		zdriver_client::detach_from_process();
		return 1;
	}

	const uintptr_t client_module_base = zdriver_client::get_module_base(L"ac_client.exe");

	if (client_module_base == 0)
	{
		std::cout << "[zdriver_client][-] ac_client.exe\n";
		return 1;
	}

	if (!run_tests(client_module_base))
	{
		zdriver_client::detach_from_process();
		return 1;
	}

	zdriver_client::detach_from_process();
	return 0;
}