# palette-grabber

A small tool to generate a color palette from an image file.

## Current State

Right now this is very basic.
It can only naively grab any colors it encounters that aren't close to any
already grabbed color, by euclidean distance in RGB-space.

Eventually this should implement getting a palette by some sort of clustering.
In fact I'm mostly drawn to making this (fairly unneeded) tool because of the
opportunity to implement a clustering algorithm in C++ and think about color
spaces to cluster in.

## Usage

(You can get the help text via the `-h` / `--help` flag.)

The normal usage is
```
palette-grabber --input <file> [optional options]
```
with all options being
```
Options:
-d,--dump-colors         : Dump all colors from the image.
-h,--help                : Print this help information.
-i,--input <file>        : The input file. Must be given a valid input file name.
-n,--naive [<threshold>] : Naively get unique (by euclidean distance) color values from image to form palette.
                           Can be given a distance threshold value, default is 100.0 if none provided.
```
