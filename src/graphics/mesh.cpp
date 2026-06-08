#include "graphics/mesh.h"
#include <utility>
#include <math.h>

namespace DoNotOpenGL {

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int> &indices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexCount = vertices.size();
	indexCount = indices.size();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

std::shared_ptr<Mesh> createCubeMesh() {
	std::vector<float> vertices = {
	    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
	    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
	    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
	    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //

	    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
	    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
	    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
	    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //

	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
	    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
	    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
	    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //

	    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
	    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
	    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
	    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //

	    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
	    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
	    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
	    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //

	    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
	    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
	    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
	    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
	};

	std::vector<unsigned int> indices = {
	    0,  1,  2,  0,  2,  3,  // front
	    4,  5,  6,  4,  6,  7,  // back
	    8,  9,  10, 8,  10, 11, // left
	    12, 13, 14, 12, 14, 15, // right
	    16, 17, 18, 16, 18, 19, // top
	    20, 21, 22, 20, 22, 23, // bottom
	};

	return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> createSphereMesh(int stacks, int slices) {

	constexpr float radius = 1.0f;
	constexpr float PI = 3.1415927;

	std::vector<float> vertices;
	vertices.reserve((stacks + 1) * (slices + 1) * 8);

	for (int i = 0; i <= stacks; i++) {
		float theta = i * PI / stacks; // 0 to PI

		for (int j = 0; j <= slices; j++) {
			float phi = j * 2.0f * PI / slices; // 0 to 2*PI

			float x = radius * std::sin(theta) * std::cos(phi);
			float y = radius * std::sin(theta) * std::sin(phi);
			float z = radius * std::cos(theta);

			// Position
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Normal
			vertices.push_back(x / radius);
			vertices.push_back(y / radius);
			vertices.push_back(z / radius);

			// Texture coordinates
			vertices.push_back((float)j / slices);
			vertices.push_back((float)i / stacks);
		}
	}

	std::vector<unsigned int> indices;
	indices.reserve(stacks * slices * 6);

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			int row1 = i * (slices + 1);
			int row2 = (i + 1) * (slices + 1);

			// Triangle 1
			indices.push_back(row1 + j);
			indices.push_back(row2 + j);
			indices.push_back(row1 + j + 1);

			// Triangle 2
			indices.push_back(row1 + j + 1);
			indices.push_back(row2 + j);
			indices.push_back(row2 + j + 1);
		}
	}

	return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> createFlatMesh(int height, int width) {

	std::vector<float> vertices;
	vertices.reserve(width * height * 8);

	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			// Vertex
			vertices.push_back(x);
			vertices.push_back(0);
			vertices.push_back(z);

			// Normal
			vertices.push_back(0);
			vertices.push_back(1.0f);
			vertices.push_back(0);

			// Texture
			vertices.push_back(x / (float)(width - 1));
			vertices.push_back(z / (float)(height - 1));
		}
	}

	std::vector<unsigned int> indices;
	indices.reserve(height * width * 6);

	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			int topLeft = z * width + x;

			int topRight = topLeft + 1;
			int bottomLeft = topLeft + width;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	return std::make_shared<Mesh>(vertices, indices); 
}

} // namespace DoNotOpenGL