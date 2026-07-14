#include "Shader.h"
#include <filesystem>
#include <print>

namespace Utils {

	static std::string GLTypeToString(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:              return "float";
		case GL_FLOAT_VEC2:         return "vec2";
		case GL_FLOAT_VEC3:         return "vec3";
		case GL_FLOAT_VEC4:         return "vec4";
		case GL_INT:                return "int";
		case GL_INT_VEC2:           return "ivec2";
		case GL_INT_VEC3:           return "ivec3";
		case GL_INT_VEC4:           return "ivec4";
		case GL_BOOL:               return "bool";
		case GL_FLOAT_MAT4:         return "mat4";
		case GL_SAMPLER_2D:         return "sampler2D";
		case GL_SAMPLER_2D_MULTISAMPLE: return "sampler2DMS";
		default:                    return "Unknown Type (" + std::to_string(type) + ")";
		}
	}

}

std::string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);

	}
	throw std::runtime_error("Failed to open file: " + std::string(filename));
}


Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
		std::string vertexCode = get_file_contents(vertexFile);
		std::string fragmentCode = get_file_contents(fragmentFile);

		
		std::filesystem::path path = vertexFile;
		m_Name = path.stem().string();


		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		// creating the vertex shader 
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX", vertexFile);

		//creating the fragment shader AKA the color shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT", fragmentFile);
	
		//link the shaders to the program

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM", "Shader Linker");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		ReflectActiveUniforms();

#ifdef _DEBUG
		ReflectShaderInfo();
#endif // _DEBUG

}

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryPath)
	{
		std::string vertexCode = get_file_contents(vertexFile);
		std::string fragmentCode = get_file_contents(fragmentFile);
		std::string geometryCode = get_file_contents(geometryPath);

		std::filesystem::path path = vertexFile;
		m_Name = path.stem().string();

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();
		const char* geometrySource = geometryCode.c_str();

		// creating the vertex shader 
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX", vertexFile);

		//creating the fragment shader AKA the color shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT", fragmentFile);

		//creating the geometry shader 
		unsigned int geometryShader;
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		glCompileShader(geometryShader);
		checkCompileErrors(geometryShader, "GEOMETRY", geometryPath);

		//link the shaders to the program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		glAttachShader(m_ID, geometryShader);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM", "Shader Linker");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		ReflectActiveUniforms();

#ifdef _DEBUG
		ReflectShaderInfo();
#endif // _DEBUG
	}

Shader::~Shader()
	{
		Delete();
	}

void Shader::Bind() const
	{
		glUseProgram(m_ID);
	};

void Shader::UnBind() const
	{
		glUseProgram(0);
	};
void Shader::Delete()
	{
		glDeleteProgram(m_ID);
	};


void Shader::ReflectActiveUniforms() 
{
	m_UniformLocationCache.clear();


	int numUniforms = 0;
	glGetProgramInterfaceiv(m_ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	std::vector<char> nameData(256);
	for (int i = 0; i < numUniforms; ++i)
	{
		GLenum properties[] = { GL_NAME_LENGTH, GL_LOCATION };
		GLint results[2];
		glGetProgramResourceiv(m_ID, GL_UNIFORM, i, 2, properties, 2, nullptr, results);

		int nameLength = results[0];
		int location = results[1];

		// Standard uniforms have a valid location; UBO uniforms return -1
		if (location != -1)
		{
			std::string name(nameLength, '\0');
			glGetProgramResourceName(m_ID, GL_UNIFORM, i, nameLength, nullptr, name.data());
			name.pop_back();

			m_UniformLocationCache[name] = location;
		}
	}
}

void Shader::ReflectShaderInfo() const
{
	int numActiveInputs = 0;
	glGetProgramInterfaceiv(m_ID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveInputs);

	std::println("\n==================================================");
	std::println("Shader Info: {} Shader", m_Name);
	std::println("==================================================");
	std::println("");
	auto GetResourceName = [&](GLenum programInterface, uint32_t index, int length) {
		std::string name(length, '\0');
		glGetProgramResourceName(m_ID, programInterface, index, length, nullptr, name.data());
		name.pop_back();
		return name;
		};

	// 1. VERTEX INPUTS
	int numInputs = 0;
	glGetProgramInterfaceiv(m_ID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numInputs);
	std::println("[Vertex Attributes Input Elements: {}]", numInputs);
	for (int i = 0; i < numInputs; ++i)
	{
		GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
		GLint res[3];
		glGetProgramResourceiv(m_ID, GL_PROGRAM_INPUT, i, 3, props, 3, nullptr, res);
		std::println("Location {:2}: {} ({})", res[2], GetResourceName(GL_PROGRAM_INPUT, i, res[0]), Utils::GLTypeToString(res[1]));
	}
	std::println("");

	// 2. FRAGMENT OUTPUTS
	int numOutputs = 0;
	glGetProgramInterfaceiv(m_ID, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numOutputs);
	std::println("[Fragment Color Outputs: {}]", numOutputs);
	for (int i = 0; i < numOutputs; ++i)
	{
		GLenum props[] = { GL_NAME_LENGTH, GL_LOCATION };
		GLint res[2];
		glGetProgramResourceiv(m_ID, GL_PROGRAM_OUTPUT, i, 2, props, 2, nullptr, res);
		std::println("Color Attachment Location {}: {}", res[1], GetResourceName(GL_PROGRAM_OUTPUT, i, res[0]));
	}
	std::println("");

	// 3. STANDARD LOOSE UNIFORMS & SAMPLERS
	int numUniforms = 0;
	glGetProgramInterfaceiv(m_ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	std::println("[Active Uniforms & Samplers]");
	for (int i = 0; i < numUniforms; ++i)
	{
		GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
		GLint res[3];
		glGetProgramResourceiv(m_ID, GL_UNIFORM, i, 3, props, 3, nullptr, res);

		// Skip variables packed inside uniform blocks since they are tracked in section 4
		if (res[2] != -1)
		{
			std::println("Location {:2}: {} ({})", res[2], GetResourceName(GL_UNIFORM, i, res[0]), Utils::GLTypeToString(res[1]));
		}
	}
	std::println("");


	// 4. UNIFORM BLOCKS (UBOs)
	int numUbos = 0;
	glGetProgramInterfaceiv(m_ID, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUbos);
	std::println("[Uniform Buffer Blocks (UBOs): {}]", numUbos);
	for (int i = 0; i < numUbos; ++i)
	{
		GLenum props[] = { GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE };
		GLint res[3];
		glGetProgramResourceiv(m_ID, GL_UNIFORM_BLOCK, i, 3, props, 3, nullptr, res);
		std::println("Block Binding {}: {} (Size: {} Bytes)", res[1], GetResourceName(GL_UNIFORM_BLOCK, i, res[0]), res[2]);
	}
	std::println("");

	// 5. SHADER STORAGE BLOCKS (SSBOs)
	int numSsbos = 0;
	glGetProgramInterfaceiv(m_ID, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numSsbos);
	std::println("[Shader Storage Blocks (SSBOs): {}]", numSsbos);
	for (int i = 0; i < numSsbos; ++i)
	{
		GLenum props[] = { GL_NAME_LENGTH, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE };
		GLint res[3];
		glGetProgramResourceiv(m_ID, GL_SHADER_STORAGE_BLOCK, i, 3, props, 3, nullptr, res);
		std::println("Block Binding {}: {} (Size: {} Bytes)", res[1], GetResourceName(GL_SHADER_STORAGE_BLOCK, i, res[0]), res[2]);
	}
	std::println("");

}

int Shader::GetUniformLocation(const std::string& name) const
{
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end())
		{
			return it->second;
		}

		// Lazy cache fallback in case the dynamic name array references something not pre-cached
		int location = glGetUniformLocation(m_ID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
}


// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
	glProgramUniform1i(m_ID, GetUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glProgramUniform1i(m_ID, GetUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glProgramUniform1f(m_ID, GetUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glProgramUniform2fv(m_ID, GetUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glProgramUniform3fv(m_ID, GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glProgramUniformMatrix4fv(m_ID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type, const char* filePath)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << "File Path: " << filePath << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << "File Path: " << filePath << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
};