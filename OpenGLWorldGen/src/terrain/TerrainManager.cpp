#include "TerrainManager.h"

#include "camera/Camera.h"

#include "MeshRenderer.h"
#include "Renderer.h"

#include "Terrain.h"

TerrainManager::TerrainManager(Renderer* renderer, Camera* camera)
	: camera(camera), renderer(renderer) {
	std::vector<GLfloat> vertices = {
		// Positions          // Colors
		// Front face
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Top-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Top-left

		// Back face
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f   // Top-left
	};

	// Indices for drawing the cube (two triangles per face)
	std::vector<GLuint> indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		4, 0, 3, 3, 7, 4,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Top face
		3, 2, 6, 6, 7, 3,
		// Bottom face
		4, 5, 1, 1, 0, 4
	};

	placeholderMesh = new MeshRenderer(
		renderer->defaultShader,
		vertices.data(),
		vertices.size() * sizeof(GLfloat),
		indices.data(),
		indices.size() * sizeof(GLuint)
	);
	placeholderMesh->renderer = renderer;
	//renderer->meshes.push_back(placeholderMesh);
}

void TerrainManager::check_nearby_chunks() {
	return;
	Terrain* closestChunk = get_closest_chunk();
	if (closestChunk == nullptr) return;

	std::cout << "X: " << closestChunk->spawnPos.x << " Y: " << closestChunk->spawnPos.y << std::endl;

	glm::vec3 chunkPos = glm::vec3(closestChunk->spawnPos.x, 0.0f, closestChunk->spawnPos.y);
	glm::vec3 cameraPosition = glm::vec3(camera->Position.x, 0.0f, camera->Position.z);

	float distanceMagnitude = glm::distance(cameraPosition, chunkPos);
	if (distanceMagnitude > distanceToGenerate) {
		glm::vec2 nextChunkPos = get_next_chunk_pos(closestChunk, cameraPosition);
		spawn_chunk(nextChunkPos);
	}
}

void TerrainManager::spawn_chunk(glm::vec2 spawnPos) {
	glm::vec2 gridPosition = spawnPos / glm::vec2(50);

	auto spawn = [this](glm::vec2 spawnPos) {
		if (chunksSpawned[spawnPos]) return;

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
	};

	spawn(spawnPos);

	glm::vec2 directions[] = {
		glm::vec2(1, 0), // east
		glm::vec2(-1, 0), // west
		glm::vec2(0, 1), // north
		glm::vec2(0, -1), // south
		glm::vec2(1, 1), // northeast
		glm::vec2(1, -1), // southeast
		glm::vec2(-1, 1), // northwest
		glm::vec2(-1, -1) // southwest
	};

	for (auto dirs : directions) {
		spawn((gridPosition + dirs) * glm::vec2(50));
	}
}

Terrain* TerrainManager::get_closest_chunk() {
	glm::vec3 cameraPosition = camera->Position;
	float closestChunkDistance = FLT_MAX;
	Terrain* closestTerrain = nullptr;
	for (const auto& [position, terrain] : chunksSpawned) {
		auto distance = glm::distance2(position, { cameraPosition.x, cameraPosition.z });

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

