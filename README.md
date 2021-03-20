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

For now, just:

```
palette-grabber [<threshold>] <image-file>
```
