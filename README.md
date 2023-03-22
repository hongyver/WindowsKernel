# WindowsKernel
windows kernel code snippet

# ExportDriver
Create Export Device Driver like DLL.


# HideProcess
* It was originated by <https://github.com/landhb/HideProcess>
* Files : Rootkit.sys, dkom.exe.

## How to
1. Open solution in VS2019.
2. build all
3. copy Rootkit.sys to system32\drivers.
4. dkom.exe [pid]
5. tasklist | findstr "process name"

## disable intergritycheck for test
<pre>
>bcdedit.exe /set nointegritychecks on
The operation completed successfully.
>bededit.exe -set loadoptions DISABLE INTEGRITY CHECKS
The operation completed successfully.
>bededit.exe -set TESTSIGNING ON
The operation completed successfully.
</pre>
