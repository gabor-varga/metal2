/** Copyright Gabor Varga 2023 */

#ifndef METAL_CONSTANT_HPP
#define METAL_CONSTANT_HPP

#include <tuple>
#include <string>
#include <fmt/core.h>
#include <cmath>


namespace metal
{

struct Zero
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 0;
    }

    std::string str() const { return "Zero"; }
};


struct One
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 1;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return "One"; }
};


struct Pi
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return M_PI;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return "Pi"; }
};


struct TwoPi
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 2 * M_PI;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return "TwoPi"; }
};


template< typename T >
class Constant
{
public:
    constexpr Constant( T value )
        : value_{ value }
    {
    }

    const auto value() const { return value_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return value_;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return fmt::format( "{0}", value_ ); }

private:
    T value_;
};

} // metal

#endif
