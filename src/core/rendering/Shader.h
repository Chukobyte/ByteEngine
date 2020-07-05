#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "../../../lib/glm/glm.hpp"
#include "../../../lib/glm/gtc/matrix_transform.hpp"
#include "../../../lib/glm/gtc/type_ptr.hpp"

class Shader {
  public:
    unsigned int ID;

    Shader();

    Shader(const char* vertexPath, const char* fragmentPath);

    Compile(const char* vertexPath, const char* fragmentPath);

    void Use();

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;

    void SetVec2Float(const std::string &name, float v1, float v2) const;

    void SetVec3Float(const std::string &name, const glm::vec3 &value) const;

    void SetVec3Float(const std::string &name, float v1, float v2, float v3) const;

    void SetVec4Float(const std::string &name, float v1, float v2, float v3, float v4) const;

    void SetMatrix4Float(const std::string &name, const glm::mat4 &mat) const;

  private:
    CheckCompileErrors(unsigned int shader, std::string type);
};

#endif
