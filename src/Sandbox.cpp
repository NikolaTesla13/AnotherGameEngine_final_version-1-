#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include <Window.h>
#include <Shader.h>
#include <Texture.h>
#include <Renderer.h>

int main(int argc, char** argv)
{
    Window window("Sandbox", 1280, 720);
    Shader shader("assets/shaders/shader.vertex.glsl","assets/shaders/shader.fragment.glsl");
    Texture texture("assets/images/my-logo.png");
    Renderer::Init();

    while (window.isOpen())
    {
        window.startFrame();
        shader.start();

        Renderer::ResetStats();
        Renderer::BeginBatch();

        for (float y = 0.0f; y < 720.0f; y += 25.0f)
        {
            for (float x = 0.0f; x < 1280.0f; x += 25.0f)
            {
                glm::vec4 color = { (x-200) / 255, 0.3f, (y-200) / 255, 1.0f };
                Renderer::DrawQuad({ x, y }, { 20.0f, 20.0f }, color);
            }
        }

        Renderer::DrawQuad({ 1280.0f/2.0f-250.0f/2, 720.0f/2.0f-250.0f/2 }, { 250.0f, 250.0f }, texture.getID());

        Renderer::EndBatch();
        Renderer::Flush();

        window.endFrame();
    }

    Renderer::Shutdown();

    return 0;
}