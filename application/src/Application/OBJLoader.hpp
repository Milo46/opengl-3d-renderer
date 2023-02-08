#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <tuple>

#include "Utility/FileManager.hpp"

enum class FaceType
{
    Triangle,
    Quad,
};

using OBJImportData = std::tuple<
    glm::vec3, // position
    glm::vec3, // normal
    glm::vec2  // texcoords
>;

std::vector<OBJImportData> LoadOBJFile(const std::string& filepath, FaceType faceType = FaceType::Triangle);

// struct OBJVertexData
// {
//     glm::vec3 Position{};
//     glm::vec3 Normal{};
//     glm::vec2 Texcoord{};
// };

// std::vector<OBJVertexData> LoadObjectModel(const std::string& filepath);
