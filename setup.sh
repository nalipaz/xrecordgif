#!/bin/bash
rm ~/bin/xrecordgif -f
gcc xrecordgif.c -lX11 -o ~/bin/xrecordgif `pkg-config --cflags --libs libnotify`
chmod +x ~/bin/xrecordgif
