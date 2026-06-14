#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <window/graphics/shaders.h>

#include <fstream>
#include <sstream>

std::string Shader::get_file_text(std::string path) {
	try {
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		return stream.str();
	} catch (const std::exception &e) {
		spdlog::error(
R"({}: troubles with shader loading:
{})",
		path, e.what());
		return "";
	}
}

void Shader::compile_shader(unsigned shader_id, const char *content) {
	int success;
	char info_log[512];

	glShaderSource(shader_id, 1, &content, NULL);
	glCompileShader(shader_id);
	// print compile errors if any
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_id, 512, NULL, info_log);
		spdlog::error("Shader compilation failed: {}", info_log);
	};
}

Shader::Shader(const char *vertex_path, const char *fragment_path) {
	std::string vertex_code = get_file_text(vertex_path);
	std::string fragment_code = get_file_text(fragment_path);
	
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	compile_shader(vertex, vertex_code.data());
	spdlog::info("Vertex shader compiled ({})", vertex_path);
	compile_shader(fragment, fragment_code.data());
	spdlog::info("Fragment shader compiled ({})", fragment_path);

	program_id = glCreateProgram();
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);
	glLinkProgram(program_id);
	int success;
	char info_log[512];
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_id, 512, NULL, info_log);
		spdlog::error("Shaders linking failed: {}", info_log);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(program_id); }

void Shader::set_bool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_mat4(const std::string &name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1,
										 GL_FALSE, &value[0][0]);
}

void Shader::set_vec4(const std::string &name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

// {vertex path, fragment path}
const std::unordered_map<ShaderKit, std::pair<char* const, char* const>> ShaderStorage::shader_kits {
	{ShaderKit::DEFAULT, {"resources/shaders/default/vertex.glsl", "resources/shaders/default/fragment.glsl"}},
	{ShaderKit::TEXTURES, {"resources/shaders/textures/vertex.glsl", "resources/shaders/textures/fragment.glsl"}},
};

std::unordered_map<ShaderKit, std::shared_ptr<Shader>> ShaderStorage::shaders {};

void ShaderStorage::load() {
	for (auto shader_kit : shader_kits) {
		shaders.emplace(shader_kit.first, std::make_shared<Shader>(shader_kit.second.first, shader_kit.second.second));
	}
	spdlog::info("Shaders loaded");
}

std::shared_ptr<Shader> ShaderStorage::get_shader(ShaderKit shader_kit) {
	return shaders[shader_kit];
}

