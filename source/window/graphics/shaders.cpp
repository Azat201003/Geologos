#include <window/graphics/shaders.h>


#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

std::string Shader::get_file_text(std::string path) {
    try {
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        return stream.str();
    } catch (const std::exception& e) {
        std::cout << path << ": troubles with shader loading: " << std::endl;
        std::cout << e.what() << std::endl;
        exit(1);
    }
}

void Shader::compile_shader(unsigned shader_id, const char* content) {
    int success;
    char info_log[512];

    glShaderSource(shader_id, 1, &content, NULL);
    glCompileShader(shader_id);
    // print compile errors if any
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    };
}

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    std::string vertex_code = get_file_text(vertex_path);
    std::string fragment_code = get_file_text(fragment_path);
    // 2. compile shaders
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(vertex, vertex_code.data());
    compile_shader(fragment, fragment_code.data());
    
    // shader Program
    program_id = glCreateProgram();
    // if (!program_id) {
    std::cout << program_id << std::endl;
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);
    std::cout << vertex_code << std::endl;
    // print linking errors if any
    int success;
    char info_log[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(program_id);
}

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
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]); 
} 

void Shader::set_vec4(const std::string& name, glm::vec4 value) const {
    std::cout << name << std::endl;
    glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]); 
}
