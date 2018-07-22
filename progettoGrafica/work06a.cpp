/*
OpenGL coordinate system (right-handed)
positive X axis points right
positive Y axis points up
positive Z axis points "outside" the screen


Y
|
|
|________X
/
/
/
Z
*/

#ifdef _WIN32
#define __USE_MINGW_ANSI_STDIO 0
#endif
// Std. Includes
#include <string>

// Loader for OpenGL extensions
// http://glad.dav1d.de/
// THIS IS OPTIONAL AND NOT REQUIRED, ONLY USE THIS IF YOU DON'T WANT GLAD TO INCLUDE windows.h
// GLAD will include windows.h for APIENTRY if it was not previously defined.
// Make sure you have the correct definition for APIENTRY for platforms which define _WIN32 but don't use __stdcall
#ifdef _WIN32
#define APIENTRY __stdcall
#endif

#include <glad/glad.h>

// GLFW library to create window and to manage I/O
#include <glfw\include\GLFW\glfw3.h>

// another check related to OpenGL loader
// confirm that GLAD didn't include windows.h
#ifdef _WINDOWS_
#error windows.h was included!
#endif

// classes developed during lab lectures to manage shaders, to load models, and for FPS camera
// in this example, the Model and Mesh classes support texturing
#include <utils/shader_v1.h>
#include <utils/model_v2.h>
#include <utils/camera.h>
#include <utils/plane.h>
#include <utils/physics_v1.h>

// we load the GLM classes used in the application
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// we include the library for images loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

//particle system classes
#include <utils/texture.h>
#include "particle_system.h"
#include "skymap.h"

// dimensions of application's window
GLuint screenWidth = 800, screenHeight = 600;

// callback functions for keyboard and mouse events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// if one of the WASD keys is pressed, we call the corresponding method of the Camera class
void apply_camera_movements();

// check collision at runtime each frame
bool ContactAddedCallbackBullet(btManifoldPoint &collisonPoint, const btCollisionObjectWrapper *obj1, int id1, int index1, const btCollisionObjectWrapper *obj2, int id2, int index2);

// we put the code for the models rendering in a separate function, because we will apply 2 rendering steps
void RenderObjects(Shader &shader, Model &envModel);

//put common data inside shader
void SetupShader(Shader &shader);

void ChangeShader();


// we initialize an array of booleans for each keybord key
bool keys[1024];

// we set the initial position of mouse cursor in the application window
GLfloat lastX = 400, lastY = 300;
// when rendering the first frame, we do not have a "previous state" for the mouse, so we need to manage this situation
bool firstMouse = true;

// parameters for time calculation (for animations)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// boolean to activate/deactivate wireframe rendering
GLboolean wireframe = GL_FALSE;

glm::mat4 view;

Texture *texture;

// we create a camera. We pass the initial position as a paramenter to the constructor. The last boolean tells that we want a camera "anchored" to the ground
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f), GL_FALSE);

// we use a directional light:  the vector defines the direction of incoming light
glm::vec3 lightDir0 = glm::vec3(0.0f, 1.0f, 0.0f);

// UV repetitions
GLfloat repeat = 1.0f;
//matrices
glm::mat4 projection;

// position and rotation map
glm::vec3 posMap = glm::vec3(0.0f, -30.0f, 0.0f);
glm::vec3 rotMap = glm::vec3(0.0f, 1.0f, 0.0f);

// boolean to handle show particle systems
#define RAIN_B 0
#define SNOW_B 1
std::vector<bool> particleBools;

// fog checker
bool isFogActive = false;

// instance of the physics class
Physics bulletSimulation;

//Shaders
Shader normalShader, rainShader, snowShader;
Shader *currentShader;

//Particle systems
ParticleSystem snow, rain;
float snowAmount, rainAmount;

/////////////////// MAIN function ///////////////////////
int main()
{
	// Initialization of OpenGL context using GLFW
	glfwInit();
	// We set OpenGL specifications required for this application
	// In this case: 3.3 Core
	// It is possible to raise the values, in order to use functionalities of OpenGL 4.x
	// If not supported by your graphics HW, the context will not be created and the application will close
	// N.B.) creating GLAD code to load extensions, try to take into account the specifications and any extensions you want to use,
	// in relation also to the values indicated in these GLFW commands
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// we set if the window is resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// we create the application's window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Piergigli-Quadrelli progetto", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// we put in relation the window and the callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// we disable the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD tries to load the context set by GLFW
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}


	// we define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// we enable Z test
	glEnable(GL_DEPTH_TEST);

	//the "clear" color for the frame buffer
	glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

	//setup shader
	normalShader = Shader("../progettoGrafica/normal_fog.vert", "../progettoGrafica/normal_fog.frag");
	glCheckError();
	rainShader = Shader("../progettoGrafica/wet_fog.vert", "../progettoGrafica/wet_fog.frag");
	glCheckError();
	snowShader = Shader("../progettoGrafica/snow_fog.vert", "../progettoGrafica/snow_fog.frag");
	glCheckError();
	currentShader = &normalShader;

	normalShader.Use();
	glCheckError();

	glUniform4fv(glGetUniformLocation(normalShader.Program,"particleColor"), 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glCheckError();

	texture = new Texture("../progettoGrafica/textures/maps/volcano_diff.png");
	glCheckError();

	Model envModel("../progettoGrafica/models/volcano.obj");
	Model rainDropModel("../progettoGrafica/models/raindrop.obj");
	Model snowFlakeModel("../progettoGrafica/models/snowflake.obj");


	// Projection matrix: FOV angle, aspect ratio, near and far planes
	projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);


	//Create the spawn plane for particle system
	glm::vec2 min(-130.0f, -130.0f);				//min x, min z
	glm::vec2 max(130.0f, 130.0f);				//max x, max z
	FixedYPlane rainPlane(min, max, 20.0f);	//min, maxe and y values

											//Create and setup the rain particle system
	rain = ParticleSystem(100000, &camera, &rainShader, &rainDropModel, &rainPlane);
	rain.SetRotationAndScale(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0009f, 0.0009f, 0.002f));
	rain.SetColor(glm::vec4(122 / 255.0f, 162 / 255.0f, 226 / 255.0f, 0.2f));
	rain.SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	rain.EnableParticleRotation(false);

	//Create and setup the snow particle system : NOT FINISHED, parameters are wrong!!!
	snow = ParticleSystem(10000, &camera, &snowShader, &snowFlakeModel, &rainPlane);
	snow.SetRotationAndScale(0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	snow.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.7f));
	snow.SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	snow.EnableParticleRotation(true);
	snow.SetParticleRotation(0.0f, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	//setup booleans for particle systems
	particleBools.push_back(false);	//RAIN_B
	particleBools.push_back(false);	//SNOW_B

									//setup skymap 
	SkyMap skymap(&camera, projection,
		&"../progettoGrafica/textures/sky/negz.jpg"[0],  //front 
		&"../progettoGrafica/textures/sky/posz.jpg"[0],  //back 
		&"../progettoGrafica/textures/sky/posy.jpg"[0],  //top 
		&"../progettoGrafica/textures/sky/negy.jpg"[0],  //bottom 
		&"../progettoGrafica/textures/sky/negx.jpg"[0],  //left 
		&"../progettoGrafica/textures/sky/posx.jpg"[0]  //right 
	);

	glCheckError();

	// added rigidbody map
	bulletObject* plane = bulletSimulation.createRigidBody(MAP, "../progettoGrafica/models/volcano.obj", posMap, 0.0f, rotMap, 0, 0, 0);
	// added callback to check collision
	gContactAddedCallback = ContactAddedCallbackBullet;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Rendering loop: this code is executed at each frame
	while (!glfwWindowShouldClose(window))
	{
		// we determine the time passed from the beginning
		// and we calculate time difference between current frame rendering and the previous one
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check is an I/O event is happening
		glfwPollEvents();
		// we apply FPS camera movements
		apply_camera_movements();
		// View matrix (=camera): position, view direction, camera "up" vector
		view = camera.GetViewMatrix();

		// we "clear" the frame and z buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		//setup data inside shaders
		SetupShader(rainShader);
		SetupShader(snowShader);
		SetupShader(normalShader);

		currentShader->Use();

		//update eventual particle shading effect
		if (particleBools[SNOW_B]) {
			snowAmount -= deltaTime / 20.0f ;
			if (snowAmount < -0.98f) snowAmount = -0.98f;
			glUniform1f(glGetUniformLocation(currentShader->Program, "snowLevel"), snowAmount);
			glCheckError();
		}
		if (particleBools[RAIN_B]) {
			rainAmount += deltaTime / 10.0f;
			if (rainAmount > 1.0f) rainAmount = 1.0f;
			glUniform1f(glGetUniformLocation(currentShader->Program, "wetLevel"), rainAmount);
			glCheckError();
		}

		//render map
		RenderObjects(*currentShader, envModel);

		if (particleBools[RAIN_B]) rain.Update();
		if (particleBools[SNOW_B]) snow.Update();

		//render sky
		skymap.Update();

		glfwSwapBuffers(window);

		// next check step of physic simulator
		bulletSimulation.dynamicsWorld->stepSimulation(1 / 60.0f);
	}

	normalShader.Delete();
	glCheckError();
	rainShader.Delete();
	glCheckError();
	snowShader.Delete();
	glCheckError();
	texture->Delete();
	glCheckError();
	// we delete the data of the physical simulation
	bulletSimulation.Clear();
	glCheckError();
	glfwTerminate();
	return 0;
}

void SetupShader(Shader &shader) {
	shader.Use();
	glCheckError();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glCheckError();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glCheckError();

	GLint lightDirLocation = glGetUniformLocation(shader.Program, "lightVector");
	glCheckError();
	GLint eyePosition = glGetUniformLocation(shader.Program, "eyePosition");
	glCheckError();
	GLint fog = glGetUniformLocation(shader.Program, "fogActive");
	glCheckError();
	glUniform3fv(lightDirLocation, 1, glm::value_ptr(lightDir0));
	glCheckError();
	glUniform3fv(eyePosition, 1, glm::value_ptr(camera.Position));
	glCheckError();
	if (isFogActive) {
		glUniform1i(fog, 1);
	}
	else {
		glUniform1i(fog, 0);
	}
	glCheckError();
}

void ChangeShader(){
	if (particleBools[RAIN_B]) {
		currentShader = &rainShader;
		//setup rain amount
		currentShader->Use();
		rainAmount = -0.2f;
		glUniform1f(glGetUniformLocation(currentShader->Program, "wetLevel"), rainAmount);
		glCheckError();
	}
	else if (particleBools[SNOW_B]) {
		currentShader = &snowShader;
		//setup snow amount
		currentShader->Use();
		glUniform3f(glGetUniformLocation(currentShader->Program, "snowDirection"), snow.direction.x, snow.direction.y, snow.direction.z);
		glCheckError();
		snowAmount = -0.2f;
		glUniform1f(glGetUniformLocation(currentShader->Program, "snowLevel"), snowAmount);
		glCheckError();
	}
	else currentShader = &normalShader;
}



//////////////////////////////////////////
void RenderObjects(Shader &shader, Model&envModel)
{
	GLint textureLocation = glGetUniformLocation(shader.Program, "tex");
	glCheckError();
	GLint repeatLocation = glGetUniformLocation(shader.Program, "repeat");
	glCheckError();

	glActiveTexture(GL_TEXTURE0);
	glCheckError();
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glCheckError();
	glUniform1i(textureLocation, 0);
	glCheckError();
	glUniform1f(repeatLocation, repeat);
	glCheckError();

	// Crea la matrice delle trasformazioni tramite la definizione delle 3 trasformazioni, e la matrice di trasformazione delle normali
	glm::mat4 envModelMatrix;
	glm::mat3 envNormalMatrix;
	envModelMatrix = glm::translate(envModelMatrix, posMap);
	envModelMatrix = glm::rotate(envModelMatrix, glm::radians(1.0f), rotMap);
	envModelMatrix = glm::scale(envModelMatrix, glm::vec3(0.0005f, 0.0005f, 0.0005f));

	envNormalMatrix = glm::inverseTranspose(glm::mat3(view*envModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(envModelMatrix));
	glCheckError();
	glUniformMatrix3fv(glGetUniformLocation(shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(envNormalMatrix));
	glCheckError();

	// model rendering
	envModel.Draw(shader);
}

//////////////////////////////////////////
// If one of the WASD keys is pressed, the camera is moved accordingly (the code is in utils/camera.h)
void apply_camera_movements()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (keys[GLFW_KEY_E])
		camera.ProcessKeyboard(UP, deltaTime);
}

//////////////////////////////////////////
// callback for keyboard events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// if ESC is pressed, we close the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//boost movement
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
		camera.Running = !camera.Running;
	}

	//enable/disable rain
	if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
		particleBools[RAIN_B] = !particleBools[RAIN_B];
		particleBools[SNOW_B] = false;
		ChangeShader();
	}

	//enable/disable snow
	if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
		particleBools[RAIN_B] = false;
		particleBools[SNOW_B] = !particleBools[SNOW_B];
		ChangeShader();
	}

	//enable/disable fog
	if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
		isFogActive = !isFogActive;
	}

	// we keep trace of the pressed keys
	// with this method, we can manage 2 keys pressed at the same time:
	// many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
	// using a boolean array, we can then check and manage all the keys pressed at the same time
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

//////////////////////////////////////////
// callback for mouse events
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// we move the camera view following the mouse cursor
	// we calculate the offset of the mouse cursor from the position in the last frame
	// when rendering the first frame, we do not have a "previous state" for the mouse, so we set the previous state equal to the initial values (thus, the offset will be = 0)
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// offset of mouse cursor position
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	// the new position will be the previous one for the next frame
	lastX = xpos;
	lastY = ypos;

	// we pass the offset to the Camera class instance in order to update the rendering
	camera.ProcessMouseMovement(xoffset, yoffset);

}

// check collision at runtime each frame
bool ContactAddedCallbackBullet(btManifoldPoint &collisonPoint, const btCollisionObjectWrapper *obj1, int id1, int index1, const btCollisionObjectWrapper *obj2, int id2, int index2) {
	bulletObject* firstObj = (bulletObject*)obj1->getCollisionObject()->getUserPointer();
	bulletObject* secondObj = (bulletObject*)obj2->getCollisionObject()->getUserPointer();
	
	//cout << "Collision " << firstObj->type << " with " << secondObj->type << endl;

	// check collision and set the hit with the map
	if ((firstObj->type != secondObj->type) && firstObj->type == PARTICLE) {
		firstObj->hit = true;
	}
	else if ((firstObj->type != secondObj->type) && secondObj->type == PARTICLE) {
		secondObj->hit = true;
	}
	return false;
}