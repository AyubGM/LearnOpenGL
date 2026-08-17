
#include "Common/Window.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"


#include <iostream>
#include "Examples/FrameBufferExample.h"
#include "Examples/CubeMapExample.h"
#include "Examples/UniformBufferExample.h"
#include "Examples/GeometryShaderExample.h"
#include "Examples/InstancingExample.h"
#include "Examples/AntiAliasingExample.h"
#include "Examples/AdvancedLightingExample.h"
#include "Examples/ShadowMappingExample.h"
#include "Examples/PointShadowsExample.h"
#include "Examples/NormalMappingExample.h"
#include <Examples/ParallaxMappingExample.h>
#include <Examples/HDRExample.h>
#include <Examples/BloomExample.h>
#include <Examples/DeferredShadingExample.h>
#include <Examples/SsaoExample.h>
#include <Examples/PBRExample.h>
#include <Examples/IBLExample.h>

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void DeltaTime(Window& window, Camera& camera)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    window.ProcessInput(deltaTime);
	camera.Update(deltaTime);
}

int main()
{
    Window window;
    Camera camera;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);  

    glm::mat4 g_Projection = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 1000.0f);
    glm::mat4 g_View = camera.GetViewMatrix();

    //FrameBufferExample frameBufferExample(window.GetWidth(), window.GetHeight());
    //CubeMapExample cubeMapExample;
    //UniformBufferExample UniformBufferExample;
    //GeometryShaderExample geometryShaderExample;
    //InstancingExample instancingExample;
    //AntiAliasingExample antiAliasingExample(window.GetWidthRef(), window.GetHeightRef());
    //AdvancedLightingExample advancedLightingExample(camera);
    //ShadowMappingExample shadowMappingExample(window.GetWidthRef(), window.GetHeightRef());
    //PointShadowsExample pointShadowsExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //NormalMappingExample normalMappingExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //ParallaxMappingExample parallaxMappingExample(camera);
	//HDRExample hRExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //BloomExample bloomExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //DeferredShadingExample deferredShadingExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //SsaoExample ssaoExample(window.GetWidthRef(), window.GetHeightRef(), camera);
    //PBRExample pBRExample(camera);
    IBLExample iBLExample(window.GetWidthRef(), window.GetHeightRef(), camera);

    while(!glfwWindowShouldClose(window.GetGLFWwindow()))
    {
        DeltaTime(window, camera);
        
        // configure transformation matrices
        g_Projection = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 1000.0f);
        g_View = camera.GetViewMatrix();

        //frameBufferExample.Run(g_View, g_Projection);
        //frameBufferExample.Exercise(g_View, g_Projection, camera);
        //cubeMapExample.Run(g_View, g_Projection);
        //cubeMapExample.Reflection(g_View, g_Projection, camera);
        //UniformBufferExample.Run(g_View, g_Projection);
        //geometryShaderExample.Run(g_View, g_Projection);
        //geometryShaderExample.VisualizeNormals(g_View, g_Projection);
        //instancingExample.Run(g_View, g_Projection);
        //antiAliasingExample.Run(g_View, g_Projection);
        //advancedLightingExample.Run(g_View, g_Projection);
        //shadowMappingExample.Run(g_View, g_Projection);
        //shadowMappingExample.ShadowsRun(g_View, g_Projection, camera.Position);
        //pointShadowsExample.Run(g_View, g_Projection);
        //normalMappingExample.Run(g_View, g_Projection);
        //parallaxMappingExample.Run(g_View, g_Projection);
		//hRExample.Run(g_View, g_Projection);
        //bloomExample.Run(g_View, g_Projection);
        //deferredShadingExample.Run(g_View, g_Projection);
		//ssaoExample.Run(g_View, g_Projection);
		//pBRExample.Run(g_View, g_Projection);
        iBLExample.Run(g_View, g_Projection);


   
        window.OnUpdate();
 
    }

    glfwTerminate();
    return 0;
}

