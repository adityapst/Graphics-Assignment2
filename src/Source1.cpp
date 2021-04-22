#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Teapot.h"
#include "values.h"
#include"Camera.h"

using namespace std;
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
/// <summary>
/// x co-ordinate of the centre of the screen
/// </summary>
float lastX = SCR_WIDTH / 2.0f;
/// <summary>
/// y co-ordinate of the centre of the screen
/// </summary>
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"

"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
"   gl_Position =MVP*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"


"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
" out vec4 color;\n"
" uniform vec3 icolor;\n"



"void main()\n"
"{\n"
"	color =vec4(icolor,1);"
"}\0";

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(LROLL, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(RROLL, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




// glfw: whenever the mouse scroll wheel scrolls, this callback is called



int main()
{
	Teapot teapot(0, 1, 0);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(float), Points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.vertices.size() * sizeof(float), teapot.vertices.data(), GL_STATIC_DRAW);

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);


	unsigned int IBO;  //Index buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  teapot.indices.size() * sizeof(unsigned int),teapot.indices.data(), GL_STATIC_DRAW);


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();


	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	int MVPLoc = glGetUniformLocation(shaderProgram, "MVP");
	int icolorLoc = glGetUniformLocation(shaderProgram, "icolor");


	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);



	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	//  glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
	 // glm::vec3 worldUp = glm::vec3(0.0,1.0,0.0);
	 // glm::vec3 cameraPos = glm::vec3(0.0, 2.0, 12.0);
	//  glm::vec3 cameraRight = glm::cross(glm::vec3(0.0, 0.0, -1.0), worldUp);
	//  glm::vec3 cameraUp = glm::cross(cameraRight,cameraFront);

	  //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 view = camera.GetViewMatrix();


	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.7, 0.7, 0.7, 1.0);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		proj = glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		float displaceX = 1.5;
		float displaceZ = 0.881;
		// cube->bind();
		float tableWidth = 2.0;
		float tableLength = 3.236;
		float tableThickness = 0.122;
		float tableLegLength = 1.0;
		float restHeight = 0.0;

		glm::mat4 tableTop;
		glm::mat4 tableLeg[4];
		glm::mat4 backRest;
		glUniform3f(icolorLoc, 0.4, 0.2, 0.1);


		tableTop = glm::mat4(1.0f); tableTop = glm::translate(tableTop, glm::vec3(0.0, tableLegLength, 0.0)); tableTop = glm::scale(tableTop, glm::vec3(tableLength, tableThickness, tableWidth));
		tableLeg[0] = glm::mat4(1.0f); tableLeg[0] = glm::translate(tableLeg[0], glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength / 2, -(tableWidth / 2 - tableThickness / 2))); tableLeg[0] = glm::scale(tableLeg[0], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[1] = glm::mat4(1.0f); tableLeg[1] = glm::translate(tableLeg[1], glm::vec3(-(tableLength / 2 - tableThickness / 2), tableLegLength / 2, -(tableWidth / 2 - tableThickness / 2))); tableLeg[1] = glm::scale(tableLeg[1], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[2] = glm::mat4(1.0f); tableLeg[2] = glm::translate(tableLeg[2], glm::vec3(-(tableLength / 2 - tableThickness / 2), tableLegLength / 2, tableWidth / 2 - tableThickness / 2)); tableLeg[2] = glm::scale(tableLeg[2], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[3] = glm::mat4(1.0f); tableLeg[3] = glm::translate(tableLeg[3], glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength / 2, tableWidth / 2 - tableThickness / 2)); tableLeg[3] = glm::scale(tableLeg[3], glm::vec3(tableThickness, tableLegLength, tableThickness));
		backRest = glm::mat4(1.0f); backRest = glm::translate(backRest, glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength + restHeight / 2, 0));; backRest = glm::scale(backRest, glm::vec3(tableThickness, restHeight, tableWidth));
		
		  //DINING TABLE
		glBindVertexArray(VAO1);

		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* tableTop));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* tableLeg[0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* tableLeg[1]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* tableLeg[2]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* tableLeg[3]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* backRest));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		tableWidth = 1.0;
		tableLength = 1.0;
		tableThickness = 0.122;
		tableLegLength = 0.7;
		restHeight = 1.0;

		tableTop = glm::mat4(1.0f); tableTop = glm::translate(tableTop, glm::vec3(0.0, tableLegLength, 0.0)); tableTop = glm::scale(tableTop, glm::vec3(tableLength, tableThickness, tableWidth));
		tableLeg[0] = glm::mat4(1.0f); tableLeg[0] = glm::translate(tableLeg[0], glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength / 2, -(tableWidth / 2 - tableThickness / 2))); tableLeg[0] = glm::scale(tableLeg[0], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[1] = glm::mat4(1.0f); tableLeg[1] = glm::translate(tableLeg[1], glm::vec3(-(tableLength / 2 - tableThickness / 2), tableLegLength / 2, -(tableWidth / 2 - tableThickness / 2))); tableLeg[1] = glm::scale(tableLeg[1], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[2] = glm::mat4(1.0f); tableLeg[2] = glm::translate(tableLeg[2], glm::vec3(-(tableLength / 2 - tableThickness / 2), tableLegLength / 2, tableWidth / 2 - tableThickness / 2)); tableLeg[2] = glm::scale(tableLeg[2], glm::vec3(tableThickness, tableLegLength, tableThickness));
		tableLeg[3] = glm::mat4(1.0f); tableLeg[3] = glm::translate(tableLeg[3], glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength / 2, tableWidth / 2 - tableThickness / 2)); tableLeg[3] = glm::scale(tableLeg[3], glm::vec3(tableThickness, tableLegLength, tableThickness));
		backRest = glm::mat4(1.0f); backRest = glm::translate(backRest, glm::vec3(tableLength / 2 - tableThickness / 2, tableLegLength + restHeight / 2, 0));; backRest = glm::scale(backRest, glm::vec3(tableThickness, restHeight, tableWidth));

		glm::mat4 displace = glm::translate(glm::mat4(1.0f), glm::vec3(displaceX, 0, 0));
		displace = glm::rotate(displace, glm::radians(0.0f), glm::vec3(0, 1, 0));
		glUniform3f(icolorLoc, 0.4, 0.1, 0.0);
		//DRAWING CHAIR 1

		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableTop));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[0]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[1]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[2]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[3]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* backRest));
		(glDrawArrays(GL_TRIANGLES, 0, 36));

		displace = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -displaceZ));
		displace = glm::rotate(displace, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniform3f(icolorLoc, 0.5, 0.0, 0.1);

		//DRAWING CHAIR 2

		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableTop));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[0]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[1]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[2]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[3]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* backRest));
		(glDrawArrays(GL_TRIANGLES, 0, 36));

		displace = glm::translate(glm::mat4(1.0f), glm::vec3(-displaceX, 0, 0));
		displace = glm::rotate(displace, glm::radians(180.0f), glm::vec3(0, 1, 0));
		glUniform3f(icolorLoc, 0.9, 1, 0.1);

		//    DRAWING CHAIR 3
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableTop));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[0]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[1]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[2]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[3]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* backRest));
		(glDrawArrays(GL_TRIANGLES, 0, 36));

		displace = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, displaceZ));
		displace = glm::rotate(displace, glm::radians(270.0f), glm::vec3(0, 1, 0));
		glUniform3f(icolorLoc, 0.3, 0.0, 0.8);

		//DRAWING CHAIR 4
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableTop));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[0]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[1]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[2]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* tableLeg[3]));
		(glDrawArrays(GL_TRIANGLES, 0, 36));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(proj* view* displace* backRest));
		(glDrawArrays(GL_TRIANGLES, 0, 36));

		//Teapot
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glm::mat4 MVP = proj * view * glm::translate(glm::mat4(1.0f),glm::vec3(0,1.255f,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02f));
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);
		glUniform3f(icolorLoc, 1.0f, 0.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, teapot.numberOfVertices,GL_UNSIGNED_INT,nullptr);
		



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

