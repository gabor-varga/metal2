/** Copyright Gabor Varga 2023 */

#ifndef METAL_UNARY_TRIGON_HPP
#define METAL_UNARY_TRIGON_HPP

#include "UnaryOperator.hpp"
#include "Common.hpp"
#include <tuple>
#include <cmath>
#include <string>
#include <fmt/core.h>


namespace metal
{

namespace detail
{

struct SinOp
{
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        return std::sin( input.eval() );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return cos( input ) * diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "sin({0})", input.str() );
    }
};

struct CosOp
{
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        return std::cos( input.eval() );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return -sin( input ) * diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "cos({0})", input.str() );
    }
};

} // detail


template< typename Input >
struct Sin : UnaryOperator< Input, detail::SinOp >
{
};

template< typename Input >
Sin( Input ) -> Sin< Input >;

template< typename Input >
struct Cos : UnaryOperator< Input, detail::CosOp >
{
};

template< typename Input >
Cos( Input ) -> Cos< Input >;


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

} // metal

#endif
