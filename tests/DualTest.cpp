#include "metal/Dual.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <utility>
#include <memory>


struct FunctionCalls
{
    int ctor = 0;
    int dtor = 0;
    int copy = 0;
    int move = 0;
    int cass = 0;
    int mass = 0;

    bool operator==( const FunctionCalls& other ) const = default;
};

std::ostream& operator<<( std::ostream& os, const FunctionCalls& object )
{
    os << "ctor " << object.ctor << std::endl;
    os << "dtor " << object.dtor << std::endl;
    os << "copy " << object.copy << std::endl;
    os << "move " << object.move << std::endl;
    os << "cass " << object.cass << std::endl;
    os << "mass " << object.mass << std::endl;
    return os;
}


template< int Id >
struct Mock
{
    Mock( int value )
        : value_{ std::make_unique< int >( value ) }
    {
        function_calls.ctor++;
    }

    ~Mock() { function_calls.dtor++; }

    Mock( const Mock& other )
        : value_{ std::make_unique< int >( *other.value_.get() ) }
    {
        function_calls.copy++;
    }

    Mock( Mock&& other ) noexcept
        : value_{ std::move( other.value_ ) }
    {
        function_calls.move++;
    }

    Mock& operator=( const Mock& other )
    {
        value_ = other.value_;
        function_calls.cass++;
    }

    Mock& operator=( Mock&& other ) noexcept
    {
        value_ = std::move( other.value_ );
        function_calls.mass++;
    }

    static void reset() { function_calls = FunctionCalls{}; }

    std::unique_ptr< int > value_ = nullptr;

    static FunctionCalls function_calls;
};

using Value = Mock< 0 >;
using Deriv = Mock< 1 >;


template<>
FunctionCalls Value::function_calls = FunctionCalls{};

template<>
FunctionCalls Deriv::function_calls = FunctionCalls{};


TEST_CASE( "Test number of function calls with different constructor arguments" )
{
    const auto test_function_calls = []( const FunctionCalls& expected_function_calls )
    {
        REQUIRE( Value::function_calls == expected_function_calls );
        REQUIRE( Deriv::function_calls == expected_function_calls );
        Value::reset();
        Deriv::reset();
    };

    const auto perfect_move = FunctionCalls{ 1, 2, 0, 1, 0, 0 };
    const auto non_perfect_copy = FunctionCalls{ 1, 2, 1, 0, 0, 0 };

    SECTION( "Constructor with both rvalue args" )
    {
        {
            metal::Dual< Value, Deriv > dual{ Value{ 1 }, Deriv{ 2 } };
        }
        test_function_calls( perfect_move );
    }

    SECTION( "Constructor with both lvalue args" )
    {
        {
            Value value{ 1 };
            Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ value, deriv };
        }
        test_function_calls( perfect_move );
    }

    SECTION( "Constructor with lvalue and rvalue args" )
    {
        {
            Value value{ 1 };
            metal::Dual< Value, Deriv > dual{ value, Deriv{ 2 } };
        }
        test_function_calls( perfect_move );
    }

    SECTION( "Constructor with rvalue and lvalue args" )
    {
        {
            Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ Value{ 1 }, deriv };
        }
        test_function_calls( perfect_move );
    }

    SECTION( "Constructor with both lvalue args" )
    {
        {
            const Value value{ 1 };
            const Deriv deriv{ 1 };
            metal::Dual< Value, Deriv > dual{ value, deriv };
        }
        test_function_calls( non_perfect_copy );
    }
}
