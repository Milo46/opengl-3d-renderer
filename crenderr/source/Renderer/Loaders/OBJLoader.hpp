// #pragma once

// #include "Renderer/Renderer.hpp"

// enum class FaceType
// {
//     None = 0,
//     Triangle, Quad,
// };

// class OBJLoader
// {
// public:
//     OBJLoader() = default;

// public:
//     std::vector<Renderer::Vertex3D> LoadData(const std::string& filepath) const;
//     std::shared_ptr<Renderer::VertexArray> LoadModel(const std::string& filepath) const;

// private:
//     FaceType m_FaceType{ FaceType::Triangle };
// };

#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <vector>
#include <string>

#include "Renderer/Backend/VertexArray.hpp"
#include "Renderer/Renderer.hpp"

enum class FaceType
{
    Triangle,
    Quad,
};

struct OBJModelData
{
    std::vector<Renderer::Vertex3D> Data{};
};

OBJModelData LoadOBJFile(const std::string& filepath, FaceType faceType = FaceType::Triangle);
std::shared_ptr<Renderer::VertexArray> LoadOBJModel(const std::string& filepath, FaceType faceType = FaceType::Triangle);
