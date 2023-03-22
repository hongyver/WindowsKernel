# WindowsKernel
windows kernel code snippet

# ExportDriver
Create Export Device Driver like DLL.


# HideProcess
it was originated by @landhb/HideProcess.
Files : Rootkit.sys, dkom.exe.

## How to

1. copy Rootkit.sys to system32\drivers.
2. dkom.exe [pid]
3. tasklist | findstr "process name"

## disable intergritycheck for test
<pre>
>bcdedit.exe /set nointegritychecks on
The operation completed successfully.
>bededit.exe -set loadoptions DISABLE INTEGRITY CHECKS
The operation completed successfully.
>bededit.exe -set TESTSIGNING ON
The operation completed successfully.
</pre>
