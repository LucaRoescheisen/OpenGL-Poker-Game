#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct PokerApp PokerApp;

typedef struct{
    unsigned int m_RendererID;

}TextTexture;

TextTexture GenerateTextureAtlas(FT_Face face);
void RenderText(char* text, float x, float y, float sx, float sy, PokerApp* app);

void DeleteTextTexture(TextTexture* texture);
void BindTextTexture(TextTexture* texture, unsigned int slot);
void UnbindTextTexture(TextTexture* texture);



#endif //TEXTTEXTURE_H
