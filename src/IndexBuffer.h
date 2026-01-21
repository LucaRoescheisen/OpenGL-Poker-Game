
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
typedef struct{
    unsigned int m_rendererID;
    unsigned int m_Count;
}IndexBuffer;

IndexBuffer* CreateIndexBuffer(const void* data, unsigned int count);
void DeleteIndexBuffer(IndexBuffer* vb);
void BindIndexBuffer(IndexBuffer* vb);
void UnbindIndexBuffer(IndexBuffer* vb);
#endif //INDEXBUFFER_H
