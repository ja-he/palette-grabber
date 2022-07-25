# palette-grabber

A small tool to generate a color palette from an image file using k-Means
clustering.

## Build

Clone and initialize submodules (this can be one command, but you might have
cloned already so I just demonstrate the 2-step initialization):

    $ git clone https://github.com/ja-he/palette-grabber.git
    ...
    $ git submodule update --init
    ...

Then all you need to do is build using `make`:

    $ make -j4

## Usage

You can get the help text via the `-h` / `--help` flag, stating:

```
palette-grabber: 
  Generate a color palette from an image file.

Usage: 
$ palette-grabber --input <file> [optional options]

Options:
-h,--help          : Print this help information. 
-i,--input <file>  : The input file. Must be given a valid input file name. 
-k,--k-means [<k>] : Specify 'k' for k-Means clustering. (Default: 10)
```
