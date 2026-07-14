#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <unordered_map>

std::string get_file_contents(const char* filename);

class Shader
{
public:
    
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    ~Shader();
    void Bind() const;
    void UnBind() const;
    void Delete();

	// utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    void ReflectShaderInfo() const;

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
   
    void checkCompileErrors(unsigned int shader, std::string type, const char* filePath);
    void ReflectActiveUniforms();
    int GetUniformLocation(const std::string& name) const;

private:
    uint32_t m_ID;
    std::string m_Name;

    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
