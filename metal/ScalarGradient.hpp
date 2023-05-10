/** Copyright Gabor Varga 2023 */

#include "Parameter.hpp"


namespace metal
{

template< typename T >
class Gradient
{
public:
    using Parameters = std::vector< Parameter >;

    Gradient( Parameters parameters, T value )
        : parameters_{ std::move( parameters ) }
        , value_{ std::move( value ) }
    {
        // assumes parameters are ordered
    }

private:
    Parameters parameters_;
    T value_;
};


// template< typename... Parameters >
// class Gradient
// {
// public:

// private:

// };

} // namespace metal
