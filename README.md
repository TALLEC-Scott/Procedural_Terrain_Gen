```markdown
# ISIM
A generator for procedural textures, rendered with Unity, utilizing a Perlin Noise implementation.

## C++ Part: Procedural Texture Generation

### Prerequisites
Ensure you have `libimgui-dev` and `libglfw3-dev` installed. If not, you can install them using the following command in the shell:

```bash
sudo apt-get install libglfw3-dev libimgui-dev
```

### Compilation
To compile the code, use the following command:

```bash
g++ -Wall -Wextra -pedantic -std=c++17 Main.cpp PerlinNoise.cpp lodepng.cpp -o terrain_creator -limgui -lstb -lX11 -lglfw -lGL
```

### Execution
After compilation, you can run the application using:

```bash
./terrain_creator
```
The interface should appear, allowing you to interact with the procedural texture generator.

**Note**: You may need to download additional libraries as the application uses `glfw` and `imgui`.

### Running on Ubuntu through Windows
If you're running Ubuntu through Windows, you might need a program like VcXsrv to execute the application.

In the C++ file directory, you will find the application already compiled. If you have the correct libraries, you can run it using `./terrain_creator`.

You will also find:
- `terrain.png`: Corresponds to the noise map.
  - `terrain_texture`: Corresponds to the textured noise map.

## Unity Section: Rendering Textures

To utilize the generated textures in Unity:
1. Open a project with Unity and load the sample scene.
   2. Select the terrain generators in the scene's objects.
   3. Utilize it to generate a noise map/height map, color map, and mesh.
   4. You can then play around with the different parameters such as
      - Frequency​
      - Octaves
      - Lacunarity
      - Offset
      - Amplitude


```
