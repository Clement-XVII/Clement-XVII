mode con cols=45 lines=23
function difference
{

    process {Write-Host $_ -ForegroundColor yellow -BackgroundColor black}

}

echo "#############################################" | difference
echo "####         Script Installation         ####" | difference
echo "#############################################" | difference
Write-host "             Service Informatique            " -ForegroundColor black -BackgroundColor White
Write-host "Please wait..."

Write-host "Installation de VLC:"
Start-Sleep -s 3
cd C:\Users\$env:UserName\Desktop\Action_Informatique\Applications\
Start-Process vlc-3.0.16-win64.exe /S -Wait
echo [OK] | difference

Write-host "Installation de TeamViewer:"
Start-Sleep -s 3
Start-Process TeamViewer_Setup_x64.exe /S -Wait
echo [OK] | difference

Write-host "Installation de Chrome:"
Start-Sleep -s 3
Start-Process -Wait -FilePath msiexec -ArgumentList '/i googlechromestandaloneenterprise64.msi /quiet'
echo [OK] | difference

Write-host "Installation de 7zip:"
Start-Sleep -s 3
Start-Process 7zip.exe /S -Wait
echo [OK] | difference

Write-host "Installation de Adobe Reader:"
Start-Sleep -s 3
msiexec /i "AcroRead.msi" TRAMSFORMS=Adobe.mst /quiet
echo [OK] | difference

Write-host "Changement du fond d'ecran..."
Start-Sleep -s 3

Function Set-WallPaper($Image) {
 
Add-Type -TypeDefinition @" 
using System; 
using System.Runtime.InteropServices;
  
public class Params
{ 
    [DllImport("User32.dll",CharSet=CharSet.Unicode)] 
    public static extern int SystemParametersInfo (Int32 uAction, 
                                                   Int32 uParam, 
                                                   String lpvParam, 
                                                   Int32 fuWinIni);
}
"@ 
  
    $SPI_SETDESKWALLPAPER = 0x0014
    $UpdateIniFile = 0x01
    $SendChangeEvent = 0x02
  
    $fWinIni = $UpdateIniFile -bor $SendChangeEvent
  
    $ret = [Params]::SystemParametersInfo($SPI_SETDESKWALLPAPER, 0, $Image, $fWinIni)
 
}
 
Set-WallPaper -Image "C:\Windows\Web\Screen\img103.jpg"
echo [OK] | difference

Write-host "OEM:"
Start-Sleep -s 3
$strPath = "HKLM:\Software\Microsoft\Windows\CurrentVersion\OEMInformation"
Set-ItemProperty -Path $strPath -Name Logo -Value "C:\Users\$env:UserName\Desktop\Action_Informatique\Application\logo.bmp"
Set-ItemProperty -Path $strPath -Name Manufacturer -Value "Action Informatique"
Set-ItemProperty -Path $strPath -Name Model -Value "Action_Informatique-PC"
Set-ItemProperty -Path $strPath -Name SupportPhone -Value "05 59 84 84 29"
Set-ItemProperty -Path $strPath -Name SupportHours -Value "09:00 – 12:00 14:00 – 18:00"
Set-ItemProperty -Path $strPath -Name SupportURL -Value https://actioninformatique.fr
echo [OK] | difference

Write-host "Redemarrage dans 20s..."
Start-Sleep -s 20
Restart-Computer