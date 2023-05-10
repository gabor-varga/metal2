/** Copyright Gabor Varga 2023 */

#include "metal/Dual.hpp"
#include "Mock.hpp"

#include <catch2/catch_test_macros.hpp>

// Set up mock objects
using Value = Mock< 0 >;
using Deriv = Mock< 1 >;

template<>
FunctionCalls Value::function_calls = FunctionCalls{};

template<>
FunctionCalls Deriv::function_calls = FunctionCalls{};


TEST_CASE( "Test number of function calls with different constructor arguments" )
{
    const auto test_function_calls
        = []( const FunctionCalls& expected_value_function_calls, const FunctionCalls& expected_deriv_function_calls )
    {
        REQUIRE( Value::function_calls == expected_value_function_calls );
        REQUIRE( Deriv::function_calls == expected_deriv_function_calls );
        Value::reset();
        Deriv::reset();
    };

    const auto perfect_move = FunctionCalls{ 1, 2, 0, 1, 0, 0 };
    SECTION( "Constructor with both rvalue args" )
    {
        {
            metal::Dual< Value, Deriv > dual{ Value{ 1 }, Deriv{ 2 } };
        }
        test_function_calls( perfect_move, perfect_move );
    }

    const auto non_perfect_copy = FunctionCalls{ 1, 2, 1, 0, 0, 0 };
    SECTION( "Constructor with both lvalue args" )
    {
        {
            Value value{ 1 };
            Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ value, deriv };

            // Make sure value and deriv objects are still valid
            REQUIRE( value.is_valid() );
            REQUIRE( deriv.is_valid() );
        }
        test_function_calls( non_perfect_copy, non_perfect_copy );
    }

    SECTION( "Constructor with lvalue and rvalue args" )
    {
        {
            Value value{ 1 };
            metal::Dual< Value, Deriv > dual{ value, Deriv{ 2 } };

            // Make sure value is still valid
            REQUIRE( value.is_valid() );
        }
        test_function_calls( non_perfect_copy, perfect_move );
    }

    SECTION( "Constructor with rvalue and lvalue args" )
    {
        {
            Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ Value{ 1 }, deriv };

            // Make sure deriv is still valid
            REQUIRE( deriv.is_valid() );
        }
        test_function_calls( perfect_move, non_perfect_copy );
    }

    SECTION( "Constructor with both const lvalue args" )
    {
        {
            const Value value{ 1 };
            const Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ value, deriv };
        }
        test_function_calls( non_perfect_copy, non_perfect_copy );
    }

    const auto perfect_move_ctor = FunctionCalls{ 1, 3, 0, 2, 0, 0 };
    SECTION( "Move constructor test" )
    {
        {
            metal::Dual< Value, Deriv > dual{ Value{ 1 }, Deriv{ 2 } };
            metal::Dual< Value, Deriv > dual2{ std::move( dual ) };
        }
        test_function_calls( perfect_move_ctor, perfect_move_ctor );
    }

    const auto non_perfect_copy_ctor = FunctionCalls{ 1, 3, 1, 1, 0, 0 };
    SECTION( "Copy constructor test" )
    {
        {
            const metal::Dual< Value, Deriv > dual{ Value{ 1 }, Deriv{ 2 } };
            metal::Dual< Value, Deriv > dual2{ dual };
        }
        test_function_calls( non_perfect_copy_ctor, non_perfect_copy_ctor );
    }

    const auto perfect_binary_op_move = FunctionCalls{ 2, 4, 0, 2, 0, 0 };
    SECTION( "Test binary operators" )
    {
        {
            metal::Dual< Value, Deriv > x{ Value{ 1 }, Deriv{ 2 } };
            metal::Dual< Value, Deriv > y{ Value{ 3 }, Deriv{ 4 } };
            auto z = x + y;

            REQUIRE( x.value().is_valid() );
            REQUIRE( x.deriv().is_valid() );
            REQUIRE( y.value().is_valid() );
            REQUIRE( y.deriv().is_valid() );

            REQUIRE( z.value().value() == 4 );
            REQUIRE( z.deriv().value() == 6 );
        }
        test_function_calls( perfect_binary_op_move, perfect_binary_op_move );
    }
}
