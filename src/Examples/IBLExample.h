#pragma once

#include "IExample.h"


#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Geometry/Model.h"
#include "../Texture/Texture2D.h"
#include "../Texture/TextureCubeMap.h"

#include <vector>

class IBLExample : public IExample
{
public:
	IBLExample(uint32_t& width, uint32_t& height, Camera& camera);
	~IBLExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

    void RenderCube();
    void RenderQuad();

	void RenderSphere();

private:
    uint32_t& m_Width;
    uint32_t& m_Height;
	Camera& m_Camera;

	Shader m_PbrShader;
	Shader m_EquirectangularToCubemapShader;
	Shader m_IrradianceShader;
	Shader m_PrefilterShader;
	Shader m_BrdfShader;
	Shader m_BackgroundShader;

	VertexArray m_SphereVAO;
	VertexBuffer m_SphereVBO;
	IndexBuffer m_SphereEBO;

    VertexArray m_QuadVAO;
    VertexBuffer m_QuadVBO;

    VertexArray m_CubeVAO;
    VertexBuffer m_CubeVBO;

	FrameBuffer m_CaptureFBO;

    // rusted iron
    Texture2D m_IronAlbedoMap;
    Texture2D m_IronNormalMap;
    Texture2D m_IronMetallicMap;
    Texture2D m_IronRoughnessMap;
    Texture2D m_IronAOMap;

    // gold
    Texture2D m_GoldAlbedoMap;
    Texture2D m_GoldNormalMap;
    Texture2D m_GoldMetallicMap;
    Texture2D m_GoldRoughnessMap;
    Texture2D m_GoldAOMap;

    // grass
    Texture2D m_GrassAlbedoMap;
    Texture2D m_GrassNormalMap;
    Texture2D m_GrassMetallicMap;
    Texture2D m_GrassRoughnessMap;
    Texture2D m_GrassAOMap;

    // plastic
    Texture2D m_PlasticAlbedoMap;
    Texture2D m_PlasticNormalMap;
    Texture2D m_PlasticMetallicMap;
    Texture2D m_PlasticRoughnessMap;
    Texture2D m_PlasticAOMap;

    // wall
    Texture2D m_WallAlbedoMap;
    Texture2D m_WallNormalMap;
    Texture2D m_WallMetallicMap;
    Texture2D m_WallRoughnessMap;
    Texture2D m_WallAOMap;

	Texture2D m_HdrTexture;
	Texture2D m_BrdfLUTTexture;

    TextureCubeMap m_EnvCubemap;
    TextureCubeMap m_IrradianceMap;
	TextureCubeMap m_PrefilterMap;

	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	int m_NrRows = 7;
	int m_NrColumns = 7;
	float m_Spacing = 2.5;

};