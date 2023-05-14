/** Copyright Gabor Varga 2023 */

#include "metal/ScalarGradient.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>


TEST_CASE( "Test Gradient construction id" )
{
    SECTION( "Test constructor increases id" )
    {
        metal::Parameter p{};
        std::array< double, 1 > v{ 2.0 };
        metal::Gradient< double, 1 > g{ { p }, v };

        REQUIRE( g.parameters() == std::array< metal::Parameter, 1 >{ p } );

        REQUIRE_THAT( 2.0, Catch::Matchers::WithinULP( g.at( p ), 0 ) );

        g.at( p ) = 1.5;
        REQUIRE_THAT( 1.5, Catch::Matchers::WithinULP( g.at( p ), 0 ) );
    }
}
