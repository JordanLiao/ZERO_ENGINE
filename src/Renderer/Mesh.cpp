#include "Mesh.h"

Mesh::~Mesh()
{
}

GLuint Mesh::getVao() const
{
    return vao;
}

void Mesh::bind() const
{
    glcheck(glBindVertexArray(vao));
}

void Mesh::unbind() const
{
    glcheck(glBindVertexArray(0));
}
