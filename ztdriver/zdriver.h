#pragma once
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>

extern "C" { //undocumented windows internal functions (exported by ntoskrnl)
	NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
}


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
		HANDLE process_id;
		PVOID target;
		PVOID buffer;
		SIZE_T size;
		SIZE_T return_size;
	};

	NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}

	NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}

	// event handler
	NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		// read wich control (attach, read, write) is called
		PIO_STACK_LOCATION stack_irp = IoGetCurrentIrpStackLocation(irp);

		// access the object sent from used mode
		auto request = reinterpret_cast<Request*>(irp->AssociatedIrp.SystemBuffer);
		if (stack_irp == nullptr || request == nullptr)
		{
			IoCompleteRequest(irp, IO_NO_INCREMENT);
			return status;
		}

		static PEPROCESS target_process = nullptr;
		const ULONG control_code = stack_irp->Parameters.DeviceIoControl.IoControlCode;

		switch (control_code)
		{
		case codes::attach:
			// Attach Request target
			status = PsLookupProcessByProcessId(request->process_id, &target_process);
			DbgPrintEx(0, 0, "[zdriver][+] Device control called - attaching!\n");
			break;
		case codes::read:
			if (target_process != nullptr)
				// Read memory Request target
				DbgPrintEx(0, 0, "[zdriver][+] -------------------------------!\n");
				DbgPrintEx(0, 0, "[zdriver][+] Device control called - reading!\n");
				status = MmCopyVirtualMemory(target_process, request->target, PsGetCurrentProcess(), request->buffer, request->size, KernelMode, &request->return_size);
				DbgPrintEx(0, 0, "[zdriver][+] target_process: %p!\n", target_process);
				DbgPrintEx(0, 0, "[zdriver][+] buffer pointer: %p!\n", request->buffer);
				DbgPrintEx(0, 0, "[zdriver][+] target pointer: %p!\n", request->target);
				DbgPrintEx(0, 0, "[zdriver][+] size: %zu!\n", request->size);
				DbgPrintEx(0, 0, "[zdriver][+] output size: %zu!\n", request->return_size);
				DbgPrintEx(0, 0, "[zdriver][+] -------------------------------!\n");
			break;
		case codes::write:
			if (target_process != nullptr)
				// Write memory Request target
				DbgPrintEx(0, 0, "[zdriver][+] -------------------------------!\n");
				DbgPrintEx(0, 0, "[zdriver][+] Device control called - writing!\n");
				
				status = MmCopyVirtualMemory(PsGetCurrentProcess(), request->buffer, target_process, request->target, request->size, KernelMode, &request->return_size);
				DbgPrintEx(0, 0, "[zdriver][+] target_process: %p!\n", target_process);
				DbgPrintEx(0, 0, "[zdriver][+] buffer pointer: %p!\n", request->buffer);
				DbgPrintEx(0, 0, "[zdriver][+] target pointer: %p!\n", request->target);
				DbgPrintEx(0, 0, "[zdriver][+] size: %zu!\n", request->size);
				DbgPrintEx(0, 0, "[zdriver][+] output size: %zu!\n", request->return_size);
				DbgPrintEx(0, 0, "[zdriver][+] -------------------------------!\n");
			break;
		default:
			DbgPrintEx(0, 0, "[zdriver][-] Device control called - no existing option!\n");
			break;
		}


		irp->IoStatus.Status = status;
		irp->IoStatus.Information = sizeof(Request);

		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return status;
	}
}
