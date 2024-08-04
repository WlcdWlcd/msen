#include "ShaderProgram.hpp"
#include "Core/Log.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace msen {
    bool check_shader(GLuint shader) {
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            LOG_CRITICAL(infoLog);
        }
        return success;
    }

    bool check_link_status(GLuint& id, GLuint& vertex_shader_id, GLuint& fragment_shader_id) {
        int success;
        glGetShaderiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            LOG_CRITICAL("SHADER PROGRAM: link-time error:\n{o}", infoLog);
            glDeleteProgram(id);
            id = 0;
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
        }
        return success;
    }

    bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id)
    {
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &source, nullptr);
        glCompileShader(shader_id);

        return check_shader(shader_id);
    }

    ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src) {
        GLuint vertex_shader_id = 0;
        if (!create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id)) {
            LOG_CRITICAL("VERTEX SHADER: compile-time error");
            glDeleteShader(vertex_shader_id);
            return;
        }

        GLuint fragment_shader_id = 0;
        if (!create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id)) {
            LOG_CRITICAL("FRAGMENT SHADER: compile-time error");
            glDeleteShader(fragment_shader_id);
            return;
        }
        m_id = glCreateProgram();
        glAttachShader(m_id, vertex_shader_id);
        glAttachShader(m_id, fragment_shader_id);
        glLinkProgram(m_id);

        if (!check_link_status(m_id, vertex_shader_id, fragment_shader_id))
        {
            return;
        }
        m_isCompiled = true;

        glDetachShader(m_id, vertex_shader_id);
        glDetachShader(m_id, fragment_shader_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }

    ShaderProgram::~ShaderProgram(){
        glDeleteProgram(m_id);
    }

    void ShaderProgram::bind() const{
        glUseProgram(m_id);
    }
    
    void ShaderProgram::unbind() {
        glUseProgram(0);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) {
        glDeleteProgram(m_id);
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram (ShaderProgram&& shaderProgram) {
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;
    }

    void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }


}