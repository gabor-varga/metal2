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
    using Value = Input;
};

// Simplify additions

template<>
struct Simplify< Add< Zero, Zero > >
{
    using Value = Zero;
};

template< IsConstant Left, IsConstant Right >
struct Simplify< Add< Left, Right > >
{
    using Value = Constant< Left::Value + Right::Value >;
};

template< typename Left >
struct Simplify< Add< Left, Zero > >
{
    using Value = typename Simplify< Left >::Value;
};

template< typename Right >
struct Simplify< Add< Zero, Right > >
{
    using Value = typename Simplify< Right >::Value;
};

// Simplify multiplications

template< typename Left >
struct Simplify< Multiply< Left, Zero > >
{
    using Value = Zero;
};

template< typename Right >
struct Simplify< Multiply< Zero, Right > >
{
    using Value = Zero;
};

template< typename Left >
struct Simplify< Multiply< Left, One > >
{
    using Value = typename Simplify< Left >::Value;
};

template< typename Right >
struct Simplify< Multiply< One, Right > >
{
    using Value = typename Simplify< Right >::Value;
};

template< IsConstant Left, IsConstant Right >
struct Simplify< Multiply< Left, Right > >
{
    using Value = Constant< Left::Value * Right::Value >;
};

// Simplify double negation

template< typename Input >
struct Simplify< Negate< Negate< Input > > >
{
    using Value = typename Simplify< Input >::Value;
};


template< typename Input_ >
class Negate
{
public:
    using Input = typename Simplify< Input_ >::Value;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return -Input::eval( args );
    }

    template< typename Var >
    using Deriv = typename Simplify< Negate< typename Input::template Deriv< Var > > >::Value;
};


template< typename Input_ >
class Sin
{
public:
    using Input = typename Simplify< Input_ >::Value;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::sin( Input::eval( args ) );
    }

    template< typename Var >
    using Deriv = typename Simplify< Multiply< Cos< Input >, typename Input::template Deriv< Var > > >::Value;
};

template< typename Input_ >
class Cos
{
public:
    using Input = typename Simplify< Input_ >::Value;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::cos( Input::eval( args ) );
    }

    template< typename Var >
    // using Deriv = typename Simplify< Negate< Multiply< Sin< Input >, typename Input::template Deriv< Var > > >
    // >::Value;
    using Deriv = typename Simplify< Multiply< Negate< Sin< Input > >, typename Input::template Deriv< Var > > >::Value;
};

template< typename Left_, typename Right_ >
class Add
{
private:
    using Left = typename Simplify< Left_ >::Value;
    using Right = typename Simplify< Right_ >::Value;

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
    using Deriv = typename Simplify< Add< LeftDeriv< Var >, RightDeriv< Var > > >::Value;
};


template< typename Left_, typename Right_ >
class Multiply
{
private:
    using Left = typename Simplify< Left_ >::Value;
    using Right = typename Simplify< Right_ >::Value;

    template< typename Var >
    using LeftDeriv = typename Left::template Deriv< Var >;
    template< typename Var >
    using RightDeriv = typename Right::template Deriv< Var >;

    template< typename Var >
    using ComposedLeft = typename Simplify< Multiply< Left, RightDeriv< Var > > >::Value;
    template< typename Var >
    using ComposedRight = typename Simplify< Multiply< Right, LeftDeriv< Var > > >::Value;

public:
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return Left::eval( args ) * Right::eval( args );
    }

    template< typename Var >
    using Deriv = typename Simplify< Add< ComposedLeft< Var >, ComposedRight< Var > > >::Value;
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
