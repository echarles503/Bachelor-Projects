# Initialize global Dictionaries

$1MBdict_128 = 
@{
    encrypt = @{}
    decrypt = @{}
}

$1MBdict_256 = 
@{
    encrypt = @{}
    decrypt = @{}
}

$500MBdict_128 = 
@{
    encrypt = @{}
    decrypt = @{}
}

$500MBdict_256 = 
@{
    encrypt = @{}
    decrypt = @{}
}

$1GBdict_128 = 
@{
    encrypt = @{}
    decrypt = @{}
}

$1GBdict_256 = 
@{
    encrypt = @{}
    decrypt = @{}
}

function Start-Tests{
    param($cipher, $dictName, $fname, $range, $verbose=0)

    for ($i = 1; $i -le $range; $i++)
    {
        if ($verbose -eq 1)
        {
            Write-Host "Iteration $($i)"
        }

        # Time encryption
        $startTime = Get-Date
        openssl $cipher -e -in "$($fname)" -out "CText-$($fname)" -pass pass:me 2>$null
        $endTime = Get-Date
        $executionTime = $endTime - $startTime
        
        # Add to dictionary
        $nthTrial = $i.toString()
        $dictName['encrypt'][$nthTrial] = $executionTime.TotalSeconds 

        # Time decryption
        $startTime = Get-Date
        openssl $cipher -d -in "CText-$($fname)" -out "PText-$($fname)" -pass pass:me 2>$null
        $endTime = Get-Date
        $executionTime = $endTime - $startTime

        $dictName['decrypt'][$nthTrial] = $executionTime.TotalSeconds 

    }

}

function New-Csv{
    param($dictName, $outfile)

    $result = 
    foreach ($subDict in $dictName.Keys)
    {
        foreach ($key in $dictName.$subDict.Keys)
        {
            [PSCustomObject]@{
                Method = $subDict
                Trial = $key
                Time = $dictName.$subDict.$key
            }
        }
    }
    $result | Export-Csv -Path "$($outfile)" -NoTypeInformation
}

# Begin Tests

Write-Host "Beginning tests for aes-128-cbc 1MB..."
Start-Tests "aes-128-cbc" $1MBdict_128 "1MB_file.txt" 100

Write-Host "Beginning tests for aes-256-cbc 1MB..."
Start-Tests "aes-256-cbc" $1MBdict_256 "1MB_file.txt" 100

Write-Host "Beginning tests for aes-128-cbc 500MB..."
Start-Tests "aes-128-cbc" $500MBdict_128 "500MB_file.txt" 25

Write-Host "Beginning tests for aes-256-cbc 500MB..."
Start-Tests "aes-256-cbc" $500MBdict_256 "500MB_file.txt" 25

Write-Host "Beginning tests for aes-128-cbc 1GB..."
Start-Tests "aes-128-cbc" $1GBdict_128 "1GB_file.txt" 25

Write-Host "Beginning tests for aes-256-cbc 1GB..."
Start-Tests "aes-256-cbc" $1GBdict_256 "1GB_file.txt" 25

# Generate csv files
Write-Host "Generating CSV files"

New-Csv $1MBdict_128 ".\1MBdata-128.csv"
New-Csv $1MBdict_256 ".\1MBdata-256.csv"
New-Csv $500MBdict_128 ".\500MBdata-128.csv"
New-Csv $500MBdict_256 ".\500MBdata-256.csv"
New-Csv $1GBdict_128 ".\1GBdata-128.csv"
New-Csv $1GBdict_256 ".\1GBdata-256.csv"

Write-Host "Done."


