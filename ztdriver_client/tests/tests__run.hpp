#pragma once
#include "tests__offsets.hpp"
#include "../ztdriver_client/zdriver_client.hpp"

// [TODO]: This need to be validated or developed
/*
	zdriver_client::attach_to_process() - validated
	zdriver_client::detach_from_process() - validated
	zdriver_client::get_module_base() -  validated
	zdriver_client::read<YOUR_D_TYPE>() - validated
	zdriver_client::write<YOUR_D_TYPE>() - validated
	zdriver_client::find_maaddy() - validated
*/

bool tests_reading_writing(const uintptr_t client_module_base)
{
	std::cout << "\n";
	std::cout << "-----tests_reading_writing-----\n";

	uint32_t armour_addrs = zdriver_client::find_maaddy<uint32_t>(client_module_base + offsets::player_dll_static, offsets::armour);

	std::cout << std::endl;
	std::cout << "Armour Address: " << "0x" << std::hex << armour_addrs << "\n";
	const uint32_t old_armour = zdriver_client::read<uint32_t>(armour_addrs);
	std::cout << "Current Armour: " << std::dec << old_armour << "\n";
	std::cout << "Setting Armour to: 120 " << "\n";
	const uint32_t armour_value = 120;
	zdriver_client::write<uint32_t>(armour_addrs, armour_value);
	const uint32_t new_armour = zdriver_client::read<uint32_t>(armour_addrs);
	std::cout << "New Armour: " << std::dec << new_armour << "\n";
	std::cout << std::endl;

	if (new_armour != old_armour)
	{
		return false;
	}

	uint32_t health_addrs = zdriver_client::find_maaddy<uint32_t>(client_module_base + offsets::health_dll_static, offsets::health);

	std::cout << std::endl;
	std::cout << "Health Address: " << "0x" << std::hex << health_addrs << "\n";
	const uint32_t old_health = zdriver_client::read<uint32_t>(health_addrs);
	std::cout << "Current Health: " << std::dec << old_health << "\n";
	std::cout << "Setting Health to: 120 " << "\n";
	const uint32_t health_value = 120;
	zdriver_client::write<uint32_t>(health_addrs, health_value);
	const uint32_t new_health = zdriver_client::read<uint32_t>(health_addrs);
	
	if (new_health != old_health)
	{
		return false;
	}

	std::cout << "New Health: " << std::dec << new_health << "\n";
	std::cout << std::endl;

	std::cout << "-------------------------\n";
	std::cout << "\n";

	return true;
}


bool test_find_mmaddy(const uintptr_t client_module_base)
{
	std::cout << "\n";
	std::cout << "-----test_find_mmaddy-----\n";

	// In this case a read 4 bytes pointer values >> uint32_t

	uint64_t health_addrs = zdriver_client::find_maaddy<uint32_t>(client_module_base + offsets::health_dll_static, offsets::health);

	// manual find_maaddy to validade readers

	uint32_t a_1 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, client_module_base + offsets::health_dll_static);
	std::cout << "a_1: " << "0x" << std::hex << a_1 << "\n";

	uint32_t a_2 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, a_1 + offsets::health[0]);
	std::cout << "0x" << std::hex << a_1 << " + " << offsets::health[0] << " : " << "0x" << std::hex << a_1 + offsets::health[0] << " --> " << "0x" << std::hex << a_2 << "\n";

	uint32_t a_3 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, a_2 + offsets::health[1]);
	std::cout << "0x" << std::hex << a_2 << " + " << offsets::health[1] << " : " << "0x" << std::hex << a_2 + offsets::health[1] << " --> " << "0x" << std::hex << a_3 << "\n";

	uint32_t a_4 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, a_3 + offsets::health[2]);
	std::cout << "0x" << std::hex << a_3 << " + " << offsets::health[2] << " : " << "0x" << std::hex << a_3 + offsets::health[2] << " --> " << "0x" << std::hex << a_4 << "\n";

	uint32_t a_5 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, a_4 + offsets::health[3]);
	std::cout << "0x" << std::hex << a_4 << " + " << offsets::health[3] << " : " << "0x" << std::hex << a_4 + offsets::health[3] << " --> " << "0x" << std::hex << a_5 << "\n";

	uint32_t a_6 = zdriver::read_memory<std::uint32_t>(zdriver_client::driver, a_5 + offsets::health[4]);
	std::cout << "0x" << std::hex << a_5 << " + " << offsets::health[4] << " : " << "0x" << std::hex << a_5 + offsets::health[4] << " --> " << "0x" << std::hex << a_6 << "\n";
	uint32_t a_7 = a_6 + offsets::health[5];
	std::cout << "0x" << std::hex << a_6 << " + " << offsets::health[5] << " : " << "0x" << std::hex << a_6 + offsets::health[5] << "\n";

	// end

	if (a_7 != health_addrs)
	{
		std::cout << std::endl;
		std::cout << "[zdriver_client][-] Some problems with find_maaddy function\n";
		return false;
	}
	else
	{
		std::cout << "[zdriver_client][+] find_maaddy function works properly\n";
	}
	std::cout << "-------------------------\n";
	std::cout << "\n";

	return true;
}


bool run_tests(const uintptr_t client_module_base)
{
	std::cout << "\n";
	std::cout << "[zdriver_client][+] Running Tests\n";;
	if (!test_find_mmaddy(client_module_base))
	{
		std::cout << "[zdriver_client][-] Tests Failed \n";;
		return false;
	}
	if (!tests_reading_writing(client_module_base))
	{
		std::cout << "[zdriver_client][-] Tests Failed \n";;
		return false;
	}
	std::cout << "[zdriver_client][+] All Tests Okk \n";

	return true;
}