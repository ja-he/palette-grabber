#include <iostream>

#include <cassert>

#include "image.hh"

int
main(int argc, char* argv[])
{
  assert(argc > 1);

  Image img(argv[1]);
  printf("read in a %d by %d image with %d channels.\n",
         img.get_width(),
         img.get_height(),
         img.get_n_channels());

  return EXIT_SUCCESS;
}
