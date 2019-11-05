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

#include "quad.hpp"
#include <block.hpp>
#include <mesh.hpp>

#define HEIGHT 20
#define WIDTH 20
#define DEPTH 20

enum BLOCK_TYPES {
	EARTH,
	WATER
};

GLuint shaderProgram;

std::vector<std::vector<std::vector<block*>>> blocks;

float mpos_x = 0.0;
float mpos_y = 0.0;
int W = 1080;
int H = 1080;

float moveSpeed = 1.0f;

glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)W / (float)H, 0.1f, 1000.f);
glm::vec3 worldPosition = glm::vec3({-50,-10,-50});
glm::mat4 fullTransformMatrix;

glm::vec3 lightPos = {10,10,0};
GLint fullTransformMatrixUniformLocation;
GLint lightPosLocation;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Next mouse Y position
	float future_mpos_y = (mpos_y + (ypos - H / 2.0f));

	if (!(future_mpos_y/H < -0.5f || future_mpos_y/H > 0.5f)) {
		mpos_y += ypos - H / 2.0f;
	}

	mpos_x += xpos - W / 2.0f;

	glfwSetCursorPos(window, W / 2, H / 2);

	std::cout << "X: " << xpos << " Y: " << ypos << std::endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float rand_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	float rand_rot_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	glm::vec3 lookVector = glm::normalize(glm::vec3(
		{
			fullTransformMatrix[0][2],
			fullTransformMatrix[1][2],
			fullTransformMatrix[2][2]
		}
	));
	glm::vec3 strafeVector = lookVector;

	switch (key) {
	case GLFW_KEY_1: {

	}
					 break;
	case GLFW_KEY_2: {

	}
					 break;
	case GLFW_KEY_LEFT:

		break;
	case GLFW_KEY_W:
		worldPosition = worldPosition - moveSpeed * lookVector;
		break;
	case GLFW_KEY_A:
		strafeVector.y = 0.0f;

		strafeVector = glm::normalize(glm::cross(strafeVector, { 0, 1, 0 }));

		worldPosition = worldPosition + moveSpeed * strafeVector;
		break;
	case GLFW_KEY_S:
		worldPosition = worldPosition + moveSpeed * lookVector;
		break;
	case GLFW_KEY_D:
		strafeVector.y = 0.0f;

		strafeVector = - glm::normalize(glm::cross(strafeVector, { 0, 1, 0 }));

		worldPosition = worldPosition + moveSpeed * strafeVector;
		break;
	case GLFW_KEY_SPACE:

		break;
	default:
		break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void updatePerspective() {
	float verticalAngle = (float)((mpos_y / H) * 3.1415);
	float horizontalAngle = (float)((mpos_x / W) * 3.1415);

	if (verticalAngle > 3.1415 / 2.0f) {
		verticalAngle = 3.1415 / 2.0f;
	}

	glm::mat4 projectionTranslationMatrix = glm::rotate(projectionMatrix, verticalAngle, glm::vec3(1, 0, 0));
	projectionTranslationMatrix = glm::rotate(projectionTranslationMatrix, horizontalAngle, glm::vec3(0, 1, 0));

	fullTransformMatrix = glm::translate(projectionTranslationMatrix, worldPosition);

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniform3fv(lightPosLocation, sizeof(lightPos), (float*)&lightPos);
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
	glfwSetKeyCallback(window, keyCallback);

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
	int cols = 126;

	// Set up sizes. (HEIGHT x WIDTH)
	blocks.resize(HEIGHT);
	for (int i = 0; i < HEIGHT; ++i) {
		blocks[i].resize(WIDTH);

		for (int j = 0; j < WIDTH; ++j)
			blocks[i][j].resize(DEPTH);
	}

	for (int row = 0; row < blocks.size(); row++) {
		for (int col = 0; col < blocks[row].size(); col++) {
			for (int dep = 0; dep < blocks[row][col].size(); dep++) {
				blocks[row][col][dep] = new block({ row, col, dep });
				blocks[row][col][dep]->genColor({ ((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f,((float)rand() / (RAND_MAX)) - 0.5f });
			}
		}
	}

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"uniform mat4 fullTransformMatrix;"
		"out vec3 color;"
		"void main() {"
		"	gl_Position = fullTransformMatrix * vec4(vertex_position, 1.0);"
		"	color = vertex_color;"
		"};";

	const char* fragment_shader =
		"#version 400\n"
		"in vec3 color;"
		"out vec4 fragColor;"
		"void main() {"
		"	fragColor = vec4(color, 1.0);"
		"};";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// insert location binding code here
	glBindAttribLocation(shaderProgram, 0, "vertex_position");
	glBindAttribLocation(shaderProgram, 1, "vertex_color");
	glBindAttribLocation(shaderProgram, 2, "vertex_normal");

	glLinkProgram(shaderProgram);
	fullTransformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "fullTransformMatrix");
	lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");

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

	int timeStepMs = 2;

	Mesh mesh({0,0,0}, 1000, 1000);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset the matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//printf("Loop: %d\n", loops);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		mesh.show();

		updatePerspective();

		glfwWaitEvents();
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