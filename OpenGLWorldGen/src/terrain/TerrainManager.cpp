#include "TerrainManager.h"

#include "camera/Camera.h"

#include "MeshRenderer.h"
#include "Renderer.h"

#include "Terrain.h"

TerrainManager::TerrainManager(Renderer* renderer, Camera* camera)
	: camera(camera), renderer(renderer) {
}

void TerrainManager::check_nearby_chunks() {
	Terrain* closestChunk = get_closest_chunk();
	if (closestChunk == nullptr) return;

	glm::vec3 chunkPos = glm::vec3(closestChunk->spawnPos.x, 0.0f, closestChunk->spawnPos.y);
	glm::vec3 cameraPosition = glm::vec3(camera->Position.x, 0.0f, camera->Position.z);

	float distanceMagnitude = glm::distance(cameraPosition, chunkPos);
	if (distanceMagnitude > distanceToGenerate) {
		glm::vec2 nextChunkPos = get_next_chunk_pos(closestChunk, cameraPosition);
		spawn_chunk(nextChunkPos);
	}
}

void TerrainManager::spawn_chunk(glm::vec2 spawnPos) {
	if (!chunksSpawned[spawnPos]) {
		Terrain* newTerrain = new Terrain(spawnPos);
		MeshRenderer* newMesh = new MeshRenderer(
			renderer->defaultShader,
			newTerrain->vertices.data(),
			newTerrain->vertices.size() * sizeof(GLfloat),
			newTerrain->indices.data(),
			newTerrain->indices.size() * sizeof(GLuint)
		);

		newMesh->renderer = renderer;
		renderer->meshes.push_back(newMesh);

		chunksSpawned[spawnPos] = newTerrain;
	}
}

Terrain* TerrainManager::get_closest_chunk() {
	glm::vec3 cameraPosition = camera->Position;
	float closestChunkDistance = FLT_MAX;
	Terrain* closestTerrain = nullptr;
	for (const auto& [position, terrain] : chunksSpawned) {
		auto distance = glm::distance(position, { cameraPosition.x, cameraPosition.z });

		if (distance < closestChunkDistance) {
			closestChunkDistance = distance;
			closestTerrain = terrain;
		}
	}

	return closestTerrain;
}

glm::vec2 TerrainManager::get_next_chunk_pos(Terrain* targetChunk, glm::vec3 cameraPos) {
	glm::vec3 direction = cameraPos - glm::vec3(0);
	direction = glm::normalize(direction);
	direction = glm::vec3(glm::round(direction.x), glm::round(direction.y), glm::round(direction.z));

	glm::vec2 positionWithDirection = glm::vec2(
		direction.x * 50 + targetChunk->spawnPos.x,
		direction.z * 50 + targetChunk->spawnPos.y);

	return positionWithDirection;
}
