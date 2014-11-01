#include "internal.h"

static Fade2D *library;

///
/// Constructor for Fade2D
///
/// @param resX Diagonal resolution of the desired window
/// @param resY Vertical resolution of the desired window
/// @param name Name of the desired window
///
Fade2D::Fade2D(int resX, int resY, char* name){
	//Start GL context and open windows
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(resX, resY, name, NULL, NULL);
	//this->maxEntity = new int[maxEntity];
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	ShaderHandler::addProgram();
	ShaderHandler::bindProgram(ShaderHandler::addShader(ShaderHandler::vertex, "..\\Fade2D\\vertexShader.txt"), ShaderHandler::addShader(ShaderHandler::fragment, "..\\Fade2D\\fragmentShader.txt"));
	ShaderHandler::useProgram();

	glUniformMatrix4fv(
		glGetUniformLocation(ShaderHandler::getProgram(), "proj"),
		1,
		GL_FALSE,
		//glm::value_ptr(glm::translate(glm::ortho(0.f, 800.0f, 600.0f, 0.f, 10.0f, -10.0f), glm::vec3(-1.f, -1.f, 0.f))));
		glm::value_ptr(glm::ortho(0.f, (float)resX, (float)resY, 0.f, 10.0f, -10.0f)));

	this->genBaseObject();

	library = this;
}

///
/// Checks if the window should close
///
bool Fade2D::windowShouldClose()
{
	glfwPollEvents();
	return !glfwWindowShouldClose(window);
}

///
/// Swaps back and front buffers, must be done every scene after rendering
///
void Fade2D::swapBuffer(){
	glfwSwapBuffers(window);
}

///
/// Prepares the scene for rendering using default color
///
void Fade2D::prepareScene(){
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///
/// Prepares scene for rendering using a custom color
///
void Fade2D::prepareScene(float R, float G, float B){
	glClearColor(R, G, B, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Fade2D::draw(){

	glBindVertexArray(base_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Fade2D::genBaseObject(){
	glGenBuffers(1, &base_vbo);
	glGenVertexArrays(1, &base_vao);
	glBindVertexArray(base_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, base_vbo);
	GLfloat verteces[] = {
		0.f, 1.f, 0.f,
		0.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

/// Interface for creating a Fade2D object
///
/// @param resX Horizontal resolution
/// @param resY Vertical resolution
/// @param name Name of the window
///
IFade2D* new_IFade2d(int resX, int resY, char* name){
	return new Fade2D(resX, resY, name);
}

/// Interface for creating an object of Entity
///
/// @param xLen Lenght of the Entity
/// @param yLen Height of the Entity
/// @param xPos Horizontal position of the Entity
/// @param yPos Vertical position of the Entiy
///
IEntity* new_IEntity(float xLen, float yLen, float xPos, float yPos){
	return new Entity(xLen, yLen, xPos, yPos, library);
}