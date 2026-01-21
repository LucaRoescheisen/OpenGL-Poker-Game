
#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct{
  unsigned int m_RendererID;
  char* filePath;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;
}Texture;

Texture* ConstructTexture(char* filePath);
void DeleteTexture(Texture* texture);
void BindTexture(Texture* texture, unsigned int slot);
void UnbindTexture(Texture* texture);





#endif //TEXTURE_H
