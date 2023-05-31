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
        using X = Double< 0 >;
        const X x{ 1.0 };
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

        using SMA = Double< 0 >;
        using GM = Double< 1 >;
        auto sma = SMA{ 6628.14 };
        auto gm = GM{ 398600.44 };

        const auto per = orbital_period( sma, gm );
        const auto per_diff_sma = diff( per, sma );
        const auto per_diff_gm = diff( per, gm );

        std::cout << per.str() << std::endl;
        std::cout << per_diff_sma.str() << std::endl;
        std::cout << per_diff_gm.str() << std::endl;

        const double eps = 0.1;
        // const std::tuple pars{ 1.5, 2.5 };
        // const double eps = 1e-6;
        std::cout << per.eval() << std::endl;
        
        const auto y0 = per.eval();

        sma.set( sma.eval() + eps );
        const auto y1 = per.eval();
        sma.set( sma.eval() - eps );
        gm.set( gm.eval() + eps );
        const auto y2 = per.eval();

        std::cout << per.eval() << std::endl;
        std::cout << per_diff_sma.eval() << std::endl;
        std::cout << ( y1 - y0 ) / eps << std::endl;
        std::cout << per_diff_gm.eval() << std::endl;
        std::cout << ( y2 - y0 ) / eps << std::endl;
    }
}
