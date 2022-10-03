#pragma once

#include <optional>
#include <string>

class FileManager
{
public:
    static const std::string c_NoContent;

public:
    static std::optional<std::string> ReadContent(const std::string_view filepath) noexcept;

public:
    explicit FileManager() = default;
    explicit FileManager(const std::string_view filepath, const bool load = true);

    bool Load() noexcept;
    bool Load(const std::string_view filepath, const bool revertOnFail = true) noexcept;

public:
    inline const std::string GetPath() const noexcept    { return m_Path; }
    inline const std::string GetContent() const noexcept { return m_Content; }
    inline const bool IsLoaded() const noexcept          { return m_Loaded; }

    inline operator bool() noexcept
    {
        return (m_Loaded && m_Content != FileManager::c_NoContent);
    }

private:
    inline const std::string InternalRead(const std::string_view filepath) noexcept
    {
        return FileManager::ReadContent(filepath).value_or(FileManager::c_NoContent);
    }

private:
    std::string m_Path;
    std::string m_Content;
    bool m_Loaded;
};
