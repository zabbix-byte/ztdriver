#include "zdriver.h"


extern "C" { //undocumented windows internal functions (exported by ntoskrnl)
	NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);
}

NTSTATUS driver_entry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	UNICODE_STRING device_name = {};
	RtlInitUnicodeString(&device_name, L"\\Device\\ztdriver");

	PDEVICE_OBJECT device_object = nullptr;
	NTSTATUS status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);

	if (status != STATUS_SUCCESS)
	{
		DbgPrintEx(0, 0, "[zdriver][-] Failed to create the driver device!\n");
		return status;
	}

	DbgPrintEx(0, 0, "[zdriver][+] Driver device was created!\n");

	UNICODE_STRING symbolinc_link = {};
	RtlInitUnicodeString(&symbolinc_link, L"\\DosDevices\\ztdriver");


	status = IoCreateSymbolicLink(&symbolinc_link, &device_name);

	if (status != STATUS_SUCCESS)
	{
		DbgPrintEx(0, 0, "[zdriver][-] Failed to create the driver symbolic link!\n");
		return status;
	}

	DbgPrintEx(0, 0, "[zdriver][+] Driver symbolic link was created!\n");

	// allow us to send data (like a event handler)
	SetFlag(device_object->Flags, DO_BUFFERED_IO);

	// handlers
	driver_object->MajorFunction[IRP_MJ_CREATE] = zdriver::create;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = zdriver::close;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = zdriver::device_control;

	ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);


	DbgPrintEx(0, 0, "[zdriver][+] Driver initilized succesfully!\n");

	return status;
}


NTSTATUS EntryPoint()
{
	DbgPrintEx(0, 0, "[zdriver][+] Just started!\n" );

	UNICODE_STRING driver_name = {};
	RtlInitUnicodeString(&driver_name, L"\\Driver\\ztdriver");

	return IoCreateDriver(&driver_name, &driver_entry);
}