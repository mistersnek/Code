// Code adapted from www.learnopengl.com, www.glfw.org
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
//texture lib
#include<stb/stb_image.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void do_movement();

// Window dimensions
const GLuint WIDTH = 640, HEIGHT = 640;

// Camera

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							// Light attributes
glm::vec3 lightPos(1.0f, 0.25f, 1.0f);

							// The MAIN function, from here we start the application and run the game loop

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current

								   // Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

								   //++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, HEIGHT, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Build and compile our shader program
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	GLuint lampshader = initShader("lampvert.glsl", "lampfrag.glsl");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat light[] = {
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, 1.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f, 1.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, 1.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, 0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, 0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, 0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, 0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f, 1.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLfloat pyramid[]{

		0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f,  -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		-0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		-0.8f, 0.5f, 0.0f,
		-0.5f, 0.0f, -0.5f,		 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,		-0.8f, 0.5f, 0.0f,
		0.0f, 0.8f,  0.0f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,		-0.8f, 0.5f, 0.0f,

		-0.5f, 0.0f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.5f, -0.8f,
		0.5f, 0.0f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.5f, -0.8f,
		0.0f, 0.8f,  0.0f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.5f, -0.8f,

		0.5f, 0.0f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.8f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.8f, 0.5f, 0.0f,
		0.0f, 0.8f,  0.0f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,		0.8f, 0.5f, 0.0f,

		0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.5f, 0.8f,
		-0.5f, 0.0f, 0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.5f, 0.8f,
		0.0f, 0.8f,  0.0f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.5f, 0.8f,
	};

	/*GLuint indices[]{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		7, 9, 8,
		10, 12, 11,
		13, 15, 14
	};*/

	// First, set the container's VAO (and VBO)
	GLuint VBOs[3], containerVAO[3];
	
	glGenVertexArrays(3, containerVAO);
	glGenBuffers(3, VBOs);

	/*GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,	GL_STATIC_DRAW);*/

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

	
	glBindVertexArray(containerVAO[0]);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);					
	// Normal attribute								
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);					
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light), light, GL_STATIC_DRAW);

	// Then, we set the light's VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//Texture
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("brick_texture.jpg", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//makes the image blurry instead of pixelated (for pixelated need to use GL_NEAREST in the last parameter)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//make image repeat in both S and T axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//use rbga for png's and rgb for jpg's
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram, "tex0");
	glUseProgram(shaderProgram);
	glUniform1i(tex0Uni, 0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Use corresponding shader when setting uniforms/drawing objects
		glUseProgram(shaderProgram);
		GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
		//glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		

		// Create camera transformations
		glm::mat4 view;
		glm::mat4 projection;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		// Draw the container (using container's vertex attributes)
		glBindVertexArray(containerVAO[0]);
		glm::mat4 model;
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		//if i need to move the piece somewhere else
		glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, pyramidPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		glUseProgram(lampshader);
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampshader, "model");
		viewLoc = glGetUniformLocation(lampshader, "view");
		projLoc = glGetUniformLocation(lampshader, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteTextures(1, &texture);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}



// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	GLfloat sensitivity = (GLfloat) 0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}