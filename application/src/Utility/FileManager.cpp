#include "FileManager.hpp"

#include <fstream>
#include <sstream>

const std::string FileManager::c_NoContent{ "(no-content)" };

std::optional<std::string> FileManager::ReadContent(const std::string_view filepath) noexcept
{
    const std::ifstream file{ filepath.data() };

    static std::stringstream buffer{};
    buffer.str({});
    buffer << file.rdbuf();

    const auto content{ buffer.str() };
    return !content.empty() ? std::make_optional(content) : std::nullopt;
}

FileManager::FileManager(const std::string_view filepath, const bool load)
    : m_Path{ filepath },
      m_Content{ load ? FileManager::InternalRead(filepath) : "" },
      m_Loaded{ load }
{}

bool FileManager::Load() noexcept
{
    if (const auto content{ FileManager::ReadContent(m_Path) })
    {
        m_Content = content.value();
        m_Loaded  = true;
    }
    else
    {
        m_Content = FileManager::c_NoContent;
        m_Loaded  = false;
    }

    return m_Loaded;
}

bool FileManager::Load(const std::string_view filepath, const bool revertOnFail) noexcept
{
    const auto pathBackup{ m_Path };
    const auto contentBackup{ m_Content };

    if (const auto content{ FileManager::ReadContent(filepath) })
    {
        m_Content = content.value();
        m_Loaded  = true;
    }
    else
    {
        if (revertOnFail)
        {
            m_Path = pathBackup;
            m_Content = contentBackup;
            return false;
        }

        m_Content = FileManager::c_NoContent;
        m_Loaded  = false;
    }

    return m_Loaded;
}
