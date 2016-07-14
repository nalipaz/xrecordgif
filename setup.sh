#!/bin/bash
rm ~/bin/xrecordgif -f
gcc xrectsel.c -lX11 -o ~/bin/xrecordgif `pkg-config --cflags --libs libnotify`
chmod +x ~/bin/xrecordgif
