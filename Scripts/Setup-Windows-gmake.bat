@echo off
REM =========================
REM Script para automatizar builds e compile_commands
REM =========================

REM Salva a pasta atual e entra na raiz do projeto
pushd ..

echo.
echo =========================
echo Generating builds...
echo =========================

REM Gera os builds usando Premake
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua gmake

echo.
echo =========================
echo Exporting compile commands...
echo =========================

REM Executa export-compile-commands
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua export-compile-commands

echo.
echo =========================
echo Combining compile commands...
echo =========================

REM Cria compile_commands.json único usando PowerShell

powershell -Command "$files = Get-ChildItem -Path compile_commands -Filter *.json; $data=@(); foreach ($f in $files) { $json=Get-Content $f.FullName | Out-String | ConvertFrom-Json; $data += $json }; $data | ConvertTo-Json -Depth 100 | Set-Content compile_commands.json"

echo.
echo =========================
echo Done! compile_commands.json is ready in project root.
echo =========================

REM Volta para a pasta original
popd
pause

