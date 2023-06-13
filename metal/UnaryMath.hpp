/** Copyright Gabor Varga 2023 */

#ifndef METAL_UNARY_MATH_HPP
#define METAL_UNARY_MATH_HPP

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
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        return -input.eval();
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


struct SquareOp
{
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        const auto tmp = input.eval();
        return tmp * tmp;
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 2 } * input * diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "{0}^2", input.str() );
    }
};


struct CubeOp
{
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        const auto tmp = input.eval();
        return tmp * tmp * tmp;
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 3 } * square( input ) * diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "{0}^3", input.str() );
    }
};


struct SquareRootOp
{
    template< typename Input >
    static constexpr auto eval( Input input )
    {
        return std::sqrt( input.eval() );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return Constant{ 0.5 } / sqrt( input ) * diff< Var >( input );
    }

    template< typename Input >
    static constexpr std::string str( Input input )
    {
        return fmt::format( "sqrt({0})", input.str() );
    }
};

} // detail


template< typename Input >
class Negate : public UnaryOperator< Input, detail::NegateOp >
{
};

template< typename Input >
Negate( Input ) -> Negate< Input >;

template< typename Input >
class Square : public UnaryOperator< Input, detail::SquareOp >
{
};

template< typename Input >
Square( Input ) -> Square< Input >;

template< typename Input >
class Cube : public UnaryOperator< Input, detail::CubeOp >
{
};

template< typename Input >
Cube( Input ) -> Cube< Input >;

template< typename Input >
class SquareRoot : public UnaryOperator< Input, detail::SquareRootOp >
{
};

template< typename Input >
SquareRoot( Input ) -> SquareRoot< Input >;


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

// template <typename T>
// concept HasFunc1 = 
//   requires(T t) {
//       { t.eval() };
//   };

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

#endif
