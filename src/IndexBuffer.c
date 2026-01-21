#include <glad/glad.h>
#include "IndexBuffer.h"
#include <stdlib.h>
#include "Macro.h"

IndexBuffer* CreateIndexBuffer(const void* data, unsigned int count) {
    IndexBuffer* ib;
    ib = malloc(sizeof(IndexBuffer));
    CHECK_MALLOC(ib);
    ib->m_Count = count;
    glGenBuffers(1, &ib->m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return ib;
}


void DeleteIndexBuffer(IndexBuffer* ib){
    glDeleteBuffers(1, &ib->m_rendererID);
    free(ib);
}

void BindIndexBuffer(IndexBuffer* ib){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_rendererID);
}

void UnbindIndexBuffer(IndexBuffer* ib) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
