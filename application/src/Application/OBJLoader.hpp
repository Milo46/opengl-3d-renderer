#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <tuple>

using OBJImportData = std::tuple<
    glm::vec3, // position
    glm::vec3, // normal
    glm::vec2  // texcoords
>;

std::vector<OBJImportData> LoadOBJFile(const std::string& filepath);

// void LoadOBJ(const char* filename, std::vector<glm::vec4&>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& elements);
