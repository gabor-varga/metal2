#include "metal/Dual.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <utility>
#include <memory>


template< int Id >
struct Mock
{
    Mock( int value )
        : value_{ std::make_unique< int >( value ) }
    {
        ctor++;
    }

    ~Mock() { dtor++; }

    Mock( const Mock& other )
        : value_{ std::make_unique< int >( *other.value_.get() ) }
    {
        copy++;
    }

    Mock( Mock&& other ) noexcept
        : value_{ std::move( other.value_ ) }
    {
        move++;
    }

    Mock& operator=( const Mock& other )
    {
        value_ = other.value_;
        cass++;
    }

    Mock& operator=( Mock&& other ) noexcept
    {
        value_ = std::move( other.value_ );
        mass++;
    }

    static void reset()
    {
        ctor = 0;
        dtor = 0;
        copy = 0;
        move = 0;
        cass = 0;
        mass = 0;
    }

    static void summary()
    {
        std::cout << "-----------" << std::endl;
        std::cout << "ctor " << ctor << std::endl;
        std::cout << "dtor " << dtor << std::endl;
        std::cout << "copy " << copy << std::endl;
        std::cout << "move " << move << std::endl;
        std::cout << "cass " << cass << std::endl;
        std::cout << "mass " << mass << std::endl;
        std::cout << "-----------" << std::endl;
    }

    std::unique_ptr< int > value_ = nullptr;

    static int ctor;
    static int dtor;
    static int copy;
    static int move;
    static int cass;
    static int mass;
};

using Value = Mock< 0 >;
using Deriv = Mock< 1 >;


template<>
int Value::ctor = 0;
template<>
int Value::dtor = 0;
template<>
int Value::copy = 0;
template<>
int Value::move = 0;
template<>
int Value::cass = 0;
template<>
int Value::mass = 0;

template<>
int Deriv::ctor = 0;
template<>
int Deriv::dtor = 0;
template<>
int Deriv::copy = 0;
template<>
int Deriv::move = 0;
template<>
int Deriv::cass = 0;
template<>
int Deriv::mass = 0;


TEST_CASE( "Mock testing" )
{
    SECTION( "Constructor called once" )
    {
        {
            Dual< Value, Deriv > dual{ Value{ 1 }, Deriv{ 2 } };
        }

        REQUIRE( Value::ctor == 1 );
        REQUIRE( Value::dtor == 2 );
        REQUIRE( Value::copy == 0 );
        REQUIRE( Value::move == 1 );
        REQUIRE( Value::cass == 0 );
        REQUIRE( Value::mass == 0 );

        Value::summary();
        Deriv::summary();
        Value::reset();
        Deriv::reset();
    }

    SECTION( "Constructor called once" )
    {
        {
            Value value{ 1 };
            Deriv deriv{ 1 };
            Dual< Value, Deriv > dual{ value, deriv };
        }

        Value::summary();
        Deriv::summary();
        Value::reset();
        Deriv::reset();

        // REQUIRE( Value::ctor == 1 );
        // REQUIRE( Value::dtor == 1 );
        // REQUIRE( Value::copy == 0 );
        // REQUIRE( Value::move == 0 );
        // REQUIRE( Value::cass == 0 );
        // REQUIRE( Value::mass == 0 );
    }

    SECTION( "Constructor called once" )
    {
        {
            Value value{ 1 };
            Dual< Value, Deriv > dual{ value, Deriv{ 2 } };
        }

        Value::summary();
        Deriv::summary();
        Value::reset();
        Deriv::reset();

        // REQUIRE( Value::ctor == 1 );
        // REQUIRE( Value::dtor == 1 );
        // REQUIRE( Value::copy == 0 );
        // REQUIRE( Value::move == 0 );
        // REQUIRE( Value::cass == 0 );
        // REQUIRE( Value::mass == 0 );
    }

    SECTION( "Constructor called once" )
    {
        {
            Deriv deriv{ 1 };
            Dual< Value, Deriv > dual{ Value{ 1 }, deriv };
        }

        Value::summary();
        Deriv::summary();
        Value::reset();
        Deriv::reset();

        // REQUIRE( Value::ctor == 1 );
        // REQUIRE( Value::dtor == 1 );
        // REQUIRE( Value::copy == 0 );
        // REQUIRE( Value::move == 0 );
        // REQUIRE( Value::cass == 0 );
        // REQUIRE( Value::mass == 0 );
    }
}
