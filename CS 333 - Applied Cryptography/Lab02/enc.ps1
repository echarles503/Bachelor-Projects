

function Remove-Files{
    Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02\receiver"
    Remove-Item *.dat
    Remove-Item *.enc
    Remove-Item *.pem
    Remove-Item M-dec.txt
    Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02\sender"
    Remove-Item *.dat
    Remove-Item *.enc
    Remove-Item *.pem
}


function New-RSA-Key{
    param($keyLength, $privateName, $publicName)
    Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02\receiver"
    openssl genrsa -out $($privateName) $keyLength 2>$null
    openssl rsa -in $($privateName) -pubout -out "../sender/$($publicName)" 2>$null
    Write-Host "Generated $($publicName)"
}


function Start-AES-Encrypt{
    param($cipher, $fname, $key, $publicKey, $AESname)

    Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02\sender"
    
    # Create AES KEY
    openssl enc -nosalt -$($cipher) -k secret -P > $($key)

    # Time File encryption
    $startTime = Get-Date
    openssl enc -$($cipher) -in $($fname) -out "$($fname).enc" -pass file:"$($key)" 2>$null
    $endTime = Get-Date
    $encryptFile = $endTime - $startTime
    
    # Time Key encryption
    $startTime = Get-Date
    openssl rsautl -encrypt -inkey $($publicKey) -pubin -in $($key) -out "$($key).enc" 2>$null
    $endTime = Get-Date
    $encryptKey = $endTime - $startTime

    Move-Item "C:\Users\elias\Documents\CS Projects\cs333\Lab02\sender\$($key).enc" "C:\Users\elias\Documents\CS Projects\cs333\Lab02\receiver\$($AESname)"
    Move-Item "C:\Users\elias\Documents\CS Projects\cs333\Lab02\sender\$($fname).enc" "C:\Users\elias\Documents\CS Projects\cs333\Lab02\receiver\$($fname).enc"
    Write-Host "Time to encrypt file: $($encryptFile)"
    Write-Host "Time to encrypt key: $($encryptKey)"
    Write-Host "Total time: $($encryptKey + $encryptFile)"
}

function Start-AES-Decrypt{
    param($cipher, $privateKey, $AESKey, $encAESKey, $cipherText, $outfile)
    Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02\receiver"
    
    # Decrypt Key
    $startTime = Get-Date
    openssl rsautl -decrypt -inkey $privateKey -in $encAESKey -out $AESKey #2>$null
    $endTime = Get-Date
    $decryptKey = $endTime - $startTime

    # Decrypt File
    $startTime = Get-Date
    openssl enc -d -"$($cipher)" -in $cipherText -out $outfile -pass file:$($AESKey) #2>$null
    $endTime = Get-Date
    $decryptFile = $endTime - $startTime

    Write-Host "Time to decrypt file: $($decryptFile)"
    Write-Host "Time to decrypt key: $($decryptKey)"
    Write-Host "Total time: $($decryptKey + $decryptFile)"
}


# Begin Tests
<#
Write-Host "Beginning 128x1024 Tests"
New-RSA-Key 1024 "privateKey1024.pem" "publicKey1024.pem"
Start-AES-Encrypt "aes-128-cbc" "M.txt" "key128.dat" "publicKey1024.pem" "128x1024key.dat.enc"
Start-AES-Decrypt "aes-128-cbc" "privateKey1024.pem" "key128.dat" "128x1024key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 192x1024 Tests"
New-RSA-Key 1024 "privateKey1024.pem" "publicKey1024.pem"
Start-AES-Encrypt "aes-192-cbc" "M.txt" "key192.dat" "publicKey1024.pem" "192x1024key.dat.enc"
Start-AES-Decrypt "aes-192-cbc" "privateKey1024.pem" "key192.dat" "192x1024key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 256x1024 Tests"
New-RSA-Key 1024 "privateKey1024.pem" "publicKey1024.pem"
Start-AES-Encrypt "aes-256-cbc" "M.txt" "key256.dat" "publicKey1024.pem" "256x1024key.dat.enc"
Start-AES-Decrypt "aes-256-cbc" "privateKey1024.pem" "key256.dat" "256x1024key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files
#>


Write-Host "Beginning 128x2048 Tests"
New-RSA-Key 2048 "privateKey2048.pem" "publicKey2048.pem"
Start-AES-Encrypt "aes-128-cbc" "M.txt" "key128.dat" "publicKey2048.pem" "128x2048key.dat.enc"
Start-AES-Decrypt "aes-128-cbc" "privateKey2048.pem" "key128.dat" "128x2048key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 192x2048 Tests"
New-RSA-Key 2048 "privateKey2048.pem" "publicKey2048.pem"
Start-AES-Encrypt "aes-192-cbc" "M.txt" "key192.dat" "publicKey2048.pem" "192x2048key.dat.enc"
Start-AES-Decrypt "aes-192-cbc" "privateKey2048.pem" "key192.dat" "192x2048key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 256x2048 Tests"
New-RSA-Key 2048 "privateKey2048.pem" "publicKey2048.pem"
Start-AES-Encrypt "aes-256-cbc" "M.txt" "key256.dat" "publicKey2048.pem" "256x2048key.dat.enc"
Start-AES-Decrypt "aes-256-cbc" "privateKey2048.pem" "key256.dat" "256x2048key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 128x4096 Tests"
New-RSA-Key 4096 "privateKey4096.pem" "publicKey4096.pem"
Start-AES-Encrypt "aes-128-cbc" "M.txt" "key128.dat" "publicKey4096.pem" "128x4096key.dat.enc"
Start-AES-Decrypt "aes-128-cbc" "privateKey4096.pem" "key128.dat" "128x4096key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 192x4096 Tests"
New-RSA-Key 4096 "privateKey4096.pem" "publicKey4096.pem"
Start-AES-Encrypt "aes-192-cbc" "M.txt" "key192.dat" "publicKey4096.pem" "192x4096key.dat.enc"
Start-AES-Decrypt "aes-192-cbc" "privateKey4096.pem" "key192.dat" "192x4096key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Write-Host "Beginning 256x4096 Tests"
New-RSA-Key 4096 "privateKey4096.pem" "publicKey4096.pem"
Start-AES-Encrypt "aes-256-cbc" "M.txt" "key256.dat" "publicKey4096.pem" "256x4096key.dat.enc"
Start-AES-Decrypt "aes-256-cbc" "privateKey4096.pem" "key256.dat" "256x4096key.dat.enc" "M.txt.enc" "M-dec.txt"

Remove-Files

Set-Location "C:\Users\elias\Documents\CS Projects\cs333\Lab02"
Write-Host "Done."


