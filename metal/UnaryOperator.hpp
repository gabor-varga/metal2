/** Copyright Gabor Varga 2023 */

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

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return Operator::eval( input_, args );
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
