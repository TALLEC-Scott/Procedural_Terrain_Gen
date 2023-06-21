#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "PerlinNoise.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include "lodepng.h"

// This function will be to get the colours (i have changed them around so much i am not sure if all comments correspond)
std::vector<unsigned char> getColor(double height)
{
    if (height >= 0.99)
    {
        return {15, 41, 81, 255}; // dark blue
    }
    if (height >= 0.98)
    {
        return {17, 47, 93, 255, 255}; // dark blue
    }
    else if (height >= 0.94)
    {
        return {22, 60, 113, 255}; // light blue
    }
    else if (height >= 0.92)
    {
        return {28, 77, 141, 255}; //lighter blue
    }
    else if (height >= 0.9)
    {
        return {248, 217, 133, 255, 255}; // sand
    }
    else if (height >= 0.83)
    {
        return {97, 171, 0, 255}; // green
    }
    else if (height >= 0.77)
    {
        return {10, 135, 14, 255}; // dark green
    }
    else if (height >= 0.72)
    {
        return {51, 25, 0, 255}; // dark brown
    }
    else if (height >= 0.69)
    {
        return {70, 35, 0, 255};
    }
    else if (height >= 0.65)
    {
        return {35, 35, 35, 255}; // brown
    }
    else if (height >= 0.60)
    {
        return {63, 52, 44, 255}; // light brown
    }
    else
    {
        return {255, 255, 255, 255}; // snow white
    }
}

std::vector<unsigned char> generatePerlinNoise(int width, int height, float horizontalRepeat, float verticalRepeat, float twistFactor, bool texture, 
float octaves, float lacuranity, float amplitude, float frequency, float scale, float offsetx, float offsety, float seed)
{
    PerlinNoise noise;
    if (seed != -1)
        noise = PerlinNoise(seed);

    std::vector<double> noiseMap;
    noiseMap.resize(width * height);

    std::vector<unsigned char> image;
    image.resize(width * height * 4);

   for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) { 
            double x = (double)j / ((double)width);
            double y = (double)i / ((double)height);

            // Make the noise
            double n = noise.noise(10 * x, 10 * y, 0.8, horizontalRepeat, verticalRepeat, twistFactor, octaves, lacuranity, amplitude, frequency, scale, offsetx, offsety);

            // Find the heightvalue
            double heightValue = (n + 1) / 2;

            // Create the images with the height value
            unsigned char heightPixel = heightValue * 255;
            image[4 * width * i + 4 * j + 0] = heightPixel;
            image[4 * width * i + 4 * j + 1] = heightPixel;
            image[4 * width * i + 4 * j + 2] = heightPixel;
            image[4 * width * i + 4 * j + 3] = 255;

            noiseMap[width * i + j] = heightValue;
        }
    }


    if (texture)
    {
        std::vector<unsigned char> texture;
        texture.resize(width * height * 4);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {

                double heightValue = noiseMap[width * i + j];

                std::vector<unsigned char> color = getColor(heightValue);

                // Use the color function to color each pixel in correspondance to its height
                texture[4 * width * i + 4 * j + 0] = color[0];
                texture[4 * width * i + 4 * j + 1] = color[1];
                texture[4 * width * i + 4 * j + 2] = color[2];
                texture[4 * width * i + 4 * j + 3] = color[3];
            }
        }
        return texture;
    }
    return image;
}

// This function is to load in our png files to be able to show them on the interface
GLuint loadTexture(const char* filename)
{
    std::vector<unsigned char> image;
    unsigned int width, height;

    unsigned error = lodepng::decode(image, width, height, filename);

    if (error)
    {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return -1;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

// Our main function that will open the interface and allow us to interact with it
int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Simple Slider Application", NULL, NULL);
    glfwMakeContextCurrent(window);

    bool texture = false; // to know whether we show the texture map or just the normal noise map

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    const int imageWidth = 512;
    const int imageHeight = 512;
    float twist = 1.0f;
    float vertical = 1.0f;
    float horizontal = 1.0f;
    float octaves = 1.0f;
    float lacuranity = 1.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float scale = 1.0f;
    float offsetx = 0.0f;
    float offsety = 0.0f;
    float seed = -1.0f;
    // get the first terrain texture
    std::vector<unsigned char> image = generatePerlinNoise(imageWidth, imageHeight, horizontal, vertical, twist, true, octaves, lacuranity, amplitude, frequency, scale, offsetx, offsety, seed);

    if (lodepng::encode("terrain_texture.png", image, imageWidth, imageHeight)) {
        std::cout << "Error: could not save PNG to 'terrain.png'" << std::endl;
    }

    // get the first noise
    image = generatePerlinNoise(imageWidth, imageHeight, horizontal, vertical, twist, texture, octaves, lacuranity, amplitude, frequency, scale, offsetx, offsety, seed);

    if (lodepng::encode("terrain.png", image, imageWidth, imageHeight)) {
        std::cout << "Error: could not save PNG to 'terrain.png'" << std::endl;
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());


        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(500, 720), ImGuiCond_Always);
            ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

            // Check to know if the textures are wanted
            if (ImGui::Button("Texture")) {
                texture = !texture;
            }

            GLuint image_texture;
            //choose which image we show in accordance to whether textures is cliked on or not
            if (!texture)    
                image_texture = loadTexture("terrain.png");
            else
                image_texture = loadTexture("terrain_texture.png");

            // show our image
            ImGui::Image(reinterpret_cast<void*>((intptr_t)image_texture), ImVec2(imageWidth, imageHeight));

            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(500, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(780, 720), ImGuiCond_Always);
            ImGui::Begin("Slider", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

            // Here are sliders
            if (ImGui::SliderFloat("Twist", &twist, 1.0f, 10.0f) ||
                ImGui::SliderFloat("Vertical", &vertical, 1.0f, 10.0f) ||
                ImGui::SliderFloat("Horizontal", &horizontal, 1.0f, 10.0f)||
                ImGui::SliderFloat("Octaves", &octaves, 1.0f, 10.0f)||
                ImGui::SliderFloat("Lacuranity", &lacuranity, 1.0f, 4.0f)||
                ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 1.0f)||
                ImGui::SliderFloat("Frequency", &frequency, 0.0f, 2.0f)|| 
                ImGui::SliderFloat("OffsetX", &offsetx, 0.0f, 20.0f)|| 
                ImGui::SliderFloat("OffsetY", &offsety, 0.0f, 20.0f)|| 
                ImGui::SliderFloat("Scale", &scale, 0.00f, 1.0f)|| 
                ImGui::SliderFloat("Seed", &seed, 0.00f, 100.0f))
            {
                // When slider is changed, update the image
                image = generatePerlinNoise(imageWidth, imageHeight, horizontal, vertical, twist, true, octaves, lacuranity, amplitude, frequency, scale, offsetx, offsety, seed);
                if (lodepng::encode("terrain_texture.png", image, imageWidth, imageHeight)) {
                    std::cout << "Error: could not save PNG to 'terrain.png'" << std::endl;
                }

                image = generatePerlinNoise(imageWidth, imageHeight, horizontal, vertical, twist, false, octaves, lacuranity, amplitude, frequency, scale, offsetx, offsety, seed);
                if (lodepng::encode("terrain.png", image, imageWidth, imageHeight)) {
                    std::cout << "Error: could not save PNG to 'terrain.png'" << std::endl;
                }
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }


    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
