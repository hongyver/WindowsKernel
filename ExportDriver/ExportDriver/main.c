#include <wdm.h>

ULONG gDllLoadCount = 0;

ULONG __declspec(dllexport) GetDllLoadCount(void);
ULONG __declspec(dllexport) AddNum(ULONG num1, ULONG num2);
ULONG __declspec(dllexport) SubNum(ULONG num1, ULONG num2);


#define PREFIX_DRV "ExportDrv : "

NTSTATUS DllInitialize(
	_In_ PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint((PREFIX_DRV "DllInitialize : %d\n", ++gDllLoadCount));

	return STATUS_SUCCESS;
}

NTSTATUS DllUnload(void)
{
	KdPrint((PREFIX_DRV "DllUnload : %d\n", --gDllLoadCount));

	return STATUS_SUCCESS;
}

ULONG
GetDllLoadCount(void)
{
	return gDllLoadCount;
}

ULONG
AddNum(ULONG num1, ULONG num2)
{
	return num1+ num2;
}

ULONG
SubNum(ULONG num1, ULONG num2)
{
	return max(num1,num2) - min(num1, num2);
}

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
	__in PDRIVER_OBJECT DriverObject,
	__in PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = DriverUnload;

	KdPrint((PREFIX_DRV "DriverEntry\n"));

	return STATUS_SUCCESS;
}