Create Exporting Driver
Copy all sys files to Drivers folder and StartService using OSRLoader.exe


* DebugView log
============================================
ExportDrv : DllInitialize : 1
MainDriver : DriverEntry
MainDriver : Load Count = 1
MainDriver : 100 - 50 = 50
SubDriver : DriverEntry
SubDriver : Load Count = 1
SubDriver : 10 + 20 = 30
MainDriver : DriverUnload
SubDriver : DriverUnload
ExportDrv : DllUnload : 0
=============================================
