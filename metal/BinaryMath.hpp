/** Copyright Gabor Varga 2023 */

#ifndef METAL_BINARY_MATH_HPP
#define METAL_BINARY_MATH_HPP

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
    template< typename Left, typename Right >
    static constexpr auto eval( Left left, Right right )
    {
        return left.eval() + right.eval();
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
    template< typename Left, typename Right >
    static constexpr auto eval( Left left, Right right )
    {
        return left.eval() - right.eval();
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
    template< typename Left, typename Right >
    static constexpr auto eval( Left left, Right right )
    {
        return left.eval() * right.eval();
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
    template< typename Left, typename Right >
    static constexpr auto eval( Left left, Right right )
    {
        return left.eval() / right.eval();
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
class Add : public BinaryOperator< Left, Right, detail::AddOp >
{
public:
    Add( Left left, Right right )
        : BinaryOperator< Left, Right, detail::AddOp >{ left, right }
    {
    }
};

template< typename Left, typename Right >
class Subtract : public BinaryOperator< Left, Right, detail::SubtractOp >
{
public:
    Subtract( Left left, Right right )
        : BinaryOperator< Left, Right, detail::SubtractOp >{ left, right }
    {
    }
};

template< typename Left, typename Right >
class Multiply : public BinaryOperator< Left, Right, detail::MultiplyOp >
{
public:
    Multiply( Left left, Right right )
        : BinaryOperator< Left, Right, detail::MultiplyOp >{ left, right }
    {
    }
};

template< typename Left, typename Right >
class Divide : public BinaryOperator< Left, Right, detail::DivideOp >
{
public:
    Divide( Left left, Right right )
        : BinaryOperator< Left, Right, detail::DivideOp >{ left, right }
    {
    }
};

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

template< typename Left >
constexpr auto operator+( Left left, int right )
{
    return simplify( Add{ left, Constant{ right } } );
}

template< typename Left >
constexpr auto operator+( Left left, double right )
{
    return simplify( Add{ left, Constant{ right } } );
}

template< typename Right >
constexpr auto operator+( int left, Right right )
{
    return simplify( Add{ Constant{ left }, right } );
}

template< typename Right >
constexpr auto operator+( double left, Right right )
{
    return simplify( Add{ Constant{ left }, right } );
}

template< typename Left, typename Right >
constexpr auto operator-( Left left, Right right )
{
    return simplify( Subtract{ left, right } );
}

template< typename Left >
constexpr auto operator-( Left left, int right )
{
    return simplify( Subtract{ left, Constant{ right } } );
}

template< typename Left >
constexpr auto operator-( Left left, double right )
{
    return simplify( Subtract{ left, Constant{ right } } );
}

template< typename Right >
constexpr auto operator-( int left, Right right )
{
    return simplify( Subtract{ Constant{ left }, right } );
}

template< typename Right >
constexpr auto operator-( double left, Right right )
{
    return simplify( Subtract{ Constant{ left }, right } );
}

template< typename Left, typename Right >
constexpr auto operator*( Left left, Right right )
{
    return simplify( Multiply{ left, right } );
}

template< typename Left >
constexpr auto operator*( Left left, int right )
{
    return simplify( Multiply{ left, Constant{ right } } );
}

template< typename Left >
constexpr auto operator*( Left left, double right )
{
    return simplify( Multiply{ left, Constant{ right } } );
}

template< typename Right >
constexpr auto operator*( int left, Right right )
{
    return simplify( Multiply{ Constant{ left }, right } );
}

template< typename Right >
constexpr auto operator*( double left, Right right )
{
    return simplify( Multiply{ Constant{ left }, right } );
}

template< typename Left, typename Right >
constexpr auto operator/( Left left, Right right )
{
    return simplify( Divide{ left, right } );
}

template< typename Left >
constexpr auto operator/( Left left, int right )
{
    return simplify( Divide{ left, Constant{ right } } );
}

template< typename Left >
constexpr auto operator/( Left left, double right )
{
    return simplify( Divide{ left, Constant{ right } } );
}

template< typename Right >
constexpr auto operator/( int left, Right right )
{
    return simplify( Divide{ Constant{ left }, right } );
}

template< typename Right >
constexpr auto operator/( double left, Right right )
{
    return simplify( Divide{ Constant{ left }, right } );
}

} // metal

#endif