#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Macro.h"
 Texture* ConstructTexture(char* filePath){
  	Texture* texture = (Texture*)malloc(sizeof(Texture));
	CHECK_MALLOC(texture);
	stbi_set_flip_vertically_on_load(1);
    texture->m_LocalBuffer = stbi_load(filePath, &texture->m_Width, &texture->m_Height, &texture->m_BPP, 4);
	if (!texture->m_LocalBuffer) {
    fprintf(stderr, "Failed to load texture: %s\n", filePath);
    fprintf(stderr, "Error: %s\n", stbi_failure_reason());
    return NULL;
	}
    glGenTextures(1, &texture->m_RendererID);
    glBindTexture(GL_TEXTURE_2D, texture->m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->m_Width, texture->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(texture->m_LocalBuffer){ stbi_image_free(texture->m_LocalBuffer); }
	if(!texture->m_LocalBuffer){ fprintf(stderr, "Failed to load texture\n"); }

    return texture;
}


void DeleteTexture(Texture* texture){
	glDeleteTextures(1, &texture->m_RendererID);
}

void BindTexture(Texture* texture, unsigned int slot){
  	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->m_RendererID);
}

void UnbindTexture(Texture* texture){
  glBindTexture(GL_TEXTURE_2D, 0);
}