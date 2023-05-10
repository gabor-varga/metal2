/** Copyright Gabor Varga 2023 */

#include "metal/SmallVector.hpp"
#include <array>

#include <catch2/catch_test_macros.hpp>


TEST_CASE( "Test default constructor" )
{
    SECTION( "Test with non-zero stack size" )
    {
        metal::SmallVector< int, 1 > v;

        REQUIRE( v.data() != nullptr );
        REQUIRE( v.capacity() == 1 );
        REQUIRE( v.size() == 0 );

        const int id = 1;
        const int val = 2;
        v[id] = val;
        REQUIRE( v[id] == val );

        // v.at( id ) = val;

        const auto f = [&]() { v.at( id ) = val; };
        // f();

        REQUIRE_THROWS( f() );
    }
}
