@echo off
setlocal

cd %~dp0
REM .\vcpkg\bootstrap-vcpkg.bat

.\vcpkg\vcpkg install angle ode --triplet=x64-windows


endlocal

