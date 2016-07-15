XRecordGIF
==========

Performs a screen capture of a selected area on Debian creating a .gif

## Requirements
This has only been tested on Debian Jessie, specifically the Bunsen Labs distribution, so I can't say how well it might support other Linux distributions.

 * libnotify-dev
 * byzanz

Both the above will be installed if you don't have them when you run `./setup.sh`, of course you can install them manually with apt if you prefer.

## Installation

Clone this gist into a temporary directory and run `./setup.sh`
```
cd /tmp
git clone https://gist.github.com/98351fa5d759a216a1836a40aa6ef8f9.git xrecordgif
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

![Example GIF](/nalipaz/xrecordgif/raw/master/example.gif)
