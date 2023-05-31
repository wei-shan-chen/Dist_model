#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


#include "shader.h"
#include "matrixStack.h"
#include "item.h"
#include "world.h"
#include "SOM.h"
#include "camera.h"
#include "RAWmodel.h"
#include "Drawmodel.h"
#include "Gui.h"

#include <math.h>
#include <iostream>
#include <array>

#define TSIZE 64

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 2.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//light
bool blinn = false;
bool blinnKeyPressed = false;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SOM_3D", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	imgui_init(window);
	// build and compile our shader program
	// ------------------------------------
	OurShader_Create();
	int numVoxelFace = 0;
	RAWmodel rawmodel;
	// rawmodel.LoadFile("raw/tetrahedronno.inf", "raw/tetrahedronno.raw");
	rawmodel.LoadFile("raw/ball67_dist.inf", "raw/ball67_dist.raw");
	// rawmodel.LoadFile("raw/utah_teapot_dist.inf", "raw/utah_teapot_dist.raw");
	// std::cout << numVoxelFace << std::endl;
	create_world(rawmodel.bounderVoxelData, rawmodel.bounderNum,&numVoxelFace);
	// std::cout << numVoxelFace << std::endl;
	Item floor(world.square);
	Item voxelball(world.voxelBall);
	Item cube(world.cube);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		imgui_create();
		// move(keyboard&mouth)
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader_init(camera.Position, camera.Front, blinn);
		ViewProjection_Create(camera.GetViewMatrix(), camera.Zoom, SCR_WIDTH, SCR_HEIGHT);
		//draw cube
		Model_Cube_Create(cube.VAO);
		//draw floor
		Model_Floor_Create(floor.VAO);
		// draw model
		Model_Create(voxelball.VAO, numVoxelFace, (float)rawmodel.infdata.resolution[0]/2, (float)rawmodel.infdata.resolution[2]/2);

		imgui_end();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	destroy_world();
	SOM_Destroy();
	return 0;
}

// move ball collision

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		go = 1;
	if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		tmp = !tmp;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		camera.ProcessKeyboard(PITCHUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		camera.ProcessKeyboard(PITCHDOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		camera.ProcessKeyboard(YAWUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		camera.ProcessKeyboard(YAWDOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
		std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    	blinnKeyPressed = false;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}