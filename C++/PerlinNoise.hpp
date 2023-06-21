#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

class PerlinNoise {
    std::vector<int> p;
    float horizontalRepeat;
    float verticalRepeat;
    float twistFactor;

public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);
    double noise(double x, double y, double z, float vertical, float horizontal, float twist, float octaves, float lacuranity, float amplitude, float frequency);
private:
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};

#endif
