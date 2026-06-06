#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace DoNotOpenGL {

struct Mesh {
	unsigned int VAO, VBO, EBO;
	unsigned int vertexCount;
	unsigned int indexCount;

	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	// Copies
	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	// Moves
	Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
};

} // namespace DoNotOpenGL