
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

typedef struct{
    unsigned int m_rendererID;
}VertexBuffer;

VertexBuffer* CreateVertexBuffer(const void* data, unsigned int size);
void DeleteVertexBuffer(VertexBuffer* vb);
void BindVertexBuffer(VertexBuffer* vb);
void UnbindVertexBuffer(VertexBuffer* vb);


#endif //VERTEXBUFFER_H
