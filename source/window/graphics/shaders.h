#pragma once

#include <string>
// #include <iostream>
#include <glm/glm.hpp>

class Shader {
private:
    std::string get_file_text(std::string filename);
    void compile_shader(unsigned shader_id, const char* content);
public:
    unsigned int program_id;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void set_bool(const std::string& name, bool value) const;
    void set_mat4(const std::string& name, glm::mat4 value) const;
    void set_vec4(const std::string& name, glm::vec4 value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
};