#ifndef _NTFILEIO_H_
#define _NTFILEIO_H_

#include "nt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef NTSTATUS(__stdcall *_IoCreateSymbolicLink)(
		_In_ PUNICODE_STRING SymbolicLinkName,
		_In_ PUNICODE_STRING DeviceName
	);

	typedef NTSTATUS(__stdcall *_IoDeleteSymbolicLink)(
		_In_ PUNICODE_STRING SymbolicLinkName
	);

	typedef NTSTATUS(__stdcall *_NtCreateFile)(
		PHANDLE FileHandle,
		ACCESS_MASK DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes,
		PIO_STATUS_BLOCK IoStatusBlock,
		PLARGE_INTEGER AllocationSize,
		ULONG FileAttributes,
		ULONG ShareAccess,
		ULONG CreateDisposition,
		ULONG CreateOptions,
		PVOID EaBuffer,
		ULONG EaLength
		);

	typedef ULONG(__stdcall *_RtlNtStatusToDosError)(
		_In_ NTSTATUS Status
	);

	typedef VOID(__stdcall *_RtlInitUnicodeString)(
		PUNICODE_STRING DestinationString,
		PCWSTR SourceString
		);

	typedef NTSTATUS(__stdcall *_NtReadFile)(
		_In_     HANDLE           FileHandle,
		_In_opt_ HANDLE           Event,
		_In_opt_ PIO_APC_ROUTINE  ApcRoutine,
		_In_opt_ PVOID            ApcContext,
		_Out_    PIO_STATUS_BLOCK IoStatusBlock,
		_Out_    PVOID            Buffer,
		_In_     ULONG            Length,
		_In_opt_ PLARGE_INTEGER   ByteOffset,
		_In_opt_ PULONG           Key
	);

	typedef NTSTATUS(__stdcall *_NtWriteFile)(
		_In_     HANDLE           FileHandle,
		_In_opt_ HANDLE           Event,
		_In_opt_ PIO_APC_ROUTINE  ApcRoutine,
		_In_opt_ PVOID            ApcContext,
		_Out_    PIO_STATUS_BLOCK IoStatusBlock,
		_In_     PVOID            Buffer,
		_In_     ULONG            Length,
		_In_opt_ PLARGE_INTEGER   ByteOffset,
		_In_opt_ PULONG           Key
	);

	typedef NTSTATUS(__stdcall *_NtClose)(
		_In_ HANDLE Handle
	);

	typedef NTSTATUS(__stdcall *_NtQueryInformationFile)(
		_In_  HANDLE                 FileHandle,
		_Out_ PIO_STATUS_BLOCK       IoStatusBlock,
		_Out_ PVOID                  FileInformation,
		_In_  ULONG                  Length,
		_In_  FILE_INFORMATION_CLASS FileInformationClass
	);

	typedef NTSTATUS(__stdcall *_NtQueryDirectoryFile)(
		_In_     HANDLE                 FileHandle,
		_In_opt_ HANDLE                 Event,
		_In_opt_ PIO_APC_ROUTINE        ApcRoutine,
		_In_opt_ PVOID                  ApcContext,
		_Out_    PIO_STATUS_BLOCK       IoStatusBlock,
		_Out_    PVOID                  FileInformation,
		_In_     ULONG                  Length,
		_In_     FILE_INFORMATION_CLASS FileInformationClass,
		_In_     BOOLEAN                ReturnSingleEntry,
		_In_opt_ PUNICODE_STRING        FileName,
		_In_     BOOLEAN                RestartScan
	);

	typedef NTSTATUS(__stdcall *_NtSetInformationFile)(
		_In_  HANDLE                 FileHandle,
		_Out_ PIO_STATUS_BLOCK       IoStatusBlock,
		_In_  PVOID                  FileInformation,
		_In_  ULONG                  Length,
		_In_  FILE_INFORMATION_CLASS FileInformationClass
	);

	typedef NTSTATUS(__stdcall *_NtWaitForSingleObject)(
		_In_ HANDLE         Handle,
		_In_ BOOLEAN        Alertable,
		_In_ PLARGE_INTEGER Timeout
	);



#ifdef	__cplusplus
}
#endif


#endif /*_NTFILEIO_H_*/