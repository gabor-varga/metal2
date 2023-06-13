/** Copyright Gabor Varga 2023 */

#ifndef METAL_UNARY_MATH_OPERATOR_HPP
#define METAL_UNARY_MATH_OPERATOR_HPP

#include <tuple>


template< typename Input, typename Operator >
class UnaryOperator
{
public:
    constexpr UnaryOperator( Input input )
        : input_{ input }
    {
    }

    constexpr auto input() const { return input_; }

    constexpr auto eval() const
    {
        return Operator::eval( input_ );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Operator::template deriv< Var >( input_ );
    }

    std::string str() const { return Operator::str( input_ ); }

private:
    Input input_;
};

#endif
