# palette-grabber

A small tool to generate a color palette from an image file.

## Current State

Besides outdated experimentations it implements a k-Means clustering (in
RGB-space) by which a palette can be extracted.

## Usage

(You can get the help text via the `-h` / `--help` flag.)

The normal usage is
```
palette-grabber --input <file> -k <k>
```
with all options being
```
Options:
-d,--dump-colors         : Dump all colors from the image. 
-h,--help                : Print this help information. 
-i,--input <file>        : The input file. Must be given a valid input file name. 
-k,--k-means [<k>]       : Get color clusters with k-Means. 
-n,--naive [<threshold>] : Naively get unique (by euclidean distance) color values from 
                           image to form palette. Can be given a distance threshold value, 
                           default is 100.0 if none provided.
```
