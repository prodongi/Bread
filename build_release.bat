@echo off

@call "c:\lan\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

devenv /clean release "Bread.sln"
devenv /build release "Bread.sln"

@pause