#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <iostream>

namespace DoNotOpenGL {

struct Texture {
	unsigned int id{};

	Texture(const char *path);
	~Texture();

	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	Texture(Texture &&other) noexcept;
	Texture &operator=(Texture &&other) noexcept;

	void use(unsigned int unit) const;
};

} // namespace DoNotOpenGL