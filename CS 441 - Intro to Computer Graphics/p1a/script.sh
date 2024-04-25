#!/bin/bash
# Automate image viewing
./proj1A
pnmtopng proj1A_out.pnm >out.png
feh out.png
rm out.png
rm proj1A_out.pnm
echo Done!
