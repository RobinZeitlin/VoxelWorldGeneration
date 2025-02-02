#include "Terrain.h"
#include <cstdlib>
#include <ctime>
#include <gtc/noise.hpp>
#include <iostream>

#include "Renderer.h"
#include "MeshRenderer.h"

#include "../src/rendering/OBJLoader.h"
#include "../src/rendering/TextureLoader.h"

Terrain::Terrain(glm::vec2 spawnPos, Shader* shader, Renderer* renderer)
    : spawnPos(spawnPos), shader(shader), renderer(renderer) {
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
            glm::vec2 posWithOffset = glm::vec2(x - (width / 2), y - (height / 2));
            addVoxel(posWithOffset);
            spawn_tree(posWithOffset);
        }
    }
}

void Terrain::addVoxel(glm::vec2 rawPos) {
    GLfloat halfVoxelSize = voxelSize * 0.5f;
    float terrainHeight = glm::round(get_height(rawPos.x, rawPos.y));

    glm::vec3 basePosition(
        rawPos.x * voxelSize,
        terrainHeight * voxelSize,
        rawPos.y * voxelSize);

    std::vector<GLfloat> cubeVertices;

    auto lambda = [this, &terrainHeight](glm::vec2 neighbourPosition) {
        return get_height(neighbourPosition.x, neighbourPosition.y) < terrainHeight;
        };

    cubeVertices.insert(cubeVertices.end(), {
        basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, // Bottom-left
        basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, // Bottom-right
        basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, // Top-right
        basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f  // Top-left
        });
    // front face z-
    if (lambda(glm::vec2(rawPos.x, rawPos.y - 1))) {
        cubeVertices.insert(cubeVertices.end(), {
            basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f
            });
    }
    // back face z+
    if (lambda(glm::vec2(rawPos.x, rawPos.y + 1))) {
        cubeVertices.insert(cubeVertices.end(), {
            basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f
            });
    }
    // right face x+
    if (lambda(glm::vec2(rawPos.x + 1, rawPos.y))) {
        cubeVertices.insert(cubeVertices.end(), {
            basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            basePosition.x + halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
            basePosition.x + halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f
            });
    }
    // left face x-
    if (lambda(glm::vec2(rawPos.x - 1, rawPos.y))) {
        cubeVertices.insert(cubeVertices.end(), {
            basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            basePosition.x - halfVoxelSize, basePosition.y - halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z + halfVoxelSize, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            basePosition.x - halfVoxelSize, basePosition.y + halfVoxelSize, basePosition.z - halfVoxelSize, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f
            });
    }

    vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());

    std::vector<GLuint> faceIndices = { 0, 1, 2, 2, 3, 0 };

    GLuint offset = (vertices.size() / 8) - 24;
    for (int i = 0; i < 6; i++) {
        std::vector<GLuint> face = faceIndices;
        for (auto& index : face) index += offset + (i * 4);
        indices.insert(indices.end(), face.begin(), face.end());
    }
}

float Terrain::get_height(int x, int y) {
    float scale = 0.02f;
    float amplitude = 10.0f;

    float noise = glm::perlin((spawnPos + glm::vec2(x, y)) * scale);
    float height = noise * amplitude;

    return glm::round(height);
}

void Terrain::spawn_tree(glm::vec2 spawnPosTree) {
    float scale = 0.015f;
    float amplitude = 10.0f;
    float minDistance = 7.0f;

    float noise = glm::perlin((spawnPos + spawnPosTree) * scale);

    if (noise < -0.5f) {
        bool tooClose = false;
        for (const auto& tree : spawnedTrees) {
            float distance = glm::length(glm::vec2(tree.x - spawnPos.x - spawnPosTree.x, tree.z - spawnPos.y - spawnPosTree.y));
            if (distance < minDistance) {
                tooClose = true;
                break;
            }
        }

        if (tooClose) return;

        MeshRenderer* cubeMesh = OBJLoader::get_instance().load_mesh("tree", shader);

        if (cubeMesh) {
            cubeMesh->apply_texture(TextureLoader::get_instance().get_texture("tree.png"));
            cubeMesh->renderer = renderer;
            renderer->meshes.push_back(cubeMesh);

            float yHeight = get_height(spawnPosTree.x, spawnPosTree.y);  // Remove the offset inconsistency
            cubeMesh->setPosition(glm::vec3(spawnPos.x + spawnPosTree.x, yHeight * voxelSize, spawnPos.y + spawnPosTree.y));


            spawnedTrees.push_back(glm::vec3(spawnPos.x + spawnPosTree.x, yHeight, spawnPos.y + spawnPosTree.y));
        }
        else {
            std::cerr << "Failed to load tree mesh!" << std::endl;
        }
    }
}

