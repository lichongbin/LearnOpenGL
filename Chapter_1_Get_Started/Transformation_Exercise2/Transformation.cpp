#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <iomanip>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT= 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

unsigned int InitTexture(const char* p, int format, bool flip)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(p, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	return texture;
}

void PrintMatrix(glm::mat4 &trans)
{
	glm::mat4 temp = glm::transpose(trans);

	for (size_t i = 0; i < 4; i++)
	{
		std::cout << std::endl;
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << std::setw(6) << temp[i][j] << ",";
		}
	}
	std::cout << std::endl;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader ourShader("vertex.txt", "fragment.txt");

	unsigned int texture1 = InitTexture("container.jpg", GL_RGB, false);
	unsigned int texture2 = InitTexture("awesomeface.png", GL_RGBA, true);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	ourShader.use();
	ourShader.setInt("ourTexture1", 0);
	ourShader.setInt("ourTexture2", 1);

	float vertices[] = {
		// positions          // colors            // texture coords
		0.5f,   0.5f, 0.0f,   1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  // top right
		0.5f,  -0.5f, 0.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  // bottom right
	   -0.5f,  -0.5f, 0.0f,   0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  // bottom left 
	   -0.5f,   0.5f, 0.0f,   1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	// Generate Vertex Array Object
	glGenVertexArrays(1, &VAO);
	// Generate Vertex Buffer Object
	glGenBuffers(1, &VBO);
	// Generate Element Buffer Object
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.5f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		glBindVertexArray(VAO);
	
		glm::mat4 trans = glm::mat4(1.0f);
		ourShader.setMat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		float scale = sin((float)glfwGetTime());
		if (scale < 0.0f)
		{
			scale = 0.0f - scale;
		}
		float y = 1.0f - 0.5f * scale;
		//trans = glm::scale(trans, glm::vec3(scale, scale, 1.0));
		//PrintMatrix(trans);
		trans = glm::translate(trans, glm::vec3(0.0f - y, y, 0.0f));
		//PrintMatrix(trans);
		trans = glm::scale(trans, glm::vec3(scale, scale, 1.0));
		//PrintMatrix(trans);

		ourShader.setMat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}