#pragma once

#include "glm.hpp"

#include <unordered_map>

class Camera;
class Terrain;
class Renderer;
class MeshRenderer;

namespace std {
	template <>
	struct hash<glm::vec2> {
		size_t operator()(const glm::vec2& v) const {
			size_t h1 = std::hash<float>()(v.x);
			size_t h2 = std::hash<float>()(v.y);
			return h1 ^ (h2 << 1);
		}
	};
}

class TerrainManager {
public:
	TerrainManager(Renderer* renderer, Camera* camera);

	void check_nearby_chunks();
	void spawn_chunk(glm::vec2 spawnPos);
	std::unordered_map<glm::vec2, Terrain*> chunksSpawned;

private:
	Terrain* get_closest_chunk();
	glm::vec2 get_next_chunk_pos(Terrain* targetChunk, glm::vec3 cameraPos);

	Camera* camera;
	Renderer* renderer;

	float distanceToGenerate = 5;

	MeshRenderer* placeholderMesh;
};