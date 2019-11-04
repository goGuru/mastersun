#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Mesh {
public:
	glm::vec3 m_pos;
	GLuint m_pvbo;
	GLuint m_cvbo;

	GLuint m_ebo;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_colors;

	int m_rows;
	int m_cols;

	bool m_needsUpdate = true;

	Mesh(glm::vec3 pos = { 0,0,0 }, int rows = 2, int cols = 2) {
		m_pos = pos;
		m_rows = rows;
		m_cols = cols;
		glGenBuffers(1, &m_pvbo);
		glGenBuffers(1, &m_cvbo);
		glGenBuffers(1, &m_ebo);
		genVertex();
		genColor({ 1,0,0 });
	}

	void genVertex() {
		m_vertices.clear();
		for (int x = 0; x < m_cols; x++) {
			for (int y = 0; y < m_rows; y++) {
				m_vertices.push_back({ x,4.0f * ((float)rand() / (RAND_MAX)) - 0.5f ,y });
			}
		}
	}

	void genColor(glm::vec3 color) {
		m_colors.clear();
		for(int i = 0; i < m_vertices.size(); i += 6){
			glm::vec3 color = { ((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f };

			m_colors.push_back(color);
			m_colors.push_back(color);
			m_colors.push_back(color);
			m_colors.push_back(color);
			m_colors.push_back(color);
			m_colors.push_back(color);
		}
	}

	void show() {
		if (m_needsUpdate) {
			glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
			glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW);

			std::vector<uint32_t> indices;

			for (int i = 0; i < m_rows * m_cols; i++) {
				if (i % m_rows > m_rows - 1) continue;
				if (i == m_rows * m_cols - 1) break;

				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i + m_rows);
				indices.push_back(i + m_rows);
				indices.push_back(i + 1);
				indices.push_back(i + m_rows + 1);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

			m_needsUpdate = false;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		glDrawElements(GL_TRIANGLES, m_vertices.size() * sizeof(glm::vec3), GL_UNSIGNED_INT, 0);
	}
};