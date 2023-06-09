/** Copyright Gabor Varga 2023 */

// Compile Time Symbolic Differentiation

#include <cmath>
#include <tuple>
#include <iostream>
#include <fmt/core.h>


template< typename Var, typename Input >
constexpr auto diff( Input input )
{
    return simplify( input.template deriv< Var >() );
}

template< typename Var, typename Input >
constexpr auto diff( Input input, Var )
{
    return diff< Var >( input );
}





template< typename Input >
constexpr auto simplify( Input input )
{
    return input;
}

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

// template< typename Input >
// constexpr auto simplify( Add< Input, Input > input )
// {
//     return Multiply{ Constant{ 2 }, input.left() };
// }

// template< typename Input, typename L, typename R >
// constexpr auto simplify( Add< Constant< L >, Input > left, Add< Constant< R >, Input > right )
// {
//     return Multiply{ Constant{ left.left().value() + right.right().value() }, left.right() };
// }

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

template< typename C1, typename C2 >
constexpr auto simplify( Multiply< Constant< C1 >, Constant< C2 > > input )
{
    return Constant{ input.left().value() * input.right().value() };
}

template< typename Left, typename Right, typename C >
constexpr auto simplify( Multiply< Left, Multiply< Constant< C >, Right > > input )
{
    return input.right().left() * input.left() * input.right().right();
}

template< typename Left, typename Right, typename C >
constexpr auto simplify( Multiply< Left, Multiply< Right, Constant< C > > > input )
{
    return input.right().right() * input.left() * input.right().left();
}

template< typename Input >
constexpr auto simplify( Negate< Negate< Input > > input )
{
    return input.input().input();
}


template< typename Input >
constexpr auto operator-( Input input )
{
    return simplify( Negate< Input >{ input } );
}

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

template< typename Left >
constexpr auto operator*( Left left, double right )
{
    return simplify( Multiply{ Constant{ right }, left } );
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

template< typename Input >
constexpr auto square( Input input )
{
    return simplify( Square{ input } );
}

template< typename Input >
constexpr auto cube( Input input )
{
    return simplify( Cube{ input } );
}

template< typename Input >
constexpr auto sqrt( Input input )
{
    return simplify( SquareRoot{ input } );
}

template< typename Input >
constexpr auto sin( Input input )
{
    return simplify( Sin{ input } );
}

template< typename Input >
constexpr auto cos( Input input )
{
    return simplify( Cos{ input } );
}

constexpr auto orbital_period( auto sma, auto gm )
{
    constexpr double TWOPI = 2 * M_PI;
    return Constant{ TWOPI } * sqrt( cube( sma ) / gm );
}

constexpr auto test_func( auto x, auto y )
{
    return sin( x ) + cos( y );
}
