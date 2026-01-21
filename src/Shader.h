#ifndef SHADER_H
#define SHADER_H



typedef struct{
    unsigned int m_RendererID;
    char* filePath;
}Shader;

Shader* ConstructShader(const char* filePath);
void DeleteShader(Shader* shader);
void BindShader(Shader* shader);
void UnbindShader(Shader* shader);
int GetUniformLocation(const char* name);



#endif //SHADER_H
