#ifndef SQLEGER_BIND_TRAITS_HPP
#define SQLEGER_BIND_TRAITS_HPP

#include <sqleger/parameter.hpp>
#include <sqleger/sentinels.hpp>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace sqleger {


template <typename T>
struct bind_traits;

template <>
struct bind_traits<user_blob> {
  static inline result bind(parameter p, user_blob const& value) noexcept;
};

template <>
struct bind_traits<double> {
  static inline result bind(parameter p, double value) noexcept;
};

template <>
struct bind_traits<int> {
  static inline result bind(parameter p, int value) noexcept;
};

template <>
struct bind_traits<int64> {
  static inline result bind(parameter p, int64 value) noexcept;
};

template <>
struct bind_traits<null_t> {
  static inline result bind(parameter p, null_t) noexcept;
};

template <>
struct bind_traits<skip_t> {
  static inline result bind(parameter p, skip_t) noexcept;
};

template <>
struct bind_traits<user_text> {
  static inline result bind(parameter p, user_text const& value) noexcept;
};

template <typename T, typename Allocator>
struct bind_traits<std::vector<T, Allocator>> {
  static inline result bind(parameter p,
                            std::vector<T, Allocator> const& value) noexcept;
};

template <typename T, std::size_t N>
struct bind_traits<std::array<T, N>> {
  static inline result bind(parameter p,
                            std::array<T, N> const& value) noexcept;
};

template <>
struct bind_traits<float> {
  static inline result bind(parameter p, float value) noexcept;
};

template <>
struct bind_traits<short> {
  static inline result bind(parameter p, short value) noexcept;
};

template <>
struct bind_traits<unsigned short> {
  static inline result bind(parameter p, unsigned short value) noexcept;
};

template <>
struct bind_traits<unsigned int> {
  static inline result bind(parameter p, unsigned int value) noexcept;
};

template <>
struct bind_traits<long> {
  static inline result bind(parameter p, long value) noexcept;
};

template <>
struct bind_traits<unsigned long> {
  static inline result bind(parameter p, unsigned long value) noexcept;
};

template <>
struct bind_traits<bool> {
  static inline result bind(parameter p, bool value) noexcept;
};

template <typename T>
struct bind_traits<std::optional<T>> {
  static inline result bind(parameter p,
                            std::optional<T> const& value) noexcept;
};

template <>
struct bind_traits<char> {
  static inline result bind(parameter p, char value) noexcept;
};

template <>
struct bind_traits<char const*> {
  static inline result bind(parameter p, char const* value) noexcept;
};

template <>
struct bind_traits<std::string> {
  static inline result bind(parameter p, std::string const& value) noexcept;
};

template <>
struct bind_traits<std::string_view> {
  static inline result bind(parameter p, std::string_view value) noexcept;
};


// =============================================================================


result bind_traits<user_blob>::bind(parameter p,
                                    user_blob const& value) noexcept
{
  return p.bind_blob(value);
}

result bind_traits<double>::bind(parameter p, double const value) noexcept
{
  return p.bind_double(value);
}

result bind_traits<int>::bind(parameter p, int const value) noexcept
{
  return p.bind_int(value);
}

result bind_traits<int64>::bind(parameter p, int64 const value) noexcept
{
  return p.bind_int64(value);
}

result bind_traits<null_t>::bind(parameter p, null_t) noexcept
{
  return p.bind_null();
}

result bind_traits<skip_t>::bind(parameter p, skip_t) noexcept
{
  return result::ok;
}

result bind_traits<user_text>::bind(parameter p,
                                    user_text const& value) noexcept
{
  return p.bind_text(value);
}

template <typename T, typename Allocator>
result bind_traits<std::vector<T, Allocator>>::bind(
  parameter p,
  std::vector<T, Allocator> const& value) noexcept
{
  return p.bind_blob(user_blob(value));
}

template <typename T, std::size_t N>
result
bind_traits<std::array<T, N>>::bind(parameter p,
                                    std::array<T, N> const& value) noexcept
{
  return p.bind_blob(user_blob(value));
}

result bind_traits<float>::bind(parameter p, float const value) noexcept
{
  return p.bind_double(static_cast<double>(value));
}

result bind_traits<short>::bind(parameter p, short const value) noexcept
{
  return p.bind_int(static_cast<int>(value));
}

result bind_traits<unsigned short>::bind(parameter p,
                                         unsigned short const value) noexcept
{
  return p.bind_int(static_cast<int>(value));
}

result bind_traits<unsigned int>::bind(parameter p,
                                       unsigned int const value) noexcept
{
  return p.bind_int64(static_cast<int64>(value));
}

result bind_traits<long>::bind(parameter p, long const value) noexcept
{
  return p.bind_int64(static_cast<int64>(value));
}

result bind_traits<unsigned long>::bind(parameter p,
                                        unsigned long const value) noexcept
{
  return p.bind_int64(static_cast<int64>(value));
}

result bind_traits<bool>::bind(parameter p, bool const value) noexcept
{
  return p.bind_int(static_cast<int>(value));
}

template <typename T>
result
bind_traits<std::optional<T>>::bind(parameter p,
                                    std::optional<T> const& value) noexcept
{
  return value ? bind_traits<T>::bind(p, *value) : p.bind_null();
}

result bind_traits<char>::bind(parameter p, char const value) noexcept
{
  return p.bind_text(string_span(&value, 1));
}

result bind_traits<char const*>::bind(parameter p,
                                      char const* const value) noexcept
{
  return p.bind_text(string_span(value));
}

result bind_traits<std::string>::bind(parameter p,
                                      std::string const& value) noexcept
{
  return p.bind_text(string_span(value));
}

result
bind_traits<std::string_view>::bind(parameter p,
                                    std::string_view const value) noexcept
{
  return p.bind_text(string_span(value));
}


} // namespace sqleger


#endif
