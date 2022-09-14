#include <utility>
#include <concepts>


namespace metal
{

template< typename Value, typename Deriv >
class Dual
{
public:
    template< typename T, typename V >
    Dual( T&& value, V&& deriv ) requires
        std::is_same_v< std::remove_cvref_t< T >, Value > && std::is_same_v< std::remove_cvref_t< V >, Deriv >
        : value_{ std::move( value ) }, deriv_{ std::move( deriv ) }
    {
    }

    // Dual( Dual&& other )
    //     : value_{ std::move( other.value_ ) }
    //     , deriv_{ std::move( other.deriv_ ) }
    // {
    // }

private:
    Value value_;
    Deriv deriv_;
};

}
