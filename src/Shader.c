#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Macro.h"
#define MAX_LINE_LENGTH 1024


typedef struct{
    char* vertexSource;
    char* fragmentSource;
}ShaderProgramSource;

typedef enum {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
}ShaderType;

static ShaderProgramSource ParseShader(const char* filePath);
static unsigned int CompileShader(const char* source, unsigned int type);
static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);


Shader* ConstructShader(const char* filePath){
    ShaderProgramSource source = ParseShader(filePath);
    Shader* shader = malloc(sizeof(Shader));
    CHECK_MALLOC(shader);
    shader->m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
    free(source.vertexSource);
    free(source.fragmentSource);
    return shader;
}

void DeleteShader(Shader* shader){
  glDeleteProgram(shader->m_RendererID);
    free(shader);
}

void BindShader(Shader* shader){
    glUseProgram(shader->m_RendererID);
}

void UnbindShader(Shader* shader){
    glUseProgram(0);
}


ShaderProgramSource ParseShader(const char* filepath){

    FILE* file = fopen(filepath, "r");

    if (file == NULL) {
        fprintf(stderr, "Could not open shader file %s\n", filepath);
        exit(EXIT_FAILURE);
    }else {
        printf("Shader found!\n");
    }

    char lineToBeRead[MAX_LINE_LENGTH];
    ShaderType type = NONE;
    char* vertexShaderSource = malloc(1);
    char* fragmentShaderSource = malloc(1);

    if (!vertexShaderSource || !fragmentShaderSource) {
        printf("Could not allocate memory.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    vertexShaderSource[0] = '\0';
    fragmentShaderSource[0] = '\0';
    size_t vertexShaderSourceLength = 0;
    size_t fragmentShaderSourceLength = 0;

    while (fgets(lineToBeRead, sizeof(lineToBeRead), file)) {
        if (strstr(lineToBeRead, "#shader") != NULL) {

            if (strstr(lineToBeRead, "vertex") != NULL) {
                type = VERTEX;
            }
            if (strstr(lineToBeRead, "fragment") != NULL) {
                type = FRAGMENT;
            }
        }
        else{
            size_t length = strlen(lineToBeRead);
            if (type == VERTEX) {

                vertexShaderSource = realloc(vertexShaderSource, vertexShaderSourceLength + length + 1);
                if (!vertexShaderSource) {
                    printf( "Memory allocation failed\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                strcpy(vertexShaderSource + vertexShaderSourceLength, lineToBeRead);
                vertexShaderSourceLength += length;

            }else if (type == FRAGMENT) {
                fragmentShaderSource = realloc(fragmentShaderSource, fragmentShaderSourceLength + length + 1);
                if (!fragmentShaderSource) {
                    printf( "Memory allocation failed\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                strcpy(fragmentShaderSource + fragmentShaderSourceLength, lineToBeRead);
                fragmentShaderSourceLength += length;

            }
        }

    }
    fclose(file);
    ShaderProgramSource result = {vertexShaderSource, fragmentShaderSource};
    return result;
}

unsigned int CompileShader(const char* source, unsigned int type) {

    const unsigned int id = glCreateShader(type);
    const char* src = source;


    if (id == 0) {
        printf("glCreateShader failed!\n");
        return 0;
    }

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);


    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed to compile shader: %s\n", message);
        free(message);
    }
    return id;
}


static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER );
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER );

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        CHECK_MALLOC(message);
        glGetProgramInfoLog(program, length, &length, message);
        printf("Shader program link error: %s\n", message);
        free(message);
        exit(EXIT_FAILURE);

    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


