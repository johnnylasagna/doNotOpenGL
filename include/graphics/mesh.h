#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

namespace DoNotOpenGL {

struct Mesh {
	unsigned int VAO, VBO, EBO;
	unsigned int vertexCount;
	unsigned int indexCount;

	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	// copies
	Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

	//moves
    Mesh(Mesh&&) = delete;
    Mesh &operator=(Mesh&&) = delete;
};

std::shared_ptr<Mesh> createCubeMesh();
std::shared_ptr<Mesh> createSphereMesh(int stacks, int slices);
std::shared_ptr<Mesh> createFlatMesh(int height, int width);
std::shared_ptr<Mesh> createTerrainMesh(int height, int width, float tileScale);
std::shared_ptr<Mesh> createConeMesh(float radius, float height, int slices);

} // namespace DoNotOpenGL