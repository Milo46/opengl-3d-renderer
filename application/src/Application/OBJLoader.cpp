#include "OBJLoader.hpp"

#include <spdlog/spdlog.h>
#include <fstream>

#include <iostream>
#include <sstream>
#include <map>

static std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> SplitFace(const std::string& face)
{
    constexpr char delimeter{ '/' };

    const auto firstDelimeter{ face.find(delimeter) };
    const auto secondDelimeter{ face.find(delimeter, firstDelimeter + 1u) };

    const auto substr1{ face.substr(0u, firstDelimeter) };
    const auto substr2{ firstDelimeter == std::string::npos ? "" : face.substr(firstDelimeter + 1u, secondDelimeter - firstDelimeter - 1u) };
    const auto substr3{ secondDelimeter == std::string::npos ? "" : face.substr(secondDelimeter + 1u, face.size() - secondDelimeter) };

    return std::make_tuple(
        static_cast<std::uint32_t>(std::stoi(substr1.empty() ? "-1" : substr1)),
        static_cast<std::uint32_t>(std::stoi(substr2.empty() ? "-1" : substr2)),
        static_cast<std::uint32_t>(std::stoi(substr3.empty() ? "-1" : substr3))
    );
}

std::vector<OBJImportData> LoadOBJFile(const std::string& filepath, FaceType faceType)
{
    std::ifstream file{ filepath };
    if (!file.is_open())
    {
        spdlog::info("Bad file!");
        return {};
    }

    std::vector<glm::vec3> vertices{};
    std::vector<glm::vec3> normals{};
    std::vector<glm::vec2> texCoords{};

    std::vector<OBJImportData> retval{};

    while (!file.eof())
    {
        std::string lineHeader{};
        file >> lineHeader;

        if (lineHeader == "v")
        {
            glm::vec3 vertex{};
            file >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (lineHeader == "vt")
        {
            glm::vec2 texCoord{};
            file >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (lineHeader == "vn")
        {
            glm::vec3 normal{};
            file >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (lineHeader == "f")
        {
            if (faceType == FaceType::Triangle)
            {
                std::string faces[3u]{};
                file >> faces[0u] >> faces[1u] >> faces[2u];

                for (std::size_t i = 0u; i < 3u; ++i)
                {
                    constexpr auto invalidValue{ static_cast<std::uint32_t>(-1) };
                    const auto [vi, ti, ni]{ SplitFace(faces[i]) };

                    retval.push_back({
                        vi == invalidValue ? glm::vec3{} : vertices.at(vi - 1u),
                        ni == invalidValue ? glm::vec3{} : normals.at(ni - 1u),
                        ti == invalidValue ? glm::vec2{} : texCoords.at(ti - 1u)
                    });
                }
            }
            else if (faceType == FaceType::Quad)
            {
                OBJImportData quad[4u]{};

                std::string faces[4u]{};
                file >> faces[0u] >> faces[1u] >> faces[2u] >> faces[3u];

                for (std::size_t i = 0u; i < 4u; ++i)
                {
                    constexpr auto invalidValue{ static_cast<std::uint32_t>(-1) };
                    const auto [vi, ti, ni]{ SplitFace(faces[i]) };

                    quad[i] = {
                        vi == invalidValue ? glm::vec3{} : vertices.at(vi - 1u),
                        ni == invalidValue ? glm::vec3{} : normals.at(ni - 1u),
                        ti == invalidValue ? glm::vec2{} : texCoords.at(ti - 1u)
                    };
                }

                retval.push_back(quad[2u]); retval.push_back(quad[3u]); retval.push_back(quad[1u]);
                retval.push_back(quad[0u]); retval.push_back(quad[1u]); retval.push_back(quad[3u]);
            }
        }
    }

    return retval;
}

// std::vector<OBJVertexData> LoadObjectModel(const std::string& filepath)
// {
//     std::ifstream file{ filepath };
//     if (!file.is_open())
//     {
//         spdlog::error("Failed to load the object model: {}", filepath);
//         return {};
//     }

//     std::vector<OBJVertexData> retval{};

//     std::vector<decltype(OBJVertexData::Position)> vertices{};
//     std::vector<decltype(OBJVertexData::Normal)> normals{};
//     std::vector<decltype(OBJVertexData::Texcoord)> texcoords{};

//     while (!file.eof())
//     {
//         std::string lineHeader{};
//         file >> lineHeader;

//         if (lineHeader == "v")
//         {
//             decltype(vertices)::value_type vertex{};
//             file >> vertex;
//             vertices.push_back(vertex);
//         }
//         else if (lineHeader == "vn")
//         {
//             decltype(normals)::value_type normal{};
//             file >> normal;
//             normals.push_back(normal);
//         }
//         else if (lineHeader == "vt")
//         {
//             decltype(texcoords)::value_type texcoord{};
//             file >> texcoord;
//             texcoords.push_back(texcoord);
//         }
//         else if (lineHeader == "f")
//         {
            
//         }
//     }

//     return retval;
// }
