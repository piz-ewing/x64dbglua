$currentDir = Get-Location
Set-Location $PSScriptRoot

function MakeRelease($arch) {
    xmake f -m minsizerel -a $arch
    xmake -y

    $destination = "release/lua/libs/dll/$arch/socket/"
    New-Item -ItemType Directory -Path $destination -Force
    $destinationFile = Join-Path -Path $destination -ChildPath "core.dll"
    $source = "build/windows/$arch/minsizerel/luasocket-core.dll"
    Copy-Item -Force $source $destinationFile

    $destination = "release/lua/libs/dll/$arch/mime/"
    New-Item -ItemType Directory -Path $destination -Force
    $destinationFile = Join-Path -Path $destination -ChildPath "mime.dll"
    $source = "build/windows/$arch/minsizerel/luasocket-mime.dll"
    Copy-Item -Force $source $destinationFile

    $bits = $(([int]($arch -eq 'x64') + 1)*32)
    $destination = "release/x$bits/plugins/"
    New-Item -ItemType Directory -Path $destination -Force
    $destinationFile = Join-Path -Path $destination -ChildPath "x64dbglua.dp$bits"
    $source = "build/windows/$arch/minsizerel/x64dbglua.dll"
    Copy-Item -Force $source $destinationFile
}

# copy common file
$destination = "release/"
New-Item -ItemType Directory -Path $destination -Force
Copy-Item -Force -Recurse lua $destination

$dirs = Get-ChildItem -Path $destination -Directory -Recurse -Filter ".vscode"
foreach ($dir in $dirs) {
    Remove-Item -Path $dir.FullName -Recurse -Force
}

$destination = "release/lua/libs/socket/"
New-Item -ItemType Directory -Path $destination -Force
$sourceFiles = @("3rd/luasocket/src/http.lua",
                    "3rd/luasocket/src/headers.lua",
                    "3rd/luasocket/src/ftp.lua",
                    "3rd/luasocket/src/url.lua",
                    "3rd/luasocket/src/tp.lua",
                    "3rd/luasocket/src/smtp.lua",
                    "3rd/luasocket/src/mbox.lua")
foreach ($source in $sourceFiles) {
    Copy-Item -Force $source $destination
}

$destination = "release/lua/libs/"
$sourceFiles = @("3rd/luasocket/src/socket.lua",
                    "3rd/luasocket/src/mime.lua",
                    "3rd/luasocket/src/ltn12.lua")
foreach ($source in $sourceFiles) {
    Copy-Item -Force $source $destination
}

# make release
MakeRelease("x64")
MakeRelease("x86")

if (Test-Path -Path ".\release" -PathType Container) {
    if (Test-Path -Path ".\release.zip") {
        Remove-Item -Path ".\release.zip"
    }
    Compress-Archive -Path ".\release\*" -DestinationPath ".\release.zip"

    Write-Host "Successfully compressed the contents inside the release folder to release.zip"
} else {
    Write-Host "The release folder cannot be found"
}

Set-Location $currentDir
