#include "FrameBufferExample.h"
#include "../TextureLoader.h"


float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,


     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f

};

float quadVertices[] = {
    // positions   // texCoords
        -0.3f,  1.0f,  0.0f, 1.0f,
        -0.3f,  0.7f,  0.0f, 0.0f,
         0.3f,  0.7f,  1.0f, 0.0f,

        -0.3f,  1.0f,  0.0f, 1.0f,
         0.3f,  0.7f,  1.0f, 0.0f,
         0.3f,  1.0f,  1.0f, 1.0f
};

FrameBufferExample::FrameBufferExample() : 
m_Shader("src/shaders/FrameBuffer/5.1.framebuffers.vert", "src/shaders/FrameBuffer/5.1.framebuffers.frag"),
m_ScreenShader("src/shaders/FrameBuffer/5.1.framebuffers_screen.vert", "src/shaders/FrameBuffer/5.1.framebuffers_screen.frag")
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();
}

FrameBufferExample::~FrameBufferExample()
{
}

void FrameBufferExample::SetUpTextures()
{
	 m_CubeTexture = TextureLoader::LoadTexture("src/textures/container.jpg");
	 m_FloorTexture = TextureLoader::LoadTexture("src/textures/metal.png");
}

void FrameBufferExample::SetUpBuffers()
{
    m_CubeVB.SetData(cubeVertices, sizeof(cubeVertices));
    m_CubeVB.SetLayout({
    { ShaderDataType::Float3, "aPos" },
    { ShaderDataType::Float2, "aTexCoords" }
    });

    m_CubeVAO.AddVertexBuffer(m_CubeVB);

    m_PlaneVB.SetData(planeVertices, sizeof(planeVertices));
    m_PlaneVB.SetLayout({
    { ShaderDataType::Float3, "aPos" },
    { ShaderDataType::Float2, "aTexCoords" }
    });

    m_PlaneVAO.AddVertexBuffer(m_PlaneVB);


    m_QuadVB.SetData(quadVertices, sizeof(quadVertices));
    m_QuadVB.SetLayout({
         { ShaderDataType::Float2, "aPos" },
        { ShaderDataType::Float2, "aTexCoords" }
     });

    m_QuadVAO.AddVertexBuffer(m_QuadVB);
}

void FrameBufferExample::SetUpShaders()
{
    m_Shader.use();
    m_Shader.setInt("texture1", 0);

    m_ScreenShader.use();
    m_ScreenShader.setInt("screenTexture", 0);
}

void FrameBufferExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    m_FrameBuffer.Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.use();
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);

    m_CubeVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_PlaneVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, m_FloorTexture);
    m_Shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);

    m_FrameBuffer.UnBind();
    glDisable(GL_DEPTH_TEST); // screen quad doesn’t need depth
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Not needed
    glClear(GL_COLOR_BUFFER_BIT);

    m_ScreenShader.use();
    m_QuadVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, m_FrameBuffer.GetTextureColorBuffer());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}

void FrameBufferExample::Exercise(glm::mat4& view, glm::mat4& projection, Camera camera)
{
    m_FrameBuffer.Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.use();
    
    glm::mat4 model = glm::mat4(1.0f);
    camera.Yaw += 180.0f; // rotate the camera's yaw 180 degrees around
    camera.ProcessMouseMovement(0, 0, false); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
    view = camera.GetViewMatrix();
    camera.Yaw -= 180.0f; // reset it back to its original orientation
    camera.ProcessMouseMovement(0, 0, true);
    //projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", projection);

    //Cubes
    m_CubeVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Floor
    m_PlaneVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, m_FloorTexture);
    m_Shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // second render pass: draw as normal
    m_FrameBuffer.UnBind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    m_Shader.setMat4("view", view);

    //Cubes
    m_CubeVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    m_Shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Floor
    m_PlaneVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, m_FloorTexture);
    m_Shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // now draw the mirror quad with screen texture
    glDisable(GL_DEPTH_TEST);
    m_ScreenShader.use();
    m_QuadVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, m_FrameBuffer.GetTextureColorBuffer());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
