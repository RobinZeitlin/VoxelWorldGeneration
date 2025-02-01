#include "MeshRenderer.h"

#include "shaders/Shader.h"
#include "Renderer.h"

MeshRenderer::MeshRenderer(Shader* shader, GLfloat* vertices, size_t verticesSize, GLuint* indices, size_t indicesSize)
    : shader(shader), vertices(vertices), indices(indices), verticesSize(verticesSize), indicesSize(indicesSize), texture(nullptr) {

    vao = new VAO();
    vao->Bind();

    vbo = new VBO(vertices, verticesSize);
    ebo = new EBO(indices, indicesSize);

    // position attribute
    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // texture coordinate attribute
    vao->LinkAttrib(*vbo, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // normal attribute
    vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    vao->Unbind();
    ebo->Unbind();
    vbo->Unbind();
}

MeshRenderer::~MeshRenderer() {
    destroy();
}

void MeshRenderer::draw() {
    shader->Activate();

    if (texture) {
        glBindTexture(GL_TEXTURE_2D, texture->textureObject);
    }

    vao->Bind();
    ebo->Bind();

    glDrawElements(renderer->currentDrawMode, indicesSize / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MeshRenderer::apply_texture(Texture* newTexture) {
    texture = newTexture;
}

void MeshRenderer::destroy() {
    delete[] vertices;
    delete[] indices;

    vao->Delete();
    vbo->Delete();
    ebo->Delete();
}
