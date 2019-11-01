#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

GLuint shaderProgram;
class cube;

std::vector<cube> cubes;

class phys {
public:
	glm::vec3 m_p;
	glm::vec3 m_v;
	glm::vec3 m_a;

	float m_roll;
	float m_pitch;
	float m_yaw;

	phys() {};
};

class quad {
public:
	glm::vec3 m_pos;
	GLuint m_pvbo;
	GLuint m_cvbo;
	glm::vec3 m_points[4];
	glm::vec3 m_colors[4];

	quad() {
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

class cube : phys {
private:
	glm::vec3 m_points[36];
	glm::vec3 m_colours[36];

	GLuint m_pvbo;
	GLuint m_cvbo;

	static int nrOfCubes;
	int m_id;
public:
	cube(glm::vec3 pos) {
		m_pvbo = m_id;
		m_cvbo = m_id;

		glGenBuffers(1, &m_pvbo);
		glGenBuffers(1, &m_cvbo);

		m_p = pos;
		m_id = nrOfCubes++;

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
			a = a * 0.2f;
		}

		/*Predefined colors of cube*/
		glm::vec3 colours[36] = {
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
			a = a + m_p;
		}

		memcpy(m_points, points, sizeof(points));
		memcpy(m_colours, colours, sizeof(colours));
	}

	int getNumberOfPoints() {
		return sizeof(m_points) / sizeof(float);
	}

	static int getNumberOfCubes() {
		return nrOfCubes;
	}

	void setPos(float x, float y, float z) {
		m_p[0] = x;
		m_p[1] = y;
		m_p[2] = z;
	}

	void show() {
		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colours), m_colours, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, getNumberOfPoints() / 3);
	}
};


int cube::nrOfCubes = 0;
float mpos_x = 0.0;
float mpos_y = 0.0;
int W = 1080;
int H = 1080;

glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
glm::vec3 worldPosition = glm::vec3(0);

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mpos_x = xpos;
	mpos_y = ypos;

	std::cout << "X: " << xpos << " Y: " << ypos << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float rand_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	float rand_rot_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	switch (key) {
	case GLFW_KEY_1: {
		cube tmp_cube({ rand_x, rand_y , rand_z });

		cubes.push_back(tmp_cube);
	}
					 break;
	case GLFW_KEY_2: {
		cube tmp_cube({ (mpos_x / (float)W) * 2.0f - 1.0f, 1.0f - (mpos_y / (float)H) * 2.0f, 0.0f });

		cubes.push_back(tmp_cube);
	}
					 break;
	case GLFW_KEY_LEFT:

		break;
	case GLFW_KEY_W:
		worldPosition = worldPosition + glm::vec3({0, 0, 0.2f});
		break;
	case GLFW_KEY_SPACE:
		cubes.clear();
		break;
	default:
		break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void updatePerspective() {
	glm::mat4 projectionTranslationMatrix = glm::rotate(projectionMatrix, (float)((mpos_y / H) * 3.1415*2 - 3.1415), glm::vec3(1, 0, 0));
	projectionTranslationMatrix = glm::rotate(projectionTranslationMatrix, (float)((mpos_x / W) * 3.1415*2 - 3.1415), glm::vec3(0, 1, 0));

	glm::mat4 fullTransformMatrix = glm::translate(projectionTranslationMatrix, worldPosition);

	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "fullTransformMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
}

int main() {
	srand(time(NULL));

	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(W, H, "Cube test", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	//glOrtho(0, 1920, 0, 1080, -1, 1);

	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, W, 0, H, -1, 1);
	//glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 1);	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	//Sets the key callback
	glfwSetKeyCallback(window, key_callback);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	//

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	int row = 0;
	int col = 0;
	int cols = 6;

	std::vector<quad> quads;

	for (int i = 0; i < 2000; i++) {
		quad newQuad;
		if (i % cols == 0) {
			row++;
		};

		newQuad.m_pos = { (i % cols) * 1.1 - cols/2, -3, -row*1.1 };
		newQuad.genVertex(1);
		newQuad.genColor({ ((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f });

		quads.push_back(newQuad);
	}

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_colour;"
		"uniform mat4 fullTransformMatrix;"
		"out vec3 colour;"
		"void main() {"
		"	colour = vertex_colour;"
		"	gl_Position = fullTransformMatrix * vec4(vertex_position, 1.0);"
		"};";

	const char* fragment_shader =
		"#version 400\n"
		"in vec3 colour;"
		"out vec4 frag_colour;"
		"void main() {"
		"	frag_colour = vec4(colour, 1.0);"
		"};";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fs);
	glAttachShader(shaderProgram, vs);

	// insert location binding code here
	glBindAttribLocation(shaderProgram, 0, "vertex_position");
	glBindAttribLocation(shaderProgram, 1, "vertex_colour");

	glLinkProgram(shaderProgram);

	//

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	int loops = 0;

	int timeStepMs = 10;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset the matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//printf("Loop: %d\n", loops);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);


		for (quad& a : quads) {
			a.show();
		}

		updatePerspective();

		glfwPollEvents();
		glfwSwapBuffers(window);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		loops++;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
	}

	/* OTHER STUFF GOES HERE NEXT */

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
