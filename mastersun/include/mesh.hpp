#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "PerlinNoise.hpp"
#include "FastNoise.h"

class Mesh {
public:
	glm::vec3 m_pos;
	GLuint m_pvbo;
	GLuint m_cvbo;
	GLuint m_nvbo;

	GLuint m_ebo;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec3> m_normals;
	std::vector<GLuint> m_indices;

	siv::PerlinNoise m_perlin;
	FastNoise fastNoise;

	int m_rows;
	int m_cols;

	bool m_needsUpdate = true;

	Mesh(glm::vec3 pos = { 0,0,0 }, int rows = 2, int cols = 2) {
		m_pos = pos;
		m_rows = rows;
		m_cols = cols;
		
		glGenBuffers(1, &m_pvbo);
		glGenBuffers(1, &m_cvbo);
		glGenBuffers(1, &m_nvbo);
		glGenBuffers(1, &m_ebo);
		genVertex();
		genColor({ 1,0,0 });
	}

	void genVertex() {
		m_vertices.clear();
		m_indices.clear();
		m_normals = m_vertices;

		for (int x = 0; x < m_cols; x++) {
			for (int z = 0; z < m_rows; z++) {
				//m_vertices.push_back({ x, 0.0f , y });

				auto noise = fastNoise.GetNoise(x, z);

				m_vertices.push_back({ x, noise*10 ,z });
				m_normals.push_back(glm::vec3(0));
			}
		}

		// Genererar index för indexarrayen samt normaler till alla punkter
		for (int i = 0; i < m_vertices.size(); i++) {
			if (i % m_rows > m_rows - 2) continue;
			if (i == m_rows * m_cols - m_rows - 2) break;

			m_indices.push_back(i);
			m_indices.push_back(i + m_rows);
			m_indices.push_back(i + 1);

			glm::vec3 normal = glm::normalize(glm::cross(m_vertices[i] - m_vertices[i + m_rows], m_vertices[i + 1] - m_vertices[i]));

			m_normals[i] = normal;
			m_normals[i + m_rows] = normal;
			m_normals[i + 1] = normal;

			m_indices.push_back(i + 1);
			m_indices.push_back(i + m_rows);
			m_indices.push_back(i + m_rows + 1);

			normal = glm::normalize(glm::cross(m_vertices[i + 1] - m_vertices[i + m_rows], m_vertices[i + m_rows + 1] - m_vertices[i + 1]));

			m_normals[i + 1] = normal;
			m_normals[i + m_rows] = normal;
			m_normals[i + m_rows + 1] = normal;
		}


	}

	void genColor(glm::vec3 color) {
		m_colors.clear();
		for(int i = 0; i < m_vertices.size(); i += 6){
			glm::vec3 color = { 243.0f/255.f , 154.0f / 255.f, 95.0f / 255.f };//{ ((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f };

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
			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
			glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), &m_colors[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_nvbo);
			glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

			m_needsUpdate = false;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_nvbo);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		glDrawElements(GL_TRIANGLES, m_vertices.size() * sizeof(glm::vec3), GL_UNSIGNED_INT, 0);
	}
};