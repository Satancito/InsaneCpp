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

if ($EmscriptenCompiler.IsPresent) {
    Import-Module -Name "$PSScriptRoot/submodules/PsBotan/Z-PsBotan.ps1" -Force -NoClobber
    #Install-EmscriptenSDK
    $botanIncludeDir = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_CONFIGURATIONS.Release.DistDirName)/include/botan-$__PSBOTAN_BOTAN_MAJOR_VERSION"
    if(!(Test-Path -Path "$botanIncludeDir" -PathType Container))
    {
        throw "Botan lib dir not detected. Generate with `"$__PSBOTAN_GITHUB_URL`"."
    }
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
        -I "$botanIncludeDir" `
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
        -Wshorten-64-to-32 `
        -Wtautological-compare
    Write-Host "Creating compiler database file ""compile_commands.json""..."
    Join-CompileCommandsJson -SourceDir $dbBuildDir -DestinationDir $PSScriptRoot
    exit
}

