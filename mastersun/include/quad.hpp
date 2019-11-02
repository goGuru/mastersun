#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class quad {
public:
	glm::vec3 m_pos;
	GLuint m_pvbo;
	GLuint m_cvbo;
	glm::vec3 m_points[4];
	glm::vec3 m_colors[4];

	quad(glm::vec3 pos = {0,0,0}) {
		m_pos = pos;
		glGenBuffers(1, &m_pvbo);
		glGenBuffers(1, &m_cvbo);
	}

	void genVertex(uint8_t size) {
		m_points[0] = { 0, 0, 0 };
		m_points[1] = { size, 0 , 0 };
		m_points[2] = { size, 0, size };
		m_points[3] = { 0, 0, size };

		/*Translating into initial position*/
		for (auto& a : m_points) {
			a = a + m_pos;
		}
	}

	void genColor(glm::vec3 color) {
		m_colors[0] = color;
		m_colors[1] = color;
		m_colors[2] = color;
		m_colors[3] = color;
	}

	void show() {
		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_QUADS, 0, 4);
	}
};