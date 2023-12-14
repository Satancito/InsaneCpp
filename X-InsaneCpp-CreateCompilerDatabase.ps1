[CmdletBinding()]
param (
    [Parameter(ParameterSetName = "Emscripten")]
    [string]
    $CppFile = "__main.cpp", 

    [Parameter(ParameterSetName = "Emscripten", Mandatory=$true)]
    [switch]
    $EmscriptenCompiler
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
$BotanVersion = & "$PSScriptRoot/submodules/PsBotan/X-PsBotan.ps1" -GetLastVersion

if ($EmscriptenCompiler.IsPresent) {
    & "$PSScriptRoot/submodules/PsEmscripten/X-PsEmscripten-SDK.ps1" -Install
    $BotanDir = "$(Get-UserHome)/.CppLibs/Botan-$BotanVersion-Release-Emscripten-ForInsane"
    Write-InfoMagenta $BotanDir
    if (!(Test-Path -Path $BotanDir -PathType Container)) {
        Write-Warning "Botan $BotanVersion($BotanDir) does not exist. Compile it with ""https://github.com/Satancito/InsaneEmscripten""."
        exit
    }
    $BotanDir = "$BotanDir/include/botan-$("$BotanVersion".Split(".") | Select-Object -First 1)"

    $objDir = "$PSScriptRoot/Build/obj"
    $dbBuildDir = "$PSScriptRoot/Build/clangd"
    New-Item -Path "$objDir" -ItemType Directory -Force | Out-Null
    New-Item -Path "$dbBuildDir" -ItemType Directory -Force | Out-Null
    Write-Host "Compiling ""$CppFile""..."
    & "$env:EMSCRIPTEN_COMPILER" `
        -c "$CppFile" `
        -o "$objDir/$CppFile.o" `
        -MJ "$dbBuildDir/$CppFile.compile_commands.json" `
        -s USE_ICU=1 `
        -I "Include" `
        -I "submodules/CommonCppIncludes" `
        -I "$BotanDir" `
        -std=c++20 `
        -Wall `
        -Wextra `
        -Wpedantic `
        -Wshadow `
        -Wstrict-aliasing `
        -Wstrict-overflow=5 `
        -Wcast-align `
        -Wmissing-declarations `
        -Wpointer-arith `
        -Wcast-qual `
        -Wshorten-64-to-32 
    Write-Host "Creating compiler database file ""compile_commands.json""..."
    Join-CompileCommandsJson -SourceDir $dbBuildDir -DestinationDir $PSScriptRoot
    exit
}