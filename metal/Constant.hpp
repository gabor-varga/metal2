/** Copyright Gabor Varga 2023 */

#include <tuple>
#include <string>
#include <fmt/core.h>


namespace metal
{

struct Zero
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 0;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        // static_assert(false, "Can not differentiate Zero");
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
