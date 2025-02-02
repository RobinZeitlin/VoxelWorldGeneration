#include "OBJLoader.h"

#include "../MeshRenderer.h"

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader() {
    clean_up();
}

void OBJLoader::clean_up() {
    for (auto& [key, file] : files)
    {
        delete file;
    }
    files.clear();
}

MeshRenderer* OBJLoader::get_mesh(std::string objName, Shader* shader) {
    if (files[objName]) {
        std::cout << "Returning OBJ" << std::endl;
        return files[objName];
    }

    load_mesh(objName, shader);

    return nullptr;
}

MeshRenderer* OBJLoader::load_mesh(const std::string objName, Shader* shader) {
    std::string objPath = filePath + objName;

    std::ifstream file(objPath + fileFormat);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << objPath << std::endl;
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> positionIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            glm::vec3 vertexPoint;
            iss >> vertexPoint.x >> vertexPoint.y >> vertexPoint.z;
            positions.push_back(vertexPoint);
        }
        else if (prefix == "vt") {
            glm::vec2 textureCoordinate;
            iss >> textureCoordinate.x >> textureCoordinate.y;
            uvs.push_back(textureCoordinate);
        }
        else if (prefix == "vn") {
            glm::vec3 normalCoordinates;
            iss >> normalCoordinates.x >> normalCoordinates.y >> normalCoordinates.z;
            normals.push_back(normalCoordinates);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vert_word_str;
                iss >> vert_word_str;
                if (vert_word_str.empty()) break;

                std::istringstream vert_word(vert_word_str);
                int pos_i, uv_i, normal_i;

                vert_word >> pos_i;
                vert_word.ignore(1);
                vert_word >> uv_i;
                vert_word.ignore(1);
                vert_word >> normal_i;

                positionIndices.push_back(pos_i - 1);
                uvIndices.push_back(uv_i - 1);
                normalIndices.push_back(normal_i - 1);
            }
        }
    }

    file.close();

    std::vector<Vertex> vertexData;
    std::vector<unsigned int> indices;
    std::unordered_map<int64_t, int> map;

    for (int i = 0; i < positionIndices.size(); i++) {
        int pos_i = positionIndices[i];
        int uv_i = uvIndices[i];
        int normal_i = normalIndices[i];

        Vertex v = {
            positions[pos_i].x, positions[pos_i].y, positions[pos_i].z,
            uvs[uv_i].x, uvs[uv_i].y,
            normals[normal_i].x, normals[normal_i].y, normals[normal_i].z,
        };

        int64_t key = ((int64_t)pos_i << 40) | ((int64_t)uv_i << 20) | normal_i;
        auto found = map.find(key);
        if (found == map.end()) {
            map[key] = vertexData.size();
            indices.push_back(vertexData.size());
            vertexData.push_back(v);
        }
        else {
            indices.push_back(found->second);
        }
    }

    GLfloat* verticesArray = new GLfloat[vertexData.size() * 8];
    GLuint* indicesArray = new GLuint[indices.size()];

    for (size_t i = 0; i < vertexData.size(); i++) {
        verticesArray[i * 8 + 0] = vertexData[i].x;
        verticesArray[i * 8 + 1] = vertexData[i].y;
        verticesArray[i * 8 + 2] = vertexData[i].z;
        verticesArray[i * 8 + 3] = vertexData[i].u;
        verticesArray[i * 8 + 4] = vertexData[i].v;
        verticesArray[i * 8 + 5] = vertexData[i].nx;
        verticesArray[i * 8 + 6] = vertexData[i].ny;
        verticesArray[i * 8 + 7] = vertexData[i].nz;
    }

    for (size_t i = 0; i < indices.size(); i++) {
        indicesArray[i] = indices[i];
    }

    MeshRenderer* newMeshRenderer = new MeshRenderer(
        shader, 
        verticesArray, 
        vertexData.size() * 8 * sizeof(GLfloat), 
        indicesArray, 
        indices.size() * sizeof(GLuint)
);

    files[objName] = newMeshRenderer;

    std::cout << "Finished Loading " << objName << " with " << vertexData.size() << " vertices." << std::endl;

    return newMeshRenderer;
}
