#include <iostream>

#include <algorithm>
#include <numeric>

#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[]) {
    int width, height, n_channels;

    uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &n_channels, 3);
    std::cout << "read in a " << width << "x" << height << " image with " << n_channels << " channels." << std::endl;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        uint32_t value = 0;
        for (int c = 0; c < 3; c++) {
          value += rgb_image[(i*width*3)+(j*3)+c];
        }
        value = value / 3;
        char c;
        if      (value <  50) c = ' ';
        else if (value < 100) c = '.';
        else if (value < 150) c = '-';
        else if (value < 200) c = '+';
        else                  c = '#';
        std::cout << c << c;
      }
      std::cout << std::endl;
    }

    stbi_image_free(rgb_image);

    return 0;
}
