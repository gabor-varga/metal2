/** Copyright Gabor Varga 2023 */

#include <cmath>
#include <tuple>
#include <iostream>


struct Zero
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 0;
    }
};


template< int Value_ >
struct Constant
{
    static constexpr auto Value = Value_;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return Value;
    }

    template< typename Var >
    using Deriv = Zero;
};

template< typename T >
concept IsConstant = requires( T t )
{
    []< int Value >( Constant< Value >& ) {}( t );
};


using One = Constant< 1 >;


template< int ID_ >
class Variable
{
public:
    static constexpr int ID = ID_;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::get< ID >( args );
    }

    template< typename Var >
    using Deriv = std::conditional_t< Var::ID == ID, One, Zero >;
};

// Forward declares

template< typename Input >
class Negate;

template< typename Input >
class Sin;

template< typename Input >
class Cos;

template< typename Left, typename Right >
class Add;

template< typename Left, typename Right >
class Multiply;


template< typename Input >
struct Simplify
{
    using Result = Input;
};

template< typename... Args >
using SimplifyResult = typename Simplify< Args... >::Result;

// Simplify additions

template<>
struct Simplify< Add< Zero, Zero > >
{
    using Result = Zero;
};

template< IsConstant Left, IsConstant Right >
struct Simplify< Add< Left, Right > >
{
    using Result = Constant< Left::Value + Right::Value >;
};

template< typename Left >
struct Simplify< Add< Left, Zero > >
{
    using Result = SimplifyResult< Left >;
};

template< typename Right >
struct Simplify< Add< Zero, Right > >
{
    using Result = SimplifyResult< Right >;
};

// Simplify multiplications

template< typename Left >
struct Simplify< Multiply< Left, Zero > >
{
    using Result = Zero;
};

template< typename Right >
struct Simplify< Multiply< Zero, Right > >
{
    using Result = Zero;
};

template< typename Left >
struct Simplify< Multiply< Left, One > >
{
    using Result = SimplifyResult< Left >;
};

template< typename Right >
struct Simplify< Multiply< One, Right > >
{
    using Result = SimplifyResult< Right >;
};

template< IsConstant Left, IsConstant Right >
struct Simplify< Multiply< Left, Right > >
{
    using Result = Constant< Left::Value * Right::Value >;
};

// Simplify double negation

template< typename Input >
struct Simplify< Negate< Negate< Input > > >
{
    using Result = SimplifyResult< Input >;
};


template< typename Input_ >
class Negate
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return -Input::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Negate< typename Input::template Deriv< Var > > >;
};


template< typename Input_ >
class Sin
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::sin( Input::eval( args ) );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Multiply< Cos< Input >, typename Input::template Deriv< Var > > >;
};

template< typename Input_ >
class Cos
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::cos( Input::eval( args ) );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Multiply< Negate< Sin< Input > >, typename Input::template Deriv< Var > > >;
};

template< typename Left_, typename Right_ >
class Add
{
private:
    using Left = SimplifyResult< Left_ >;
    using Right = SimplifyResult< Right_ >;

    template< typename Var >
    using LeftDeriv = typename Left::template Deriv< Var >;
    template< typename Var >
    using RightDeriv = typename Right::template Deriv< Var >;

public:
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return Left::eval( args ) + Right::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Add< LeftDeriv< Var >, RightDeriv< Var > > >;
};


template< typename Left_, typename Right_ >
class Multiply
{
private:
    using Left = SimplifyResult< Left_ >;
    using Right = SimplifyResult< Right_ >;

    template< typename Var >
    using LeftDeriv = typename Left::template Deriv< Var >;
    template< typename Var >
    using RightDeriv = typename Right::template Deriv< Var >;

    template< typename Var >
    using ComposedLeft = SimplifyResult< Multiply< Left, RightDeriv< Var > > >;
    template< typename Var >
    using ComposedRight = SimplifyResult< Multiply< Right, LeftDeriv< Var > > >;

public:
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return Left::eval( args ) * Right::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Add< ComposedLeft< Var >, ComposedRight< Var > > >;
};


template< typename Input, int ID >
constexpr auto diff( Input input, Variable< ID > var )
{
    return typename Input::template Deriv< Variable< ID > >{};
}


template< int ID >
constexpr auto operator-( Variable< ID > var )
{
    return Negate< Variable< ID > >{};
}
