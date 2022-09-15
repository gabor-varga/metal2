#include <utility>
#include <concepts>


namespace metal
{

template< typename Value, typename Deriv >
class Dual
{
public:
    // template< typename T, typename V >
    // Dual( T&& value, V&& deriv ) requires
    //     std::is_same_v< std::remove_cvref_t< T >, Value > && std::is_same_v< std::remove_cvref_t< V >, Deriv >
    //     : value_{ std::move( value ) }, deriv_{ std::move( deriv ) }
    // {
    // }

    Dual( const Value& value, const Deriv& deriv )
        : value_{ value }
        , deriv_{ deriv }
    {
    }

    Dual( const Value& value, Deriv&& deriv )
        : value_{ value }
        , deriv_{ std::move( deriv ) }
    {
    }

    Dual( Value&& value, const Deriv& deriv )
        : value_{ std::move( value ) }
        , deriv_{ deriv }
    {
    }

    Dual( Value&& value, Deriv&& deriv )
        : value_{ std::move( value ) }
        , deriv_{ std::move( deriv ) }
    {
    }

    template< typename T, typename V >
    Dual( T value, V deriv ) requires
        (!std::is_same_v< std::remove_cvref_t< T >, Value >) && (!std::is_same_v< std::remove_cvref_t< V >, Deriv >)
        : value_{ value }
        , deriv_{ deriv }
    {
    }

    template< typename T, typename V >
    Dual( const Dual< T, V >& other )
        : value_{ other.value() }
        , deriv_{ other.deriv() }
    {
    }

    // Dual( Dual&& other )
    //     : value_{ std::move( other.value_ ) }
    //     , deriv_{ std::move( other.deriv_ ) }
    // {
    // }

    const Value& value() const { return value_; }
    const Deriv& deriv() const { return deriv_; }

    template< typename V1, typename D1, typename V2, typename D2 >
    friend auto operator+( const Dual< V1, D1 >& x, const Dual< V2, D2 >& y );

    // template< typename V1, typename D1, typename V2, typename D2 >
    // friend auto operator+( Dual< V1, D1 >&& x, Dual< V2, D2 >&& y );

private:
    Value value_;
    Deriv deriv_;
};

template< typename V1, typename D1, typename V2, typename D2 >
auto operator+( const Dual< V1, D1 >& x, const Dual< V2, D2 >& y )
{
    using V3 = decltype( std::declval< V1 >() + std::declval< V2 >() );
    using D3 = decltype( std::declval< D1 >() + std::declval< D2 >() );
    return Dual< V3, D3 >( x.value_ + y.value_, x.deriv_ + y.deriv_ );
}

// template< typename V1, typename D1, typename V2, typename D2 >
// auto operator+( Dual< V1, D1 >&& x, Dual< V2, D2 >&& y )
// {
//     using V3 = decltype( std::declval< V1 >() + std::declval< V2 >() );
//     using D3 = decltype( std::declval< D1 >() + std::declval< D2 >() );
//     return Dual< V3, D3 >( x.value_ + y.value_, x.deriv_ + y.deriv_ );
// }

}
