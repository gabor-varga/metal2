/** Copyright Gabor Varga 2023 */

#include <cmath>
#include <tuple>
#include <iostream>


template< typename Input, typename Var >
using Diff = typename Input::template Deriv< Var >;


struct Zero
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 0;
    }
};

struct Half
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 0.5;
    }

    template< typename Var >
    using Deriv = Zero;
};

struct One
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 1;
    }

    template< typename Var >
    using Deriv = Zero;
};

struct Two
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 2;
    }

    template< typename Var >
    using Deriv = Zero;
};

struct Three
{
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& /**args*/ )
    {
        return 3;
    }

    template< typename Var >
    using Deriv = Zero;
};


template< typename T >
class Constant
{
public:
    constexpr Constant( T value )
        : value_{ value }
    {
    }

    template< typename... Args >
    constexpr auto eval( const std::tuple< Args... >& /**args*/ ) const
    {
        return value_;
    }

    template< typename Var >
    using Deriv = Zero;

private:
    T value_;
};


// template< typename Type, typename T >
// concept IsConstant = std::is_same_v< Type, Constant< T > >;

// using One = Constant< 1 >;
// using Two = Constant< 2 >;

// static constexpr Constant One{ 1 };
// static constexpr Constant Two{ 2 };


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

template< typename Input >
class Squre;

template< typename Input >
class SqureRoot;

template< typename Left, typename Right >
class Add;

template< typename Left, typename Right >
class Subtract;

template< typename Left, typename Right >
class Multiply;

template< typename Left, typename Right >
class Divide;


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

// Simplify subtraction

template<>
struct Simplify< Subtract< Zero, Zero > >
{
    using Result = Zero;
};

template< typename Left >
struct Simplify< Subtract< Left, Zero > >
{
    using Result = SimplifyResult< Left >;
};

template< typename Right >
struct Simplify< Subtract< Zero, Right > >
{
    using Result = SimplifyResult< Negate< Right > >;
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

// Simplify division

template< typename Left >
struct Simplify< Divide< Left, Zero > >
{
    static_assert( "Division by zero" );
};

template< typename Right >
struct Simplify< Divide< Zero, Right > >
{
    using Result = Zero;
};

template< typename Left >
struct Simplify< Divide< Left, One > >
{
    using Result = SimplifyResult< Left >;
};

// Add back when "reciprocal" is added
// template< typename Right >
// struct Simplify< Divide< One, Right > >
// {
//     using Result = SimplifyResult< Right >;
// };

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
    using Deriv = SimplifyResult< Negate< Diff< Input, Var > > >;
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
    using Deriv = SimplifyResult< Multiply< Cos< Input >, Diff< Input, Var > > >;
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
    using Deriv = SimplifyResult< Multiply< Negate< Sin< Input > >, Diff< Input, Var > > >;
};


template< typename Input_ >
class Square
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        const auto tmp = Input::eval( args );
        return tmp * tmp;
    }

    template< typename Var >
    using Deriv = SimplifyResult< Multiply< Multiply< Two, Input >, Diff< Input, Var > > >;
};


template< typename Input_ >
class Cube
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        const auto tmp = Input::eval( args );
        return tmp * tmp * tmp;
    }

    template< typename Var >
    using Deriv = SimplifyResult< Multiply< Multiply< Three, Square< Input > >, Diff< Input, Var > > >;
};


template< typename Input_ >
class SquareRoot
{
public:
    using Input = SimplifyResult< Input_ >;

    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return std::sqrt( Input::eval( args ) );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Multiply< Divide< Half, SquareRoot< Input > >, Diff< Input, Var > > >;
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
class Subtract
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
        return Left::eval( args ) - Right::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Subtract< LeftDeriv< Var >, RightDeriv< Var > > >;
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
    using ComposedLeft = SimplifyResult< Multiply< LeftDeriv< Var >, Right > >;
    template< typename Var >
    using ComposedRight = SimplifyResult< Multiply< RightDeriv< Var >, Left > >;

public:
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return Left::eval( args ) * Right::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Add< ComposedLeft< Var >, ComposedRight< Var > > >;
};


template< typename Left_, typename Right_ >
class Divide
{
private:
    using Left = SimplifyResult< Left_ >;
    using Right = SimplifyResult< Right_ >;

    template< typename Var >
    using LeftDeriv = typename Left::template Deriv< Var >;
    template< typename Var >
    using RightDeriv = typename Right::template Deriv< Var >;

    template< typename Var >
    using ComposedLeft = SimplifyResult< Multiply< LeftDeriv< Var >, Right > >;
    template< typename Var >
    using ComposedRight = SimplifyResult< Multiply< RightDeriv< Var >, Left > >;

public:
    template< typename... Args >
    static constexpr auto eval( const std::tuple< Args... >& args )
    {
        return Left::eval( args ) / Right::eval( args );
    }

    template< typename Var >
    using Deriv = SimplifyResult< Divide< Subtract< ComposedLeft< Var >, ComposedRight< Var > >, Square< Right > > >;
};


template< typename Input, int ID >
constexpr auto diff( Input, Variable< ID > )
{
    return typename Input::template Deriv< Variable< ID > >{};
}


template< typename Input >
constexpr auto operator-( Input )
{
    return Negate< Input >{};
}

template< typename Left, typename Right >
constexpr auto operator+( Left, Right )
{
    return Add< Left, Right >{};
}

template< typename Left, typename Right >
constexpr auto operator-( Left, Right )
{
    return Subtract< Left, Right >{};
}

template< typename Left, typename Right >
constexpr auto operator*( Left, Right )
{
    return Multiply< Left, Right >{};
}

template< typename Left, typename Right >
constexpr auto operator/( Left, Right )
{
    return Divide< Left, Right >{};
}

template< typename Input >
constexpr auto square( Input )
{
    return Square< Input >{};
}

template< typename Input >
constexpr auto cube( Input )
{
    return Cube< Input >{};
}

template< typename Input >
constexpr auto sqrt( Input )
{
    return SquareRoot< Input >{};
}

template< typename Input >
constexpr auto sin( Input )
{
    return Sin< Input >{};
}

template< typename Input >
constexpr auto cos( Input )
{
    return Cos< Input >{};
}

constexpr auto orbital_period( auto sma, auto gm )
{
    return sqrt( cube( sma ) / gm );
}

constexpr auto test_func( auto x, auto y )
{
    return sin( x ) + cos( y );
}
