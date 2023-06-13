/** Copyright Gabor Varga 2023 */

#ifndef METAL_VARIABLE_HPP
#define METAL_VARIABLE_HPP

#include "Constant.hpp"
#include <tuple>
#include <string>
#include <algorithm>
#include <fmt/core.h>


namespace metal
{

namespace detail
{

template< size_t N >
struct StringLiteral
{
    static constexpr auto Size = N;

    constexpr StringLiteral( const char ( &str )[N] ) { std::copy_n( str, N, value ); }

    constexpr bool operator<=>( const StringLiteral& other ) const = default;

    char value[N];
};

// template< size_t N1, size_t N2 >
// constexpr bool operator==( const StringLiteral< N1 >& left, const StringLiteral< N1 >& right )
// {
//     if constexpr ( N1 != N2 )
//     {
//         return false;
//     }
//     else
//     {
//         return left == right;
//     }
// }

}


template< detail::StringLiteral Name_, typename Value >
class Variable
{
public:
    static constexpr detail::StringLiteral Name = Name_;

    explicit Variable( Value value )
        : value_{ value }
    {
    }

    constexpr auto eval() const { return value_; }

    template< typename Var >
    constexpr auto deriv() const
    {
        if constexpr ( Var::Name == Name )
        {
            return One{};
        }
        else
        {
            return Zero{};
        }
    }

    std::string str() const { return Name.value; }

private:
    Value value_;
};

template< detail::StringLiteral Name >
using Double = Variable< Name, double >;

#define DOUBLE( name, value ) metal::Double< #name > name{ value };

} // metal

#endif
