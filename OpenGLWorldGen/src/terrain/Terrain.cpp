#include "Terrain.h"
#include <cstdlib>
#include <ctime>

#include <gtc/noise.hpp>

Terrain::Terrain(glm::vec2 spawnPos) 
: spawnPos(spawnPos) {
    generate_terrain(width, height);
}

Terrain::~Terrain() {}

void Terrain::generate_terrain(int width, int height) {
    vertices.clear();
    indices.clear();

    srand(time(nullptr));

    vertices.reserve(width * height * 8);
    indices.reserve(width * height * 6);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // generate a random color
            glm::vec3 color(
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
            );

            glm::vec2 posWithOffset = glm::vec2(x - (width / 2), y - (height / 2)) + spawnPos;
            addVoxel(posWithOffset, color);
        }
    }
}

void Terrain::addVoxel(glm::vec2 rawPos, glm::vec3 color) {

    GLfloat halfVoxelSize = voxelSize * 0.5f; // Calculate half voxel size
    // get the height of the location
    float terrainHeight = glm::round(get_height(rawPos.x, rawPos.y));

    glm::vec3 basePosition(
        rawPos.x * voxelSize, 
        terrainHeight * voxelSize, 
        rawPos.y * voxelSize);

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
    std::vector<GLuint> cubeIndices = {
        // top face
        2, 3, 7, 7, 6, 2,
    };

    auto lambda = [this, &cubeIndices, &terrainHeight](glm::vec2 neighbourPosition, std::vector<GLuint> additionalIndices) {
        if (glm::round(get_height(neighbourPosition.x, neighbourPosition.y)) < terrainHeight) {
            cubeIndices.insert(cubeIndices.end(), additionalIndices.begin(), additionalIndices.end());
        }
    };

    lambda({ rawPos.x + 1, rawPos.y }, { 1, 2, 6, 6, 5, 1 }); // right
    lambda({ rawPos.x - 1, rawPos.y }, { 0, 3, 7, 7, 4, 0 }); // left
    lambda({ rawPos.x, rawPos.y - 1 }, { 0, 1, 2, 2, 3, 0 }); // forward
    lambda({ rawPos.x, rawPos.y + 1 }, { 4, 5, 6, 6, 7, 4 }); // back

    // add indices for the cube, adding an offset based on the current number of vertices
    GLuint offset = vertices.size() / 6 - 8;
    for (GLuint i = 0; i < cubeIndices.size(); ++i) {
        indices.push_back(cubeIndices[i] + offset);
    }
}

// perlin noise
float Terrain::get_height(int x, int y) {
    float scale = 0.02f;
    float amplitude = 10.0f;

    float noise = glm::perlin(glm::vec2(x, y) * scale);
    float height = noise * amplitude;

    return height;
}
