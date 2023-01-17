#pragma once

#include "Utility/NonConstructible.hpp"

class Filesystem : public NonConstructible
{
public:
    static bool Initialize() noexcept;
};
