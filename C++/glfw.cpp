#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glut.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "PerlinNoise.hpp"
#include "lodepng.h"

// Generate Perlin noise function
std::vector<unsigned char> generatePerlinNoise(int width, int height, float horizontalRepeat, float verticalRepeat, float twistFactor)
{
    // Create an instance of the PerlinNoise class
    PerlinNoise noise;

    std::vector<double> noiseMap;
    noiseMap.resize(width * height);

    std::vector<unsigned char> image;
    image.resize(width * height * 4);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double nx = static_cast<double>(x) / width * horizontalRepeat;
            double ny = static_cast<double>(y) / height * verticalRepeat;

            // Apply twist factor
            double angle = twistFactor * 2 * M_PI;
            double twistedX = nx * cos(angle) - ny * sin(angle);
            double twistedY = nx * sin(angle) + ny * cos(angle);

            // Generate Perlin noise value
            double value = noise.noise(twistedX, twistedY, 0.0);

            double heightValue = (value + 1) / 2;

            unsigned char heightPixel = heightValue * 255;
            image[4 * width * x + 4 * y + 0] = heightPixel;
            image[4 * width * x + 4 * y + 1] = heightPixel;
            image[4 * width * x + 4 * y + 2] = heightPixel;
            image[4 * width * x + 4 * y + 3] = 255;

            noiseMap[width * y + x] = value;
        }
    }

    return image;
}

void display()
{
    // Our state
    static float horizontalRepeat = 0.0;
    static float verticalRepeat = 0.0;
    static float twistFactor = 0.0;
    std::vector<double> noiseMap;

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();

    // Create a window with your sliders
    ImGui::Begin("Perlin Noise Controls");

    ImGui::SliderFloat("Horizontal Repeat", &horizontalRepeat, 0.0f, 10.0f);
    ImGui::SliderFloat("Vertical Repeat", &verticalRepeat, 0.0f, 10.0f);
    ImGui::SliderFloat("Twist Factor", &twistFactor, 0.0f, 10.0f);

    static std::vector<unsigned char> Image;

    // If any of the sliders have been changed, regenerate the noise
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        Image = generatePerlinNoise(512, 512, horizontalRepeat, verticalRepeat, twistFactor);
    }

    ImGui::End();

    if (!Image.empty())
    {
        GLuint my_opengl_texture;
        glGenTextures(1, &my_opengl_texture);
        glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Image[0]);
        ImGui::Image((void*)(intptr_t)my_opengl_texture, ImVec2(512, 512));
        glDeleteTextures(1, &my_opengl_texture);
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    display_w = glutGet(GLUT_WINDOW_WIDTH);
    display_h = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();

}

int main(int argc, char** argv)
{
    // Initialize FreeGLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Perlin Noise Demo");

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        return 1;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Initialize ImGui Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL3_Init();

    // Run the main loop
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}