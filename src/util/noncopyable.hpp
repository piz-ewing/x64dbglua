#pragma once

namespace util
{
class Noncopyable
{
public:
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;

protected:
    Noncopyable() = default;
    ~Noncopyable() noexcept = default;
};

} // namespace util
