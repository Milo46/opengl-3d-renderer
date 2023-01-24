#include "OBJLoader.hpp"

#include <spdlog/spdlog.h>
#include <fstream>

static std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> SplitFace(const std::string& face)
{
    const char delimeter{ '/' };

    const auto firstDelimeter{ face.find(delimeter) };
    const auto secondDelimeter{ face.find(delimeter, firstDelimeter + 1u) };

    const auto substr1{ face.substr(0u, firstDelimeter) };
    const auto substr2{ face.substr(firstDelimeter + 1u, secondDelimeter - firstDelimeter - 1u) };
    const auto substr3{ face.substr(secondDelimeter + 1u, face.size() - secondDelimeter) };

    return std::make_tuple(
        std::stoi(substr1.empty() ? "-1" : substr1),
        std::stoi(substr2.empty() ? "-1" : substr2),
        std::stoi(substr3.empty() ? "-1" : substr3)
    );
}

std::vector<OBJImportData> LoadOBJFile(const std::string& filepath)
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
            std::string faces[3u]{};
            file >> faces[0u] >> faces[1u] >> faces[2u];

            for (std::size_t i = 0u; i < 3u; ++i)
            {
                const auto invalidValue{ static_cast<std::uint32_t>(-1) };
                const auto [vi, ti, ni]{ SplitFace(faces[i]) };

                retval.push_back({
                    vi == invalidValue ? glm::vec3{} : vertices.at(vi - 1u),
                    ni == invalidValue ? glm::vec3{} : vertices.at(ni - 1u),
                    ti == invalidValue ? glm::vec2{} : vertices.at(ti - 1u)
                });
            }
        }
    }

    return retval;
}
