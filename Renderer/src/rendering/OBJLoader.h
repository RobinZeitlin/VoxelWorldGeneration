#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <queue>

#include <fstream>
#include <sstream>

#include <glm.hpp>

#include "MeshData.h"
#include "../shaders/Shader.h"

class MeshRenderer;

class OBJLoader {
public:
    static OBJLoader& get_instance() {
        static OBJLoader instance;
        return instance;
    }

    OBJLoader();
    ~OBJLoader();

    void clean_up();

    MeshRenderer* get_mesh(std::string objName, Shader* shader);
    MeshRenderer* load_mesh(const std::string objName, Shader* shader);

    std::unordered_map<std::string, MeshRenderer*> files;
    std::string filePath = "res/objmodels/";
    std::string fileFormat = ".obj";
};