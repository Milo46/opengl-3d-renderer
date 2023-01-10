#pragma once

struct Bindable
{
    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;
};
