#pragma once
#include "imports.hpp"

// https://learn.microsoft.com/es-es/windows/win32/api/ioapiset/nf-ioapiset-deviceiocontrol

namespace zdriver
{
	namespace codes
	{
		constexpr ULONG attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}

	// Conection between user mode execution and kernel mode execution
	struct Request
	{
		DWORD process_id;
		PVOID target;
		PVOID buffer;
		SIZE_T size;
		SIZE_T return_size;
	};


	// The idea in future is to tranform this in a class to not pass driver_handle (Handle) every time.
	bool attach_to_process(HANDLE driver_handle, const DWORD process_id)
	{
		Request driver_req = {};
		driver_req.process_id = process_id;

		// Out buffer is the same like in.
		return DeviceIoControl(
			driver_handle,
			codes::attach,
			&driver_req,
			sizeof(driver_req),
			&driver_req,
			sizeof(driver_req),
			nullptr,
			nullptr
		);

	}

	template <class T>
	T read_memory(HANDLE driver_handle, const std::uintptr_t addr)
	{
		T read_data = {};

		Request driver_req = {};
		driver_req.target = reinterpret_cast<PVOID>(addr);
		// this will put the result (the buffer) in temp.
		driver_req.buffer = &read_data;
		driver_req.size = sizeof(T);

		// call io
		DeviceIoControl(
			driver_handle,
			codes::read,
			&driver_req, 
			sizeof(driver_req), 
			&driver_req, 
			sizeof(driver_req),
			nullptr,
			nullptr
		);

		return read_data;
	}

	template <class T>
	void write_memory(HANDLE driver_handle, const std::uintptr_t addr, const T& value)
	{
		Request driver_req = {};
		driver_req.target = reinterpret_cast<PVOID>(addr);
		driver_req.buffer = (PVOID)&value;
		driver_req.size = sizeof(T);

		// call io
		DeviceIoControl(
			driver_handle,
			codes::write,
			&driver_req,
			sizeof(driver_req),
			&driver_req,
			sizeof(driver_req),
			nullptr,
			nullptr
		);
	}
}