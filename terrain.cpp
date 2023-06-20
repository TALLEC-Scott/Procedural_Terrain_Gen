#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "PerlinNoise.hpp"
#include "lodepng.h"

int main()
{
    // Seed with real random number if available
    std::random_device rd;
    // Choose a random mean between 1 and 6 for our dice roll
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(1, 1000000);
    int seed = uniform_dist(e1);

    // Create a PerlinNoise object with the assigned random seed
    PerlinNoise pn(seed);

    int height = 512;
    int width = 512;

    std::vector<unsigned char> image;
    image.resize(width * height * 4);

    for(int i = 0; i < height; ++i) {     // y
        for(int j = 0; j < width; ++j) {  // x
            double x = (double)j/((double)width);
            double y = (double)i/((double)height);

            // Typical Perlin noise
            double n = pn.noise(10 * x, 10 * y, 0.8);

            // Wood like structure
            //n = 20 * pn.noise(x, y, 0.8);
            //n = n - floor(n);

            // Map the values to the [0, 255] interval, for simplicity we use 
            // tones of grey
            image[4 * width * i + 4 * j + 0] = floor(255 * n);
            image[4 * width * i + 4 * j + 1] = floor(255 * n);
            image[4 * width * i + 4 * j + 2] = floor(255 * n);
            image[4 * width * i + 4 * j + 3] = 255;
        }
    }

    // Create a .png file using lodepng
    lodepng::encode("terrain.png", image, width, height);

    return 0;
}
