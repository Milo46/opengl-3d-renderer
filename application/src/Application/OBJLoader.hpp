#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <vector>
#include <string>

enum class FaceType
{
    Triangle,
    Quad,
};

struct OBJVertexData
{
    glm::vec3 Position{};
    glm::vec3 Normal{};
    glm::vec2 Texcoord{};
};

struct OBJModelData
{
    std::vector<OBJVertexData> Data{};
};

OBJModelData LoadOBJFile(const std::string& filepath, FaceType faceType = FaceType::Triangle);
