#pragma once

#include "Utility/NonConstructible.hpp"

#include <string>

class Filesystem : public NonConstructible
{
public:
    static bool Initialize() noexcept;
};
