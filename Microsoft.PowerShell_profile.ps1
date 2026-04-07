<# 
powershell version 5.1, called from 
$env:HOMEDRIVE$env:HOMEPATH\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1
powershell version 7.6, called from 
$env:HOMEDRIVE$env:HOMEPATH\Documents\PowerShell\Microsoft.PowerShell_profile.ps1

determine powershell version : 
$PSVersionTable
#>
. $env:HOMEDRIVE$env:HOMEPATH/dev/.venv/3.13/Scripts/Activate.ps1
cd $env:HOMEDRIVE$env:HOMEPATH/dev/sdl3
