#include "Terrain.h"
#include <cstdlib>
#include <ctime>

Terrain::Terrain() {
    generate_terrain(width, height);
}

Terrain::~Terrain() {}

void Terrain::generate_terrain(int width, int height) {
    vertices.clear();
    indices.clear();

    srand(time(nullptr));

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // get the height of the location
            float terrainHeight = glm::round(get_height(x, y));

            glm::vec3 basePosition(x * voxelSize, terrainHeight * voxelSize, y * voxelSize);

            // generate a random color
            glm::vec3 color(
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
            );

            addVoxel(basePosition, color);
        }
    }
}

void Terrain::addVoxel(glm::vec3 basePosition, glm::vec3 color) {
    GLfloat halfVoxelSize = voxelSize * 0.5f; // Calculate half voxel size

    // for now il use this to add a slight shade
    glm::vec3 bottomColor = color - glm::vec3(0.2f);
    // cube vertex points with voxelsize taken into account
    GLfloat cubeVertices[] = {
        basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, bottomColor.r, bottomColor.g, bottomColor.b,  // Bottom-left-front
        basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, bottomColor.r, bottomColor.g, bottomColor.b,  // Bottom-right-front
        basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, color.r, color.g, color.b,  // Top-right-front
        basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, color.r, color.g, color.b,  // Top-left-front
        basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, bottomColor.r, bottomColor.g, bottomColor.b,  // Bottom-left-back
        basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, bottomColor.r, bottomColor.g, bottomColor.b,  // Bottom-right-back
        basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, color.r, color.g, color.b,  // Top-right-back
        basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, color.r, color.g, color.b   // Top-left-back
    };
    vertices.insert(vertices.end(), std::begin(cubeVertices), std::end(cubeVertices));

    // faces of the cube
    GLuint cubeIndices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Top face
        2, 3, 7, 7, 6, 2,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 2, 6, 6, 5, 1
    };
    
    // add indices for the cube, adding an offset based on the current number of vertices
    GLuint offset = vertices.size() / 6 - 8;
    for (GLuint i = 0; i < 30; ++i) {
        indices.push_back(cubeIndices[i] + offset);
    }
}

// creates waves, we use these as a temporary height map
float Terrain::get_height(int x, int y) {
    return sin(x * 0.1f) * cos(y * 0.1f) * 5.0f + 5.0f;
}
