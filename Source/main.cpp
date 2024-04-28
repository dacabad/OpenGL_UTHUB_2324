#include <iostream>
#include <unordered_set>
#include <vector>

#include "Cube.h"
#include "glad/glad.h"
#include "glfw3.h"
#include "Shader.h"
#include "glm.hpp"
#include "ext.hpp"
#include "../Model.h"


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

constexpr float WINDOW_WIDTH		= 800;
constexpr float WINDOW_HEIGHT		= 800;

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

void SpawnModel(Transform inTransform);

int main()
{
	uint8_t err;
	GLFWwindow* window = initOpenGL(err);

	Model model("Contents/shrek_reduced.obj");
	model.transform_.location.x = 0.f;
	model.transform_.location.y = 0.f;
	model.transform_.location.z = 0.f;

	Model anothermodel("Contents/teapot.obj");
	anothermodel.transform_.location.x = 0.f;
	anothermodel.transform_.location.y = 0.f;
	anothermodel.transform_.location.z = -10.f;

	std::vector<Model> modelArray;
	modelArray.push_back(model);
	//modelArray.push_back(anothermodel);


	// SETUP
	unsigned int VBO;
	unsigned int VAO;
	unsigned int IBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), model.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(int), model.indices.data(), GL_STATIC_DRAW);

	void* offset = nullptr;
	unsigned int stride = 3 * sizeof(float);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader vertex("Shaders/vertexShader.glsl", GL_VERTEX_SHADER);
	Shader fragment("Shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);



	ShaderProgram shaderProgram(vertex, fragment);

	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);

		glBindVertexArray(VAO);

		shaderProgram.useProgram();

		GLint colorUniformId = glGetUniformLocation(shaderProgram.getShaderId(), "polyColor");
		glUniform4f(colorUniformId, 0xAA, 0xCC, 0x00, 1.f);

		for(const auto& localRenderModel : modelArray)
		{
			glm::mat4 modelMat(1.f);
			modelMat = glm::translate(modelMat, localRenderModel.transform_.location);
			//modelMat = glm::rotate(modelMat, glm::radians((float)glfwGetTime() * 30.f), worldVectors::forwardVector);
			modelMat = glm::rotate(modelMat, glm::radians((float)glfwGetTime() * 30.f), worldVectors::rightVector);
			//modelMat = glm::rotate(modelMat, glm::radians((float)glfwGetTime() * 30.f), worldVectors::upVector);
			//modelMat = glm::scale(modelMat, glm::vec3(3 * glm::cos(glfwGetTime())));

			glm::mat4 view(1.f);
			//view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f * (glm::cos(glfwGetTime()) + 1) * 0.5));
			view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));

			glm::mat4 perspective(1.f);
			perspective = glm::perspective(glm::radians(45.f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.f);

			glm::mat4 mvp = perspective * view * modelMat;

			GLint mvpUniformLocation = glGetUniformLocation(shaderProgram.getShaderId(), "mvp");
			glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));

			glDrawElements(GL_TRIANGLES, localRenderModel.indices.size(), GL_UNSIGNED_INT, (void*)0);
		}
	}
}