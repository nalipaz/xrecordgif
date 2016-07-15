XRecordGIF
==========

Performs a screen capture of a selected area of the screen on Linux creating a .gif

## Credits
A lot of this code was taken from publically available places on the web.

[sdbbs on askubuntu.com](http://askubuntu.com/questions/487725/how-can-i-draw-selection-rectangle-on-screen-for-my-script): The screen drawing for obtaining coordinates of the rectangle.  
[lolilolicon & HashBox on archlinux boards](https://bbs.archlinux.org/viewtopic.php?id=85378): The original source-code for the screen drawing that sdbbs modified and subsequently posted.  
[wiki.archlinux.org](https://wiki.archlinux.org/index.php/Desktop_notifications): The users whom contributed to the section on how to use libnotify within C.  
Various C programming tutorials: Being this is my first project in C I learned a lot on various other sites.  

## Requirements
This has only been tested on Debian Jessie, specifically the Bunsen Labs distribution, so I can't say how well it might support other Linux distributions.

 * libnotify-dev
 * byzanz

Both the above will be installed if you don't have them when you run `./setup.sh`, of course you can install them manually with apt if you prefer.

## Installation

Clone this gist into a temporary directory and run `./setup.sh`
```
cd /tmp
git clone https://github.com/nalipaz/xrecordgif.git
cd xrecordgif
./setup.sh
```
If `~/bin` is in your `PATH` (I recommend this practice) then the script should then be immediately available.
## Usage

The script takes one argument, the number of seconds you wish to record.

Example:
```
xrecordgif 3
```

After running the command you need to draw a rectangle in the location of the area you wish to capture. Recording commences upon dropping the drag handle. The gif is saved to `~/recording.gif`.

## Example GIF

![Example GIF](https://raw.githubusercontent.com/nalipaz/xrecordgif/master/example.gif)
