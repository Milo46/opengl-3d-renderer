// #include "OBJLoader.hpp"

// #include <fstream>

// static std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> SplitFace(const std::string& face)
// {
//     constexpr char delimeter{ '/' };

//     const auto firstDelimeter{ face.find(delimeter) };
//     const auto secondDelimeter{ face.find(delimeter, firstDelimeter + 1u) };

//     const auto substr1{ face.substr(0u, firstDelimeter) };
//     const auto substr2{ firstDelimeter == std::string::npos ? "" : face.substr(firstDelimeter + 1u, secondDelimeter - firstDelimeter - 1u) };
//     const auto substr3{ secondDelimeter == std::string::npos ? "" : face.substr(secondDelimeter + 1u, face.size() - secondDelimeter) };

//     return std::make_tuple(
//         static_cast<std::uint32_t>(std::stoi(substr1.empty() ? "-1" : substr1)),
//         static_cast<std::uint32_t>(std::stoi(substr2.empty() ? "-1" : substr2)),
//         static_cast<std::uint32_t>(std::stoi(substr3.empty() ? "-1" : substr3))
//     );
// }

// std::vector<Renderer::Vertex3D> OBJLoader::LoadData(const std::string& filepath)
// {
//     std::ifstream file{ filepath };
//     if (!file.is_open())
//     {
//         spdlog::info("Bad file!");
//         return {};
//     }

//     std::vector<glm::vec3> vertices{};
//     std::vector<glm::vec3> normals{};
//     std::vector<glm::vec2> texCoords{};

//     decltype(OBJModelData::Data) data{};

//     while (!file.eof())
//     {
//         std::string lineHeader{};
//         file >> lineHeader;

//         if (lineHeader == "v")
//         {
//             glm::vec3 vertex{};
//             file >> vertex.x >> vertex.y >> vertex.z;
//             vertices.push_back(vertex);
//         }
//         else if (lineHeader == "vt")
//         {
//             glm::vec2 texCoord{};
//             file >> texCoord.x >> texCoord.y;
//             texCoords.push_back(texCoord);
//         }
//         else if (lineHeader == "vn")
//         {
//             glm::vec3 normal{};
//             file >> normal.x >> normal.y >> normal.z;
//             normals.push_back(normal);
//         }
//         else if (lineHeader == "f")
//         {
//             if (m_FaceType == FaceType::Triangle)
//             {
//                 std::string faces[3u]{};
//                 file >> faces[0u] >> faces[1u] >> faces[2u];

//                 for (std::size_t i = 0u; i < 3u; ++i)
//                 {
//                     constexpr auto invalidValue{ static_cast<std::uint32_t>(-1) };
//                     const auto [vi, ti, ni]{ SplitFace(faces[i]) };

//                     data.push_back({
//                         vi == invalidValue ? glm::vec3{} : vertices.at(vi - 1u),
//                         ni == invalidValue ? glm::vec3{} : normals.at(ni - 1u),
//                         ti == invalidValue ? glm::vec2{} : texCoords.at(ti - 1u)
//                     });
//                 }
//             }
//             else if (m_FaceType == FaceType::Quad)
//             {
//                 OBJVertexData quad[4u]{};

//                 std::string faces[4u]{};
//                 file >> faces[0u] >> faces[1u] >> faces[2u] >> faces[3u];

//                 for (std::size_t i = 0u; i < 4u; ++i)
//                 {
//                     constexpr auto invalidValue{ static_cast<std::uint32_t>(-1) };
//                     const auto [vi, ti, ni]{ SplitFace(faces[i]) };

//                     quad[i] = {
//                         vi == invalidValue ? glm::vec3{} : vertices.at(vi - 1u),
//                         ni == invalidValue ? glm::vec3{} : normals.at(ni - 1u),
//                         ti == invalidValue ? glm::vec2{} : texCoords.at(ti - 1u)
//                     };
//                 }

//                 data.push_back(quad[2u]); data.push_back(quad[3u]); data.push_back(quad[1u]);
//                 data.push_back(quad[0u]); data.push_back(quad[1u]); data.push_back(quad[3u]);
//             }
//         }
//     }

//     return { data };
// }

// std::shared_ptr<Renderer::VertexArray> OBJLoader::LoadModel(const std::string& filepath)
// {
//     const auto vertexData{ OBJLoader::LoadData(filepath) };

    
// }
