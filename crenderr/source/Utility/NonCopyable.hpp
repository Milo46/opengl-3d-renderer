#pragma once

/**
 * Source: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
 */
template<typename _Ty>
class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    _Ty& operator=(const _Ty&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};
