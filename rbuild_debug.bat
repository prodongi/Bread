@echo off

@call "c:\lan\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

devenv /clean debug "Bread.sln"
devenv /rebuild debug "Bread.sln"

@pause