# isim
Un generator for procedural textures, rendered with Unity

Perlin Noise implementation


For the C++ Part:

To run the code please use the following in a shell:

g++ -Wall -Wextra -pedantic -std=c++17 Main.cpp PerlinNoise.cpp lodepng.cpp -o terrain -limgui -lstb -lX11 -lglfw -lGL

This means you might need to download some extra libraries to run this as it is using glfw and imgui.

Pour run sur ubuntu a travers windows il vous faudra un programme comme VcXsrv.