/** Copyright Gabor Varga 2023 */

#include "metal/Core.hpp"
#include <iostream>
#include <catch2/catch_test_macros.hpp>
// #include <catch2/matchers/catch_matchers_floating_point.hpp>


// double sqrt( double x )
// {
//     return std::sqrt( x );
// }

constexpr double cube( double x )
{
    return x * x * x;
}


constexpr auto foo( auto x, auto y )
{
    return 2 * M_PI * sqrt( cube( x ) / y );
}


TEST_CASE( "Test basic expression" )
{
    SECTION( "Test basic stuff" )
    {
        // const metal::Double< "x" > x{ 1.0 };
        // const metal::Double< "y" > y{ 2.0 };

        DOUBLE( x, 1.0 );
        DOUBLE( y, 2.0 );
    
        const auto w = foo( 1.0, 2.0 );
        const auto z = foo( x, y );
        const auto dzdx = diff( z, x );
        const auto dzdy = diff( z, y );

        std::cout << z.str() << std::endl;
        std::cout << dzdx.str() << std::endl;
        std::cout << dzdy.str() << std::endl;

        std::cout << w << std::endl;
        std::cout << z.eval() << std::endl;
        std::cout << dzdx.eval() << std::endl;
        std::cout << dzdy.eval() << std::endl;
    }

    SECTION( "Test trigonometric functions" )
    {
        const metal::Double< "x" > x{ 1.0 };
        fmt::println( "{0} = {1}", x.str(), x.eval() );
        const auto xd = diff( x, x );
        fmt::println( "{0} = {1}", xd.str(), xd.eval() );

        const auto y = sin( x );
        fmt::println( "{0} = {1}", y.str(), y.eval() );
    
        const auto d1 = diff( y, x );
        fmt::println( "{0} = {1}", d1.str(), d1.eval() );

        const auto d2 = diff( d1, x );
        fmt::println( "{0} = {1}", d2.str(), d2.eval() );

        const auto d3 = diff( d2, x );
        fmt::println( "{0} = {1}", d3.str(), d3.eval() );

        const auto d4 = diff( d3, x );
        fmt::println( "{0} = {1}", d4.str(), d4.eval() );

        // using SMA = Variable< 0 >;
        // using GM = Variable< 1 >;
        // auto sma = SMA{};
        // auto gm = GM{};

        // const auto per = orbital_period( sma, gm );
        // const auto per_diff_sma = diff( per, sma );
        // const auto per_diff_gm = diff( per, gm );

        // std::cout << per.str() << std::endl;
        // std::cout << per_diff_sma.str() << std::endl;
        // std::cout << per_diff_gm.str() << std::endl;

        // const double eps = 0.1;
        // const std::tuple a{ 6628.14, 398600.44 };
        // std::cout << per.eval( a ) << std::endl;
        
        // const std::tuple a1{ 6628.14 + eps, 398600.44 };
        // const std::tuple a2{ 6628.14, 398600.44 + eps };
        // const auto y0 = per.eval( a );
        // const auto y1 = per.eval( a1 );
        // const auto y2 = per.eval( a2 );

        // std::cout << per.eval( a ) << std::endl;
        // std::cout << per_diff_sma.eval( a ) << std::endl;
        // std::cout << ( y1 - y0 ) / eps << std::endl;
        // std::cout << per_diff_gm.eval( a ) << std::endl;
        // std::cout << ( y2 - y0 ) / eps << std::endl;
    }
}
