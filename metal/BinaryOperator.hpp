/** Copyright Gabor Varga 2023 */

#ifndef METAL_BINARY_OPERATOR_HPP
#define METAL_BINARY_OPERATOR_HPP

#include <tuple>


template< typename Left, typename Right, typename Operator >
class BinaryOperator
{
public:
    constexpr BinaryOperator( Left left, Right right )
        : left_{ left }
        , right_{ right }
    {
    }

    constexpr auto left() const { return left_; }
    constexpr auto right() const { return right_; }

    constexpr auto eval() const
    {
        return Operator::eval( left_, right_ );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Operator::template deriv< Var >( left_, right_ );
    }

    std::string str() const { return Operator::str( left_, right_ ); }

private:
    Left left_;
    Right right_;
};

#endif
