#include <iostream>
#include <vector>

#include "Cube.h"
#include "glad/glad.h"
#include "glfw3.h"
#include "Shader.h"
#include "glm.hpp"
#include "ext.hpp"



#define ERRMSG_SUCCESS					0b00000000
#define ERRMSG_FAILED_LOAD_WINDOW		0b00000100
#define ERRMSG_FAILED_LOAD_GLAD			0b00000001

//{
//	ERRMSG_FAILED_LOAD_GLAD | ERRMSG_FAILED_LOAD_WINDOW
//
//	0b00000100
//	0b00000001
//
//	0b00000101
//}

constexpr int WINDOW_WIDTH		= 800;
constexpr int WINDOW_HEIGHT		= 800;

//const float vertices[] = 
//{
//	-0.5f,	 -0.5f,		0.f, 1.f, 1.f, 1.f,// First triangle
//	0.5f,	 0.5f,		0.f, 1.f, 1.f, 1.f,//
//	0.5f,	 -0.5f,		0.f, 1.f, 1.f, 1.f,//
//							 
//	-0.5f,	 -0.5f,		0.f, 1.f, 1.f, 1.f,// Second triangle
//	0.5f,	 0.5f,		0.f, 1.f, 1.f, 1.f,//
//	-0.5f,	 0.5f,		0.f, 1.f, 1.f, 1.f,//
//};

const float vertices[] = {
    0.5f,  0.5f, 0.0f, 1.f, 1.f, 1.f,
    0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
   -0.5f, -0.5f, 0.0f, 1.f, 0.f, 1.f,
   -0.5f,  0.5f, 0.0f, 1.f, 0.f, 0.f 
};

const unsigned int indices[] = {
    0, 1, 3,	// tri 1
    1, 2, 3		// tri 2
    //0, 2, 3		// tri 2
};

namespace worldVectors
{
	glm::vec3 upVector		(0.f, 0.f, 1.f);
	glm::vec3 rightVector	(0.f, 1.f, 0.f);
	glm::vec3 forwardVector	(1.f, 0.f, 0.f);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* initOpenGL(uint8_t& err_level)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	err_level = ERRMSG_SUCCESS;

	// Window setup
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Learning", nullptr, nullptr);
	if(!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		err_level = ERRMSG_FAILED_LOAD_WINDOW;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		err_level = ERRMSG_FAILED_LOAD_GLAD;
	}

	return window;
}


int main()
{
	uint8_t err;
	GLFWwindow* window = initOpenGL(err);

	// SETUP
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // estoy configurando que las órdenes que vienen tienen que ver con este VBO
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::cube), Shapes::cube, GL_STATIC_DRAW);

	/*unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)0);

	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)(3 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	Shader vertex("Shaders/vertexShader.glsl", GL_VERTEX_SHADER);
	Shader fragment("Shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgram(vertex, fragment);

	Shader fragmentRed("Shaders/fragmentShaderRed.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram shaderProgramRed(vertex, fragmentRed);


	std::vector<glm::vec3> objectLocations;
	objectLocations.push_back(glm::vec3{0.f, 0.f, 0.f});
	objectLocations.push_back(glm::vec3{1.f, 0.f, 1.f});
	objectLocations.push_back(glm::vec3{-0.5f, -0.5f, -1.f});

	// Render loop
	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);

		for(auto location : objectLocations)
		{
			shaderProgram.useProgram();

			GLint colorUniformId = glGetUniformLocation(shaderProgram.getShaderId(), "polyColor");

			glUniform4f(colorUniformId, 1.f, 1.f, 1.f, 1.f);

			glm::mat4 transformMat(1.f);
			transformMat = glm::rotate(transformMat, glm::radians((float)glfwGetTime() * 30.f), worldVectors::forwardVector);
			transformMat = glm::rotate(transformMat, glm::radians((float)glfwGetTime() * 30.f), worldVectors::upVector);
			transformMat = glm::scale(transformMat, glm::vec3(0.5f));
			//transformMat = glm::translate(transformMat, glm::vec3(-0.2f, 0.3f, 0.f));
			transformMat = glm::translate(transformMat, location);

			GLint transformUniformLocation = glGetUniformLocation(shaderProgram.getShaderId(), "transformMatrix");
			glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, std::size(Shapes::cube));

			glUniform4f(colorUniformId, 1.f, 0.f, 1.f, 1.f);
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * 3));
			//glDrawArrays(GL_TRIANGLES, 3, 3);
		}
	}
}