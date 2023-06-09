/** Copyright Gabor Varga 2023 */

#ifndef METAL_COMMON_HPP
#define METAL_COMMON_HPP


namespace metal
{

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

} // metal

#endif
