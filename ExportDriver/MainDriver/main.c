#include <wdm.h>

DECLSPEC_IMPORT ULONG GetDllLoadCount(void);
DECLSPEC_IMPORT ULONG AddNum(ULONG num1, ULONG num2);
DECLSPEC_IMPORT ULONG SubNum(ULONG num1, ULONG num2);

#define PREFIX_DRV "MainDriver : "

void
DriverUnload(
	IN PDRIVER_OBJECT DriveObject
)
{
	UNREFERENCED_PARAMETER(DriveObject);

	KdPrint((PREFIX_DRV "DriverUnload\n"));
}

NTSTATUS
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = DriverUnload;

	KdPrint((PREFIX_DRV "DriverEntry\n"));
	KdPrint((PREFIX_DRV "Load Count = %d\n", GetDllLoadCount()));
	KdPrint((PREFIX_DRV "100 - 50 = %d\n", SubNum(100, 50)));

	return STATUS_SUCCESS;
}