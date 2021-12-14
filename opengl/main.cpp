// Code adapted from www.learnopengl.com, www.glfw.org

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(640, 640, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current
	
	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, 640, 640);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom Left  
		0.5f, -0.5f, 0.0f, // Right 
		-0.5f,  0.5f, 0.0f,  // top left  

		-0.5f,  0.5f, 0.0f, // top left  
		0.5f, 0.5f, 0.0f, // top right 
		0.5f, -0.5f, 0.0f,  // bottom 
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

    // ================================
    // buffer setup
    // ===============================
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl","frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the first Triangle
		// use shader
		glUseProgram(shaderProgram);

		//+++++++ Define transform matrix (uncomment the transform matrix you want to create)+++++++
		glm::mat4 transform;
	
		/* Translation */
		//transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f));

		/* Rotation */
		//transform = glm::rotate(transform, (GLfloat)(3.14), glm::vec3(0.0f, 0.0f, 1.0f));

		/* Scaling */
		float modelScale = 1.0f;

		while (modelScale > 0.5f) {
			transform = glm::scale(transform, glm::vec3(modelScale, modelScale, 0.0f));
			modelScale = modelScale - 0.1f;
		}








		/* animation example: triangle rotation */

		// rotate 360 degrees and stop
		if ((GLfloat)glfwGetTime() * 0.5f <= 6.28)
		transform = glm::rotate(transform, (GLfloat)(glfwGetTime() * -0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		//add this for going 360 counter clockwise after going 360 clockwise
		//else
		//	transform = glm::rotate(transform, (GLfloat)(glfwGetTime() * 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//for rotating the square around the origin (0,0) use rotation and translation
		//transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));


		/* animation example: triangle translation */

		/*if ((GLfloat)glfwGetTime() * 0.5f <= 0.5f) {
			transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.5f, 0.0f, 0.0f));

			//used for going diagonally
			//transform = glm::translate(transform, glm::vec3(0.0f,(GLfloat)glfwGetTime() * 0.5f, 0.0f));
		} 
		else 
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));*/

		/* animation example:  */
		//GLfloat dx = (GLfloat)(glm::abs(glm::sin(glfwGetTime())) * 0.5);
		//transform = glm::translate(transform, glm::vec3(dx, 0.0f, 0.0f));
		//add this for moving diagonally
		//transform = glm::translate(transform, glm::vec3(0.0f, dx, 0.0f));
		

		// Get matrix's uniform location and set matrix
		GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// draw object
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindVertexArray(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}