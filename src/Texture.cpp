#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Texture.h>
#include <Debugger.h>

Texture::Texture(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    auto* pixels = stbi_load(path.c_str(), &width, &height, &bits, STBI_rgb_alpha);

    glCall(glCreateTextures(GL_TEXTURE_2D, 1, &ID));
    glCall(glBindTexture(GL_TEXTURE_2D, ID));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    stbi_image_free(pixels);

    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int Texture::getID()
{
    return ID;
}