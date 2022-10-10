#pragma once

struct NonConstructible
{
    NonConstructible()  = delete;
    ~NonConstructible() = delete;
};
