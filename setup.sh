#!/bin/bash
gcc xrectsel.c -lX11 -o ~/bin/xrectsel
cp xrecordgif ~/bin/
chmod +x ~/bin/xrectsel
chmod +x ~/bin/xrecordgif