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

        // // using A = typename Simplify< Multiply< Constant< 2 >, Constant< 3 > > >::Value;
        // using A = typename Simplify< Multiply< Constant< 2 >, Constant< 3 > > >::Result;
        // std::cout << A::eval( p ) << std::endl;
        // std::cout << A::Deriv< X >::eval( p ) << std::endl;
        // std::cout << typeid( A{} ).name() << std::endl;
        // std::cout << typeid( A::Deriv< X >{} ).name() << std::endl;

        // const auto x = X{};
        // const auto y = -x;
        // using Y2 = decltype( y );
        // std::cout << Y2::eval( p ) << std::endl;
        // using Z2 = decltype( diff( y, x ) );
        // std::cout << Z2::eval( p ) << std::endl;

        using SMA = Variable< 0 >;
        using GM = Variable< 1 >;
        constexpr auto sma = SMA{};
        constexpr auto gm = GM{};
        // constexpr auto per = test_func( sma, gm );
        constexpr auto per = orbital_period( sma, gm );
        constexpr auto per_diff_sma = diff( per, sma );
        constexpr auto per_diff_gm = diff( per, gm );

        using Period = decltype( per );
        using PeriodDiffSma = decltype( per_diff_sma );
        using PeriodDiffGm = decltype( per_diff_gm );

        std::cout << typeid( Period{} ).name() << std::endl;
        std::cout << typeid( PeriodDiffSma{} ).name() << std::endl;
        std::cout << typeid( PeriodDiffGm{} ).name() << std::endl;

        const std::tuple pars{ 6628.14, 398600.44 };
        const double eps = 0.1;
        // const std::tuple pars{ 1.5, 2.5 };
        // const double eps = 1e-6;
        std::cout << 2 * M_PI * Period::eval( pars ) << std::endl;
        
        std::cout << 2 * M_PI * PeriodDiffSma::eval( pars ) << std::endl;
        const std::tuple pars2{ std::get< 0 >( pars ) + eps, std::get< 1 >( pars ) };
        std::cout << 2 * M_PI * ( Period::eval( pars2 ) - Period::eval( pars ) ) / eps << std::endl;

        std::cout << 2 * M_PI * PeriodDiffGm::eval( pars ) << std::endl;
        const std::tuple pars3{ std::get< 0 >( pars ), std::get< 1 >( pars ) + eps };
        std::cout << 2 * M_PI * ( Period::eval( pars3 ) - Period::eval( pars ) ) / eps << std::endl;
    }
}
