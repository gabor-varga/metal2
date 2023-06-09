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

static constexpr auto EmptyStringLiteral = StringLiteral{ "" };

}


template< int ID_, typename Value, detail::StringLiteral Name_ = detail::EmptyStringLiteral >
class Variable
{
public:
    static constexpr int ID = ID_;
    static constexpr detail::StringLiteral Name = Name_;

    explicit Variable( Value value )
        : value_{ value }
    {
        if constexpr ( Name.Size == 1 )
        {
            name_ = fmt::format( "Var_{0}", ID );
        }
        else
        {
            name_ = Name.value;
        }
    }


    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        if constexpr ( ID > std::tuple_size_v< std::tuple< Args... > > )
        {
            return value_;
        }
        else
        {
            return std::get< ID >( args );
        }
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        if constexpr ( Var::ID == ID )
        {
            return One{};
        }
        else
        {
            return Zero{};
        }
    }

    std::string str() const { return name_; }

private:
    std::string name_;
    Value value_;

};

} // metal

#endif
