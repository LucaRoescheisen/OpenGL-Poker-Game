#include <../dependencies/glad/glad.h>
#include <../dependencies/GLFW/glfw3.h>
#include "textTexture.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "App.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Macro.h"

 struct CharacterInfo{
    float advanceX;
    float advanceY;
    float bitmapWidth;
    float bitmapHeight;
    float bitmapLeft;
    float bitmapTop;
    float tx;

}c[128];

int atlasWidth = 0;
int atlasHeight = 0;

TextTexture GenerateTextureAtlas(FT_Face face){
    FT_Set_Pixel_Sizes(face, 0, 24);
    FT_GlyphSlot g = face->glyph;
    int glyphWidth = 0;
    int glyphHeight = 0;


    TextTexture textTexture;

    for(int i = 32; i < 128; i++){
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)){
            printf("Failed to load Glyph\n");
            continue;
          }
        glyphWidth = g->bitmap.width;
        glyphHeight = max(glyphHeight, g->bitmap.rows);
        atlasWidth += g->bitmap.width + 1;
    }
    //Ignore the following comments, they are just here to remind me what does what. Honestly just sanity checks


    atlasHeight = glyphHeight;
    printf("Atlas width = %d, height = %d\n", atlasWidth, atlasHeight);

     (glActiveTexture(GL_TEXTURE0));  //Activate a texture unit at slot 0
     (glGenTextures(1, &textTexture.m_RendererID));
     (glBindTexture(GL_TEXTURE_2D, textTexture.m_RendererID));

     (glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0)); // Set to 0 to tell openGL we will fill it later.....
     (glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // Tells OpenGL how many bytes each row is aligned to
     (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
     (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
     (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
     (glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));


    int x = 0;
    for(int i = 32; i < 128; i++){
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)){
            continue;
        }
         (glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer));


        c[i].advanceX = g->advance.x >> 6;            //Aparently a bitwise shift of 6, pretty much divides by 6???? wtf
        c[i].advanceY = g->advance.y >> 6;
        c[i].bitmapWidth = g->bitmap.width;
        c[i].bitmapHeight = g->bitmap.rows;
        c[i].bitmapLeft = g->bitmap_left;
        c[i].bitmapTop = g->bitmap_top;
        c[i].tx = (float)x /atlasWidth;

        x += g->bitmap.width +1;
    }

    return textTexture;
}
void RenderText(char* text, float x, float y, float sx, float sy, PokerApp* app) {
    glUseProgram(app->textShader->m_RendererID);
    glBindVertexArray(app->textVAO);
    glEnableVertexAttribArray(app->in_v4_coordLocation);
    glBindBuffer(GL_ARRAY_BUFFER,  app->textVBO);
    glVertexAttribPointer(app->in_v4_coordLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, app->textVBO);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, app->textTexture.m_RendererID);



  const int length = strlen(text);
    struct point{
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };
    struct point* coords = malloc(sizeof(struct point) * 6 * length);
    CHECK_MALLOC(coords);
    int n = 0;

    for(char *p = text; *p ; p++){
        float x2 = x + c[*p].bitmapLeft * sx; // X is starting pos, then we are  adding horizontal offset * the size (sx)
        float y2 = -y - c[*p].bitmapTop * sy; // -y as text is placed on the baselines so you need to shift it downwards
        float w = c[*p].bitmapWidth * sx;
        float h = c[*p].bitmapHeight * sy;

        x += c[*p].advanceX * sx;
        y += c[*p].advanceY * sy;

        if(!w || !h){
          continue;
        }
        //Builds 2 triangles to render a glyph onto
        coords[n++] = (struct point){x2, -y2, c[*p].tx, 0};
        coords[n++] = (struct point){x2 + w, -y2    , c[*p].tx + c[*p].bitmapWidth / atlasWidth,   0};
        coords[n++] = (struct point){x2,     -y2 - h, c[*p].tx, c[*p].bitmapHeight / atlasHeight};

        coords[n++] = (struct point){x2 + w, -y2    , c[*p].tx + c[*p].bitmapWidth / atlasWidth,   0};
        coords[n++] = (struct point){x2,     -y2 - h, c[*p].tx,  c[*p].bitmapHeight / atlasHeight};
        coords[n++] = (struct point){x2 + w, -y2 - h, c[*p].tx + c[*p].bitmapWidth / atlasWidth,c[*p].bitmapHeight / atlasHeight};
        //printf("Text position: (%f, %f), Size: (%f, %f)\n", x, y, w, h);
    }
         (glBufferData(GL_ARRAY_BUFFER, sizeof(struct point) * n, coords, GL_DYNAMIC_DRAW));
         (glDrawArrays(GL_TRIANGLES, 0, n));
            glBindVertexArray(0);
         glDisableVertexAttribArray(app->in_v4_coordLocation);
        free(coords);
}

void DeleteTextTexture(TextTexture* texture){
    glDeleteTextures(1, &texture->m_RendererID);
}

void BindTextTexture(TextTexture* texture, unsigned int slot){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->m_RendererID);
}

void UnbindTextTexture(TextTexture* texture){
    glBindTexture(GL_TEXTURE_2D, 0);
}