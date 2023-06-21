# isim
Un generator for procedural textures, rendered with Unity

Perlin Noise implementation


For the C++ Part:

To run the code please use the following in a shell:

g++ -Wall -Wextra -pedantic -std=c++17 Main.cpp PerlinNoise.cpp lodepng.cpp -o terrain_creator -limgui -lstb -lX11 -lglfw -lGL

Then you will be able to run: ./terrain_creator
The interface should show up.

This means you might need to download some extra libraries to run this as it is using glfw and imgui.

Pour run sur ubuntu a travers windows il vous faudra un programme comme VcXsrv.

In the C++ file you will find the application already compiled. If you have the correct libraries you can just
run it using ./terrain_creator.

You will also find a terrain.png which corresponds to the noise map and the terrain_texture which corresponds to
the textured noise map.