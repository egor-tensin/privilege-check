param(
    [string] $BuildDir = 'C:\Projects\build',
    [string] $ProjectDir = $null,
    [string] $Platform = $null,
    [string] $Configuration = $null
)

$ErrorActionPreference = "Stop";

function Invoke-Exe {
    param(
        [ScriptBlock] $Cmd,
        [int[]] $AllowedExitCodes = @(0)
    )

    $backupErrorActionPreference = $script:ErrorActionPreference
    $script:ErrorActionPreference = 'Continue'

    try {
        & $Cmd
        if ($AllowedExitCodes -notcontains $LastExitCode) {
            throw "External command failed with exit code ${LastExitCode}: $Cmd"
        }
    } finally {
        $script:ErrorActionPreference = $backupErrorActionPreference
    }
}

function Test-AppVeyor {
    return Test-Path env:APPVEYOR
}

function Build-Project {
    param(
        [Parameter(Mandatory=$true)]
        [string] $ProjectDir,
        [Parameter(Mandatory=$true)]
        [string] $BuildDir,
        [Parameter(Mandatory=$true)]
        [string] $Platform,
        [Parameter(Mandatory=$true)]
        [string] $Configuration
    )

    mkdir $BuildDir
    cd $BuildDir

    Invoke-Exe { cmake -Wno-dev                 `
        -G "Visual Studio 14 2015" -A $Platform `
        $ProjectDir
    }
    
    Invoke-Exe { cmake --build . --config "$Configuration" -- /m }
}

if (Test-AppVeyor) {
    $cwd = pwd
    $ProjectDir = $env:APPVEYOR_BUILD_FOLDER
    $BuildDir = 'C:\Projects\build'
    $Platform = $env:PLATFORM
    $Configuration = $env:CONFIGURATION
}

Build-Project                         `
    -ProjectDir $ProjectDir           `
    -BuildDir $BuildDir               `
    -Platform $Platform               `
    -Configuration $Configuration

if (Test-AppVeyor) {
    cd $cwd
}
