#include <cmath>
#include <random>
#include <algorithm>
#include "PerlinNoise.hpp"

PerlinNoise::PerlinNoise() {
    p = { 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 
                      103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 
                      26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 
                      87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 
                      77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 
                      46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 
                      187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 
                      198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 
                      255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 
                      170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 
                      172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 
                      104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 
                      241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 
                      157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 
                      93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};
    p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise(unsigned int seed) {
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);
    p.insert(p.end(), p.begin(), p.end());
}


double PerlinNoise::noise(double x, double y, double z, float vertical, float horizontal, float twist, float octaves, float lacunarity, float amplitude, float frequency, float scale, float offsetx, float offsety) {
    double total = 0;

    x = x * scale + offsetx;
    y = y * scale + offsety;
    z = z * scale;

    for(int i = 0; i < octaves; i++) {
        // Twist
        double angle = twist * 2 * M_PI * frequency;
        double twistedX = x * cos(angle) - y * sin(angle);
        double twistedY = x * sin(angle) + y * cos(angle);
        // apply the vertical or hortizontal extra variables
        twistedX *= vertical * frequency;
        twistedY *= horizontal * frequency;

        int X = (int)floor(twistedX) & 255;
        int Y = (int)floor(twistedY) & 255;
        int Z = (int)floor(z * frequency) & 255;

        twistedX -= floor(twistedX);
        twistedY -= floor(twistedY);
        z -= floor(z);

        // fadeeeee
        double u = fade(twistedX);
        double v = fade(twistedY);
        double w = fade(z);

        int A = p[X] + Y;
        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int B = p[X + 1] + Y;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;

        // add results
        double res = 
        lerp(w, lerp(v, lerp(u, grad(p[AA], twistedX, twistedY, z), grad(p[BA], twistedX - 1, twistedY, z)),
        lerp(u, grad(p[AB], twistedX, twistedY - 1, z), grad(p[BB], twistedX - 1, twistedY - 1, z))),
        lerp(v, lerp(u, grad(p[AA + 1], twistedX, twistedY, z - 1), grad(p[BA + 1], twistedX - 1, twistedY, z - 1)), 
        lerp(u, grad(p[AB + 1], twistedX, twistedY - 1, z - 1), grad(p[BB + 1], twistedX - 1, twistedY - 1, z - 1))));

        // adjust the range to make 0-1
        res = (res + 1.0) / 2.0;
        
        total += res * amplitude;
        
        amplitude *= 0.5;
        frequency *= lacunarity;
    }
    return total;
}


double PerlinNoise::fade(double t) { 
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) { 
    return a + t * (b - a); 
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14) ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
