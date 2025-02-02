#pragma once

#include "../rendering/Vertex.h"

#include <vector>

struct MeshData {
    std::vector<Vertex> vertexData;
    std::vector<unsigned int> indices;
};

struct MeshToLoad {
    MeshToLoad(std::string meshName, MeshData* meshData)
        : meshName(meshName), meshData(meshData) {}

    std::string meshName;
    MeshData* meshData;
};