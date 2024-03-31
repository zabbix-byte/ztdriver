#pragma once
#include "../ztdriver_client/imports.hpp"


namespace offsets
{
	uint32_t player_dll_static = 0x17E0A8;
	uint32_t health_dll_static = 0x183828;


	std::vector<uint32_t> armour = { 0xF0 };
	std::vector<uint32_t> health = { 0x8, 0xA0, 0x30, 0x30, 0x30, 0x304 };
}