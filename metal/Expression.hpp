/** Copyright Gabor Varga 2023 */

// Compile Time Symbolic Differentiation

#include <cmath>
#include <tuple>
#include <iostream>
#include <fmt/core.h>


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
