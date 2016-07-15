#!/bin/bash
packages=""

has_libnotify=$(dpkg-query -W -f='${Status}' libnotify-dev 2>/dev/null | grep -c "ok installed")
[ $has_libnotify == "0" ] && packages="$packages libnotify-dev"

has_byzanz=$(dpkg-query -W -f='${Status}' byzanz 2>/dev/null | grep -c "ok installed")
[ $has_byzanz == "0" ] && packages="$packages byzanz"

if [ ! -z "$packages" ]; then
  sudo apt-get install $packages
fi

rm ~/bin/xrecordgif -f
gcc xrecordgif.c -lX11 -o ~/bin/xrecordgif `pkg-config --cflags --libs libnotify`
chmod +x ~/bin/xrecordgif
