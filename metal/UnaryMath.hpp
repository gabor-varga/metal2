/** Copyright Gabor Varga 2023 */

#include "UnaryOperator.hpp"
#include "Common.hpp"
#include <cmath>
#include <fmt/core.h>


namespace metal
{

namespace detail
{

struct NegateOp
{
    template< typename Input, typename... Args >
    static constexpr auto eval( Input input, std::tuple< Args... > args )
    {
        return -input.eval( args );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return -diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "(-{0})", input.str() );
    }
};


template< typename Input >
struct SquareOp
{
    template< typename Input, typename... Args >
    static constexpr auto eval( Input input, std::tuple< Args... > args )
    {
        const auto tmp = input_.eval( args );
        return tmp * tmp;
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 2 } * input_ * diff< Var >( input_ );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "{0}^2", input.str() );
    }
};


template< typename Input >
struct CubeOp
{
    template< typename Input, typename... Args >
    static constexpr auto eval( Input input, std::tuple< Args... > args )
    {
        const auto tmp = input_.eval( args );
        return tmp * tmp * tmp;
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 3 } * square( input_ ) * diff< Var >( input_ );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "{0}^3", input.str() );
    }
};


template< typename Input >
struct SquareRootOp
{
    template< typename Input, typename... Args >
    static constexpr auto eval( Input input, std::tuple< Args... > args )
    {
        return std::sqrt( input_.eval( args ) );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 0.5 } / sqrt( input_ ) * diff< Var >( input_ );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "sqrt({0})", input.str() );
    }
};

} // detail


template< typename Input >
using Negate = UnaryOperator< Input, NegateOp >;

template< typename Input >
using Square = UnaryOperator< Input, SquareOp >;

template< typename Input >
using Cube = UnaryOperator< Input, CubeOp >;

template< typename Input >
using SquareRoot = UnaryOperator< Input, SquareRootOp >;


template< typename Input >
constexpr auto operator-( Input input )
{
    return simplify( Negate< Input >{ input } );
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
constexpr auto simplify( Negate< Negate< Input > > input )
{
    return input.input().input();
}

} // metal
