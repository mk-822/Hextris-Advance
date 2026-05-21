Add-Type -AssemblyName System.Drawing

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path

function Darken-Rect($bmp, [int]$x0, [int]$y0, [int]$w, [int]$h) {
    for($y = $y0; $y -lt ($y0 + $h); $y++) {
        for($x = $x0; $x -lt ($x0 + $w); $x++) {
            $c = $bmp.GetPixel($x, $y)
            $bmp.SetPixel($x, $y, [System.Drawing.Color]::FromArgb([int]($c.R / 2), [int]($c.G / 2), [int]($c.B / 2)))
        }
    }
}

$frame = New-Object System.Drawing.Bitmap((Join-Path $repo 'src\Image\frame_dbmp.bmp'))
$key = $frame.GetPixel(0, 0)

try {
    for($i = 0; $i -lt 12; $i++) {
        $sourceName = ('bg{0:D2}.bmp' -f $i)
        $outputName = ('bg{0:D2}_dbmp.bmp' -f $i)
        $sourcePath = Join-Path $repo "src\Image\$sourceName"

        $src = New-Object System.Drawing.Bitmap($sourcePath)
        $dst = New-Object System.Drawing.Bitmap($src.Width, $src.Height, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb)
        $g = [System.Drawing.Graphics]::FromImage($dst)
        $g.DrawImage($src, 0, 0, $src.Width, $src.Height)
        $g.Dispose()
        $src.Dispose()

        Darken-Rect $dst 88 8 67 144
        Darken-Rect $dst 16 8 50 86

        for($fy = 0; $fy -lt 160; $fy++) {
            for($fx = 2; $fx -lt 81; $fx++) {
                $c = $frame.GetPixel($fx, $fy)
                if($c.ToArgb() -ne $key.ToArgb()) {
                    $dst.SetPixel(80 + $fx, $fy, $c)
                }
            }
        }

        $outPath = Join-Path $repo "src\Image\$outputName"
        $dst.Save($outPath, [System.Drawing.Imaging.ImageFormat]::Bmp)
        $dst.Dispose()
        Write-Output "rebaked $outputName from $sourceName"
    }
}
finally {
    $frame.Dispose()
}
