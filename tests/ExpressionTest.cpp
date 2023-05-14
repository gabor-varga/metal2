/** Copyright Gabor Varga 2023 */

#include "metal/Expression.hpp"

#include <catch2/catch_test_macros.hpp>
// #include <catch2/matchers/catch_matchers_floating_point.hpp>


consteval auto foo( auto x )
{
    return x;
}


TEST_CASE( "Test basic expression" )
{
    SECTION( "Test trigonometric functions" )
    {
        using X = Variable< 0 >;
        using Y = Sin< X >;
        const std::tuple p{ 1.0, 2.0 };
        // REQUIRE_THAT( std::sin( 1.0 ), Catch::Matchers::WithinULP( Y::eval( p ), 0 ) );
        std::cout << Y::eval( p ) << std::endl;
        using Z = Y::Deriv< X >;
        // using Z = Negate< Cos< X > >;
        std::cout << Z::eval( p ) << std::endl;
        // REQUIRE_THAT( std::cos( 1.0 ), Catch::Matchers::WithinULP( Z::eval( p ), 0 ) );
        using W = Z::Deriv< X >;
        std::cout << W::eval( p ) << std::endl;
        using U = W::Deriv< X >;
        std::cout << U::eval( p ) << std::endl;
        using V = U::Deriv< X >;
        std::cout << V::eval( p ) << std::endl;
        std::cout << typeid( X{} ).name() << std::endl;
        std::cout << typeid( Y{} ).name() << std::endl;
        std::cout << typeid( Z{} ).name() << std::endl;
        std::cout << typeid( W{} ).name() << std::endl;
        std::cout << typeid( U{} ).name() << std::endl;
        std::cout << typeid( V{} ).name() << std::endl;

        // using A = typename Simplify< Multiply< Constant< 2 >, Constant< 3 > > >::Value;
        using A = typename Simplify< Multiply< Constant< 2 >, Constant< 3 > > >::Result;
        std::cout << A::eval( p ) << std::endl;
        std::cout << A::Deriv< X >::eval( p ) << std::endl;
        std::cout << typeid( A{} ).name() << std::endl;
        std::cout << typeid( A::Deriv< X >{} ).name() << std::endl;

        const auto x = X{};
        const auto y = -x;
        using Y2 = decltype( y );
        std::cout << Y2::eval( p ) << std::endl;
        using Z2 = decltype( diff( y, x ) );
        std::cout << Z2::eval( p ) << std::endl;
    }
}
