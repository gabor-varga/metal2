/** Copyright Gabor Varga 2023 */

#include "Parameter.hpp"
#include "Util.hpp"

#include <array>
#include <vector>
#include <algorithm>
#include <fmt/core.h>


namespace metal
{

class ParameterNotFoundException : public std::runtime_error
{
public:
    ParameterNotFoundException( const Parameter& p )
        : std::runtime_error{ fmt::format( "Parameter not found, id={0}", p.id() ) }
    {
    }
};


// template< typename Derived >
// class GradientBase
// {
// public:
//     const typename Derived::Parameters& parameters() const
//     {
//         return static_cast< const Derived& >( *this ).parameters();
//     }

//     const typename Derived::T& at( const Parameter& p ) const
//     {
//         return static_cast< const Derived& >( *this ).parameters();
//     }

//     typename Derived::T& at( const Parameter& p )
//     {
//         return static_cast< const Derived& >( *this ).parameters();
//     }
// };


template< typename T_, int Size_ >
class Gradient// : public GradientBase< Gradient< T_, Size_ > >
{
public:
    using T = T_;
    static constexpr int Size = Size_;

    template< typename S >
    using Holder = std::conditional_t< Size == -1, std::vector< S >, std::array< S, Size > >;

    using Parameters = Holder< Parameter >;
    using Value = Holder< T >;

    Gradient() = default;

    Gradient( Parameters parameters, Value value )
        : parameters_{ std::move( parameters ) }
        , value_{ std::move( value ) }
    {
        // assumes parameters are ordered
    }

    const Parameters& parameters() const { return parameters_; }

    const T& at( const Parameter& p ) const
    {
        const auto iter = std::ranges::lower_bound( parameters_, p );
        check< ParameterNotFoundException >( iter != parameters_.end(), p );
        const auto id = std::distance( parameters_.begin(), iter );
        return value_[id];
    }

    T& at( const Parameter& p )
    {
        const auto iter = std::ranges::lower_bound( parameters_, p );
        check< ParameterNotFoundException >( iter != parameters_.end(), p );
        const auto id = std::distance( parameters_.begin(), iter );
        return value_[id];
    }

private:
    Parameters parameters_;
    Value value_;
};


// template< typename T, int Size, typename Op >
// class GradientBinaryOp
// {

// };

// template< typename T, int Size1, int Size2 >
// Gradient< T, Size > operator+( const Gradient< T, Size1 >& left, const Gradient< T, Size2 >& right )
// {

// }

} // namespace metal
