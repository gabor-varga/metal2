/** Copyright Gabor Varga 2023 */

#include "metal/Parameter.hpp"

#include <thread>
#include <catch2/catch_test_macros.hpp>


TEST_CASE( "Test Parameter construction id" )
{
    SECTION( "Test constructor increases id" )
    {
        const auto p = metal::Parameter{};
        REQUIRE( p.id() == 1 );
        const auto p2 = metal::Parameter{};
        REQUIRE( p2.id() == 2 );

        constexpr int num = 10000;
        const auto foo = [](){
            for ( int i = 0; i < num; i++ )
                const auto p = metal::Parameter{};
        };
        auto th1 = std::thread( foo );
        auto th2 = std::thread( foo );

        th1.join();
        th2.join();
        REQUIRE( metal::Parameter{}.id() == 2 * num + 3 );
    }
}
