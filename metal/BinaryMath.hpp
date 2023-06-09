/** Copyright Gabor Varga 2023 */

#include "BinaryOperator.hpp"
#include "Common.hpp"
#include "Constant.hpp"
#include <cmath>
#include <fmt/core.h>


namespace metal
{

namespace detail
{

struct AddOp
{
    template< typename Left, typename Right, typename... Args >
    static constexpr auto eval( Left left, Right right, std::tuple< Args... > args )
    {
        return left.eval( args ) + right.eval( args );
    }

    template< typename Var, typename Left, typename Right >
    static constexpr auto deriv( Left left, Right right )
    {
        return diff< Var >( left ) + diff< Var >( right );
    }

    template< typename Left, typename Right >
    static constexpr std::string str( Left left, Right right )
    {
        return fmt::format( "({0} + {1})", left.str(), right.str() );
    }
};

struct SubtractOp
{
    template< typename Left, typename Right, typename... Args >
    static constexpr auto eval( Left left, Right right, std::tuple< Args... > args )
    {
        return left.eval( args ) - right.eval( args );
    }

    template< typename Var, typename Left, typename Right >
    static constexpr auto deriv( Left left, Right right )
    {
        return diff< Var >( left ) - diff< Var >( right );
    }

    template< typename Left, typename Right >
    static constexpr std::string str( Left left, Right right )
    {
        return fmt::format( "({0} - {1})", left.str(), right.str() );
    }
};

struct MultiplyOp
{
    template< typename Left, typename Right, typename... Args >
    static constexpr auto eval( Left left, Right right, std::tuple< Args... > args )
    {
        return left.eval( args ) * right.eval( args );
    }

    template< typename Var, typename Left, typename Right >
    static constexpr auto deriv( Left left, Right right )
    {
        return diff< Var >( left ) * right + diff< Var >( right ) * left;
    }

    template< typename Left, typename Right >
    static constexpr std::string str( Left left, Right right )
    {
        return fmt::format( "({0} * {1})", left.str(), right.str() );
    }
};

struct DivideOp
{
    template< typename Left, typename Right, typename... Args >
    static constexpr auto eval( Left left, Right right, std::tuple< Args... > args )
    {
        return left.eval( args ) / right.eval( args );
    }

    template< typename Var, typename Left, typename Right >
    static constexpr auto deriv( Left left, Right right )
    {
        return ( diff< Var >( left ) * right - diff< Var >( right ) * left ) / square( right );
    }

    template< typename Left, typename Right >
    static constexpr std::string str( Left left, Right right )
    {
        return fmt::format( "({0} / {1})", left.str(), right.str() );
    }
};

} // detail


template< typename Left, typename Right >
using Add = BinaryOperator< Left, Right, detail::AddOp >;

template< typename Left, typename Right >
using Subtract = BinaryOperator< Left, Right, detail::SubtractOp >;

template< typename Left, typename Right >
using Multiply = BinaryOperator< Left, Right, detail::MultiplyOp >;

template< typename Left, typename Right >
using Divide = BinaryOperator< Left, Right, detail::DivideOp >;

// Simplify rules

template< typename Left >
constexpr auto simplify( Add< Left, Zero > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Add< Zero, Right > input )
{
    return input.right();
}

template< typename Left >
constexpr auto simplify( Subtract< Left, Zero > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Subtract< Zero, Right > input )
{
    return -input.right();
}

template< typename Left >
constexpr auto simplify( Multiply< Left, Zero > )
{
    return Zero{};
}

template< typename Right >
constexpr auto simplify( Multiply< Zero, Right > )
{
    return Zero{};
}

template< typename Left >
constexpr auto simplify( Multiply< Left, One > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Multiply< One, Right > input )
{
    return input.right();
}

// Operators

template< typename Left, typename Right >
constexpr auto operator+( Left left, Right right )
{
    return simplify( Add{ left, right } );
}

// template< typename Left >
// constexpr auto operator+( Left left, int right )
// {
//     return simplify( Add{ left, Constant{ right } } );
// }

// template< typename Left >
// constexpr auto operator+( Left left, double right )
// {
//     return simplify( Add{ left, Constant{ right } } );
// }

template< typename Left, typename Right >
constexpr auto operator-( Left left, Right right )
{
    return simplify( Subtract{ left, right } );
}

template< typename Left, typename Right >
constexpr auto operator*( Left left, Right right )
{
    return simplify( Multiply{ left, right } );
}

// template< typename Left >
// constexpr auto operator*( Left left, double right )
// {
//     return simplify( Multiply{ Constant{ right }, left } );
// }

// template< typename Right >
// constexpr auto operator*( double left, Right right )
// {
//     return simplify( Multiply{ Constant{ left }, right } );
}

template< typename Left, typename Right >
constexpr auto operator/( Left left, Right right )
{
    return simplify( Divide{ left, right } );
}

} // metal
