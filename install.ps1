$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

$Repo = "rivethorn/newcx"
$ExecName = "newcx"
$CExecName = "newc"
$CppExecName = "newcpp"

function Test-Admin {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = [Security.Principal.WindowsPrincipal]::new($identity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Add-ToPath {
    param(
        [Parameter(Mandatory = $true)]
        [string] $Directory,

        [Parameter(Mandatory = $true)]
        [ValidateSet("User", "Machine")]
        [string] $Target
    )

    $currentPath = [Environment]::GetEnvironmentVariable("Path", $Target)
    $entries = @()
    if ($currentPath) {
        $entries = $currentPath -split ";"
    }

    if ($entries -notcontains $Directory) {
        $newPath = (@($entries) + $Directory) -join ";"
        [Environment]::SetEnvironmentVariable("Path", $newPath, $Target)
        $env:Path = (@($env:Path -split ";") + $Directory) -join ";"
        Write-Host "Added $Directory to $Target PATH."
    }
}

$arch = [Runtime.InteropServices.RuntimeInformation]::OSArchitecture
switch ($arch) {
    "X64" {
        $platform = "windows-x86_64"
    }
    default {
        Write-Error "Unsupported Windows architecture: $arch"
    }
}

$release = Invoke-RestMethod "https://api.github.com/repos/$Repo/releases/latest"
$tag = $release.tag_name
if (-not $tag) {
    Write-Error "Unable to determine latest $ExecName release tag."
}

$version = $tag -replace "^v", ""
$asset = "$ExecName-v$version-$platform.exe"
$url = "https://github.com/$Repo/releases/download/$tag/$asset"
$tempFile = Join-Path ([IO.Path]::GetTempPath()) "$ExecName-$([Guid]::NewGuid()).exe"

try {
    Write-Host "Downloading $ExecName v$version for $platform..."
    Invoke-WebRequest -Uri $url -OutFile $tempFile

    $installForAllUsers = "n"
    if (-not [Console]::IsInputRedirected) {
        $installForAllUsers = Read-Host "Install for all users to Program Files? This requires Administrator. [y/N]"
    }

    $isAdmin = Test-Admin
    if ($installForAllUsers -match "^(y|yes)$") {
        if ($isAdmin) {
            $installDir = Join-Path $env:ProgramFiles "$ExecName\bin"
            $pathTarget = "Machine"
        }
        else {
            Write-Warning "Administrator privileges unavailable; falling back to user install."
            $installDir = Join-Path $env:LOCALAPPDATA "Programs\$ExecName\bin"
            $pathTarget = "User"
        }
    }
    else {
        $installDir = Join-Path $env:LOCALAPPDATA "Programs\$ExecName\bin"
        $pathTarget = "User"
    }

    New-Item -ItemType Directory -Force -Path $installDir | Out-Null

    $newcxPath = Join-Path $installDir "$ExecName.exe"
    $newcPath = Join-Path $installDir "$CExecName.exe"
    $newcppPath = Join-Path $installDir "$CppExecName.exe"

    Move-Item -Force $tempFile $newcxPath
    Copy-Item -Force $newcxPath $newcPath
    Copy-Item -Force $newcxPath $newcppPath

    Add-ToPath -Directory $installDir -Target $pathTarget

    Write-Host "$ExecName installed to $installDir"
    Write-Host "Generated $CExecName.exe and $CppExecName.exe launchers."
    Write-Host "Installation complete. Run with: $CExecName or $CppExecName"
}
finally {
    if (Test-Path $tempFile) {
        Remove-Item -Force $tempFile
    }
}
