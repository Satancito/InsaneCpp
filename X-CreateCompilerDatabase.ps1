[CmdletBinding()]
param (
    [Parameter(ParameterSetName = "Emscripten")]
    [Parameter(ParameterSetName = "Android")]
    [string]
    $CppFile = "__main.cpp", 

    [Parameter(ParameterSetName = "Emscripten", Mandatory = $true)]
    [switch]
    $EmscriptenCompiler,

    [Parameter(ParameterSetName = "Android", Mandatory = $true)]
    [switch]
    $AndroidCompiler,

    [Parameter(ParameterSetName = "Android")]
    [string]
    $AndroidAPI = [string]::Empty,

    [Parameter(ParameterSetName = "Android", Mandatory=$true)]
    [string]
    $NormalizedABI


)
$ErrorActionPreference = "Stop"
Import-Module -Name "$PSScriptRoot/submodules/PsBotan/Z-PsBotan.ps1" -Force -NoClobber
Import-Module -Name "$PSScriptRoot/submodules/PsICU/Z-PsIcu.ps1" -Force -NoClobber
$CXXFLAGS = @(
    "-std=c++20", 
    "-DINSANE_EXPORTS=1", 
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wshadow",
    "-Wstrict-aliasing",
    "-Wstrict-overflow=5",
    "-Wcast-align",
    "-Wmissing-declarations",
    "-Wpointer-arith",
    "-Wcast-qual",
    "-Wshorten-64-to-32",
    "-Wtautological-compare"
)
$objDir = "$PSScriptRoot/Build/obj"
$dbBuildDir = "$PSScriptRoot/Build/clangd"
New-Item -Path "$objDir" -ItemType Directory -Force | Out-Null
New-Item -Path "$dbBuildDir" -ItemType Directory -Force | Out-Null
if ($EmscriptenCompiler.IsPresent) {
    Install-EmscriptenSDK
    $compiler = "$env:EMSCRIPTEN_COMPILER"
    $botanIncludeDir = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_BUILD_CONFIGURATIONS.WasmRelease.DistDirName)/include/botan-$__PSBOTAN_BOTAN_MAJOR_VERSION"
    if (!(Test-Path -Path "$botanIncludeDir" -PathType Container)) {
        throw "Botan lib dir not detected. Generate with `"$__PSBOTAN_GITHUB_URL`"."
    }
    $CXXFLAGS += @("-s USE_ICU=1", "-I$botanIncludeDir")
}
if($AndroidCompiler.IsPresent){
    Install-AndroidNDK
    $AndroidAPI = Test-AndroidNDKApi -Api $AndroidAPI -Assert
    $compiler = Get-AndroidNDKCompiler -AndroidAPI $AndroidAPI -NormalizedABI $NormalizedABI
    $botanIncludeDir = "$(Get-CppLibsDir)/$($__PSBOTAN_ANDROID_BUILD_CONFIGURATIONS[$__PSCOREFXS_ANDROIDNDK_ANDROID_ABI_CONFIGURATIONS.$NormalizedABI.NameRelease].DistDirName -f $AndroidAPI)/include/botan-$__PSBOTAN_BOTAN_MAJOR_VERSION"
    if (!(Test-Path -Path "$botanIncludeDir" -PathType Container)) {
        throw "Botan lib dir not detected. Generate with `"$__PSBOTAN_GITHUB_URL`"."
    }
    $icuIncludeDir = "$(Get-CppLibsDir)/$($__PSICU_ANDROID_BUILD_CONFIGURATIONS[$__PSCOREFXS_ANDROIDNDK_ANDROID_ABI_CONFIGURATIONS.$NormalizedABI.NameRelease].DistDirName -f $AndroidAPI)/include/unicode"
    if (!(Test-Path -Path "$icuIncludeDir" -PathType Container)) {
        throw "ICU lib dir not detected. Generate with `"$__PSICU_GITHUB_URL`"."
    }
    $CXXFLAGS += @("-I$icuIncludeDir", "-I$botanIncludeDir")
}


    
Write-Host "Compiling ""$CppFile""..."

& "$compiler" `
    -c "$CppFile" `
    -o "$objDir/$CppFile.o" `
    -MJ "$dbBuildDir/$CppFile.compile_commands.json" `
    -I"Include" `
    -I"submodules/CommonCppIncludes" `
    $CXXFLAGS

Write-Host "Creating compiler database file ""compile_commands.json""..."
Join-CompileCommandsJson -SourceDir $dbBuildDir -DestinationDir $PSScriptRoot

# Remove-Item -Path "$objDir" -Force -Recurse -ErrorAction Ignore
# Remove-Item -Path "$dbBuildDir" -Force -Recurse -ErrorAction Ignore
exit


