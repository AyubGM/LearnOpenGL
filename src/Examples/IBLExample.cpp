#include "IBLExample.h"
#include <GLFW/glfw3.h>

IBLExample::IBLExample(uint32_t& width, uint32_t& height, Camera& camera) : m_Width(width), m_Height(height), m_Camera(camera), m_PbrShader("Shaders/IBL/2.2.2.pbr.vert", "Shaders/IBL/2.2.2.pbr.frag"),
m_EquirectangularToCubemapShader("Shaders/IBL/2.2.2.cubemap.vert", "Shaders/IBL/2.2.2.equirectangular_to_cubemap.frag"),
m_IrradianceShader("Shaders/IBL/2.2.2.cubemap.vert", "Shaders/IBL/2.2.2.irradiance_convolution.frag"),
m_PrefilterShader("Shaders/IBL/2.2.2.cubemap.vert", "Shaders/IBL/2.2.2.prefilter.frag"),
m_BrdfShader("Shaders/IBL/2.2.2.brdf.vert", "Shaders/IBL/2.2.2.brdf.frag"),
m_BackgroundShader("Shaders/IBL/2.2.2.background.vert", "Shaders/IBL/2.2.2.background.frag"),

m_IronAlbedoMap("resources/textures/pbr/rusted_iron/albedo.png"),
m_IronNormalMap("resources/textures/pbr/rusted_iron/normal.png"),
m_IronMetallicMap("resources/textures/pbr/rusted_iron/metallic.png"),
m_IronRoughnessMap("resources/textures/pbr/rusted_iron/roughness.png"),
m_IronAOMap("resources/textures/pbr/rusted_iron/ao.png"),

m_GoldAlbedoMap("resources/textures/pbr/gold/albedo.png"),
m_GoldNormalMap("resources/textures/pbr/gold/normal.png"),
m_GoldMetallicMap("resources/textures/pbr/gold/metallic.png"),
m_GoldRoughnessMap("resources/textures/pbr/gold/roughness.png"),
m_GoldAOMap("resources/textures/pbr/gold/ao.png"),

m_GrassAlbedoMap("resources/textures/pbr/grass/albedo.png"),
m_GrassNormalMap("resources/textures/pbr/grass/normal.png"),
m_GrassMetallicMap("resources/textures/pbr/grass/metallic.png"),
m_GrassRoughnessMap("resources/textures/pbr/grass/roughness.png"),
m_GrassAOMap("resources/textures/pbr/grass/ao.png"),

m_PlasticAlbedoMap("resources/textures/pbr/plastic/albedo.png"),
m_PlasticNormalMap("resources/textures/pbr/plastic/normal.png"),
m_PlasticMetallicMap("resources/textures/pbr/plastic/metallic.png"),
m_PlasticRoughnessMap("resources/textures/pbr/plastic/roughness.png"),
m_PlasticAOMap("resources/textures/pbr/plastic/ao.png"),

m_WallAlbedoMap("resources/textures/pbr/wall/albedo.png"),
m_WallNormalMap("resources/textures/pbr/wall/normal.png"),
m_WallMetallicMap("resources/textures/pbr/wall/metallic.png"),
m_WallRoughnessMap("resources/textures/pbr/wall/roughness.png"),
m_WallAOMap("resources/textures/pbr/wall/ao.png"),
m_HdrTexture("resources/textures/hdr/newport_loft.hdr"),
m_CaptureFBO([&] {
	FramebufferSpecification spec;

	spec.Width = 512;
	spec.Height = 512;
	spec.Attachments = { FramebufferTextureFormat::DEPTH24STENCIL8 };

	return FrameBuffer(spec);
	}())
{
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();

	m_LightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	m_LightColors = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
}

IBLExample::~IBLExample()
{
}



void IBLExample::SetUpTextures()
{
}

void IBLExample::SetUpBuffers()
{
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	m_QuadVBO.SetData(quadVertices, sizeof(quadVertices));
	m_QuadVBO.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoords"}
		});

	m_QuadVAO.AddVertexBuffer(m_QuadVBO);

	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		 // bottom face
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 // top face
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	m_CubeVBO.SetData(cubeVertices, sizeof(cubeVertices));
	m_CubeVBO.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoords"}
		});

	m_CubeVAO.AddVertexBuffer(m_CubeVBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;
	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	std::vector<float> data;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
	}

	m_SphereVBO.SetData(data.data(), data.size() * sizeof(float));
	m_SphereVBO.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoords"}
		});

	m_SphereEBO.SetData(indices.data(), indices.size());

	m_SphereVAO.AddVertexBuffer(m_SphereVBO);
	m_SphereVAO.SetIndexBuffer(m_SphereEBO);
}

void IBLExample::SetUpShaders()
{

	m_BackgroundShader.setInt("environmentMap", 0);

	m_PbrShader.setInt("irradianceMap", 0);
	m_PbrShader.setInt("prefilterMap", 1);
	m_PbrShader.setInt("brdfLUT", 2);
	m_PbrShader.setInt("albedoMap", 3);
	m_PbrShader.setInt("normalMap", 4);
	m_PbrShader.setInt("metallicMap", 5);
	m_PbrShader.setInt("roughnessMap", 6);
	m_PbrShader.setInt("aoMap", 7);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	m_EquirectangularToCubemapShader.Bind();
	m_EquirectangularToCubemapShader.setInt("equirectangularMap", 0);
	m_EquirectangularToCubemapShader.setMat4("projection", captureProjection);

	m_HdrTexture.Bind();
	glViewport(0, 0, 512, 512);

	m_EnvCubemap.AllocateEmpty(512, 512);

	m_CaptureFBO.Bind();
	m_CaptureFBO.SetDrawBuffer(GL_COLOR_ATTACHMENT0);
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_EquirectangularToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap.GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}
	m_CaptureFBO.UnBind();

	m_IrradianceMap.AllocateEmpty(32, 32);

	m_CaptureFBO.Resize(32, 32);

	m_IrradianceShader.Bind();
	m_IrradianceShader.setInt("environmentMap", 0);
	m_IrradianceShader.setMat4("projection", captureProjection);

	m_EnvCubemap.Bind();
	glViewport(0, 0, 32, 32);

	m_CaptureFBO.Bind();
	m_CaptureFBO.SetDrawBuffer(GL_COLOR_ATTACHMENT0);
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_IrradianceShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap.GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}
	m_CaptureFBO.UnBind();

	m_PrefilterMap.AllocateEmpty(128, 128);

	m_PrefilterShader.Bind();
	m_PrefilterShader.setInt("environmentMap", 0);
	m_PrefilterShader.setMat4("projection", captureProjection);
	m_EnvCubemap.Bind();

	m_CaptureFBO.Resize(128, 128);
	m_CaptureFBO.Bind();
	m_CaptureFBO.SetDrawBuffer(GL_COLOR_ATTACHMENT0);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));


		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		m_PrefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_PrefilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap.GetID(), mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCube();
		}
	}

	m_CaptureFBO.UnBind();

	m_BrdfLUTTexture.AllocateEmpty(512, 512, GL_RG16F);

	m_CaptureFBO.Bind();
	m_CaptureFBO.Resize(512, 512);
	m_CaptureFBO.SetDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLUTTexture.GetID(), 0);
	glViewport(0, 0, 512, 512);
	m_BrdfShader.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderQuad();
	m_CaptureFBO.UnBind();

	glViewport(0, 0, m_Width, m_Height);

}

void IBLExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		m_PbrShader.Bind();
		m_PbrShader.setMat4("projection", projection);
		m_BackgroundShader.Bind();
		m_BackgroundShader.setMat4("projection", projection);
	}
	

	m_PbrShader.Bind();
	glm::mat4 model = glm::mat4(1.0f);
	m_PbrShader.setMat4("view", view);
	m_PbrShader.setVec3("camPos", m_Camera.Position);

	// bind pre-computed IBL data
	m_IrradianceMap.Bind(0);
	m_PrefilterMap.Bind(1);
	m_BrdfLUTTexture.Bind(2);

	// rusted iron
	m_IronAlbedoMap.Bind(3);
	m_IronNormalMap.Bind(4);
	m_IronMetallicMap.Bind(5);
	m_IronRoughnessMap.Bind(6);
	m_IronAOMap.Bind(7);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
	m_PbrShader.setMat4("model", model);
	m_PbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	RenderSphere();

	// gold
	m_GoldAlbedoMap.Bind(3);
	m_GoldNormalMap.Bind(4);
	m_GoldMetallicMap.Bind(5);
	m_GoldRoughnessMap.Bind(6);
	m_GoldAOMap.Bind(7);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
	m_PbrShader.setMat4("model", model);
	m_PbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	RenderSphere();

	//grass
	m_GrassAlbedoMap.Bind(3);
	m_GrassNormalMap.Bind(4);
	m_GrassMetallicMap.Bind(5);
	m_GrassRoughnessMap.Bind(6);
	m_GrassAOMap.Bind(7);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
	m_PbrShader.setMat4("model", model);
	m_PbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	RenderSphere();

	// wall
	m_WallAlbedoMap.Bind(3);
	m_WallNormalMap.Bind(4);
	m_WallMetallicMap.Bind(5);
	m_WallRoughnessMap.Bind(6);
	m_WallAOMap.Bind(7);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
	m_PbrShader.setMat4("model", model);
	m_PbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	RenderSphere();

	for (unsigned int i = 0; i < m_LightPositions.size(); ++i)
	{
		glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		//newPos = m_LightPositions[i];
		m_PbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		m_PbrShader.setVec3("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		m_PbrShader.setMat4("model", model);
		m_PbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		RenderSphere();
	}

	m_BackgroundShader.Bind();
	m_BackgroundShader.setMat4("view", view);
	m_EnvCubemap.Bind(0);

	RenderCube();
}


void IBLExample::RenderQuad()
{
	m_QuadVAO.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void IBLExample::RenderCube()
{
	m_CubeVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void IBLExample::RenderSphere()
{
	m_SphereVAO.Bind();
	glDrawElements(GL_TRIANGLE_STRIP, m_SphereEBO.GetCount(), GL_UNSIGNED_INT, 0);
}
