#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>

class Shader {
private:
	std::string get_file_text(std::string filename);
	void compile_shader(unsigned shader_id, const char *content);
public:
	unsigned int program_id;
	Shader(const char *vertexPath, const char *fragmentPath);
	void use();
	void set_bool(const std::string &name, bool value) const;
	void set_mat4(const std::string &name, glm::mat4 value) const;
	void set_vec4(const std::string &name, glm::vec4 value) const;
	void set_int(const std::string &name, int value) const;
	void set_float(const std::string &name, float value) const;
};

enum class ShaderKit {
	DEFAULT,
	TEXTURES,
};


class ShaderStorage {
private:
	static std::unordered_map<ShaderKit, std::shared_ptr<Shader>> shaders;
	static const std::unordered_map<ShaderKit, std::pair<char* const, char* const>> shader_kits;
public:
	static void load();
	static std::shared_ptr<Shader> get_shader(ShaderKit);
};

