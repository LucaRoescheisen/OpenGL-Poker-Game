#include <glad/glad.h>
#include "VertexBuffer.h"
#include <stdlib.h>
#include "Macro.h"

VertexBuffer* CreateVertexBuffer(const void* data, unsigned int size){
    VertexBuffer* vb;
    vb = malloc(sizeof(VertexBuffer));
    CHECK_MALLOC(vb);
    glGenBuffers(1, &vb->m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, vb->m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return vb;
}


void DeleteVertexBuffer(VertexBuffer* vb){
    glDeleteBuffers(1, &vb->m_rendererID);
    free(vb);
}

void BindVertexBuffer(VertexBuffer* vb){
     glBindBuffer(GL_ARRAY_BUFFER, vb->m_rendererID);
}

void UnbindVertexBuffer(VertexBuffer* vb) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
