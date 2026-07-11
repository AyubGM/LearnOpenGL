#include "Shader.h";
#include <filesystem>

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
	std::runtime_error("Failed to open file: " + std::string(filename));
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

	// utility uniform functions
	// ------------------------------------------------------------------------
	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	 void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }

	 void Shader::setVec3(const std::string& name, glm::vec3 value) const
	{
		 glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::setMat4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1,GL_FALSE, glm::value_ptr(value));
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

	void Shader::ReflectActiveUniforms() 
	{
		int numActiveUniforms = 0;
		glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

		int maxNameLength = 0;
		glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

		if (numActiveUniforms > 0 && maxNameLength > 0)
		{
			std::vector<char> bufferName(maxNameLength);
			for (int i = 0; i < numActiveUniforms; i++)
			{
				int size = 0;
				uint32_t type = 0;
				int length = 0;
				glGetActiveUniform(m_ID, i, maxNameLength, &length, &size, &type, bufferName.data());

				std::string name(bufferName.data(), length);

				int location = glGetUniformLocation(m_ID, name.c_str());
				m_UniformLocationCache[name] = location;
			}
		}
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		auto it = m_UniformLocationCache.find(name);
		if (m_UniformLocationCache.contains(name))
		{
			return m_UniformLocationCache[name];
		}

		// Lazy cache fallback in case the dynamic name array references something not pre-cached
		int location = glGetUniformLocation(m_ID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}