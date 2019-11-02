#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class block {
public:
	glm::vec3 m_pos;
	GLuint m_pvbo;
	GLuint m_cvbo;
	glm::vec3 m_points[36];
	glm::vec3 m_colors[36];

	block(glm::vec3 pos = { 0,0,0 }) {
		m_pos = pos;
		glGenBuffers(1, &m_pvbo);
		glGenBuffers(1, &m_cvbo);
	}

	void genVertex(uint8_t size) {
		/*Predefined points of cube*/
		glm::vec3 points[36] = {
			{-1.0f,-1.0f,-1.0f}, // triangle 1 : begin
			{-1.0f,-1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f}, // triangle 1 : end
			{1.0f, 1.0f,-1.0f}, // triangle 2 : begin
			{-1.0f,-1.0f,-1.0f},
			{-1.0f, 1.0f,-1.0f}, // triangle 2 : end
			{1.0f,-1.0f, 1.0f},
			{-1.0f,-1.0f,-1.0f},
			{1.0f,-1.0f,-1.0f},
			{1.0f, 1.0f,-1.0f},
			{1.0f,-1.0f,-1.0f},
			{-1.0f,-1.0f,-1.0f},
			{-1.0f,-1.0f,-1.0f},
			{-1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f,-1.0f},
			{1.0f,-1.0f, 1.0f},
			{-1.0f,-1.0f, 1.0f},
			{-1.0f,-1.0f,-1.0f},
			{-1.0f, 1.0f, 1.0f},
			{-1.0f,-1.0f, 1.0f},
			{1.0f,-1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{1.0f,-1.0f,-1.0f},
			{1.0f, 1.0f,-1.0f},
			{1.0f,-1.0f,-1.0f},
			{1.0f, 1.0f, 1.0f},
			{1.0f,-1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f,-1.0f},
			{-1.0f, 1.0f,-1.0f},
			{1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f,-1.0f},
			{-1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{1.0f,-1.0f, 1.0f}
		};

		/*Scaler*/
		for (auto& a : points) {
			a = a * 0.5f;
		}

		/*Predefined colors of cube*/
		glm::vec3 colors[36] = {
			/*1.first*/
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},

			/*2.first*/
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},

			/*3.first*/
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},

			/*2.last*/
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},

			/*1.last*/
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},

			/*3.last*/
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},

			/*4.first*/
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},

			/*5.first*/
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},

			/*5.last*/
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},
			{0.0f, 0.0f,  1.0f},

			/*6.first*/
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},

			/*6.last*/
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},
			{1.0f, 0.0f,  0.0f},

			/*4.last*/
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f},
			{0.0f, 1.0f,  0.0f}
		};

		/*Translating into initial position*/
		for (auto& a : points) {
			a = a + m_pos;
		}

		memcpy(m_points, points, sizeof(points));
		memcpy(m_colors, colors, sizeof(colors));
	}

	void genColor(glm::vec3 color) {
		for (auto& a : m_colors) {
			a = color;
		}
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

		glDrawArrays(GL_TRIANGLES, 0, sizeof(m_points)/sizeof(glm::vec3));
	}
};