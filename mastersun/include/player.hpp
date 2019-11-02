#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "quad.hpp"

class Player {
public:
	glm::vec3 m_pos;
	std::vector<quad> m_quads;

	Player() {
		m_pos = {0, 0, 0};

	};

};