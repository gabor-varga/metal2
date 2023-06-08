/** Copyright Gabor Varga 2023 */

#include <cmath>
#include <tuple>
#include <iostream>
#include <fmt/core.h>


template< typename Var, typename Input >
constexpr auto diff( Input input )
{
    return simplify( input.template deriv< Var >() );
}

template< typename Var, typename Input >
constexpr auto diff( Input input, Var )
{
    return simplify( input.template deriv< Var >() );
}

struct Zero
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 0;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        // static_assert(false, "Can not differentiate Zero");
    }

    std::string str() const { return "Zero"; }
};

struct One
{
    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return 1;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return "One"; }
};

template< typename T >
class Constant
{
public:
    constexpr Constant( T value )
        : value_{ value }
    {
    }

    const auto value() const { return value_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > ) const
    {
        return value_;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Zero{};
    }

    std::string str() const { return fmt::format( "{0}", value_ ); }

private:
    T value_;
};


template< int ID_ >
class Variable
{
public:
    static constexpr int ID = ID_;

    Variable( std::string_view name = std::string{} )
        : name_{ name.empty() ? fmt::format( "Var_{0}", ID ) : name }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return std::get< ID >( args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        if constexpr ( Var::ID == ID )
        {
            return One{};
        }
        else
        {
            return Zero{};
        }
    }

    std::string str() const { return name_; }

private:
    std::string name_;
};

// Forward declares

// template< typename Input >
// class Negate;

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


// template< typename Input >
// constexpr auto subexp( Input input )
// {

// }

// template< typename... Args >
// constexpr auto cse( Args... args )
// {

// }


template< typename Input, typename Operator >
class UnaryOperator
{
public:
    constexpr UnaryOperator( Input input )
        : input_{ input }
    {
    }

    constexpr auto input() const { return input_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return Operator::eval( input_, args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Operator::template deriv< Var >( input_ );
    }

    std::string str() const { return Operator::str( input_.str() ); }

private:
    Input input_;
};

struct NegateOp
{
    template< typename Input, typename... Args >
    static constexpr auto eval( Input input, std::tuple< Args... > args )
    {
        return -input.eval( args );
    }

    template< typename Var, typename Input >
    static constexpr auto deriv( Input input )
    {
        return -diff< Var >( input );
    }

    static constexpr std::string str( std::string_view input ) { return fmt::format( "(-{0})", input ); }
};


template< typename Input >
using Negate = UnaryOperator< Input, NegateOp >;


template< typename Input >
class Sin
{
public:
    constexpr Sin( Input input )
        : input_{ input }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return std::sin( input_.eval( args ) );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return cos( input_ ) * diff< Var >( input_ );
    }

    std::string str() const { return fmt::format( "sin({0})", input_.str() ); }

private:
    Input input_;
};

template< typename Input >
class Cos
{
public:
    constexpr Cos( Input input )
        : input_{ input }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return std::cos( input_.eval( args ) );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return -sin( input_ ) * diff< Var >( input_ );
    }

    std::string str() const { return fmt::format( "cos({0})", input_.str() ); }

private:
    Input input_;
};


template< typename Input >
class Square
{
public:
    constexpr Square( Input input )
        : input_{ input }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        const auto tmp = input_.eval( args );
        return tmp * tmp;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Constant{ 2 } * input_ * diff< Var >( input_ );
    }

    std::string str() const { return fmt::format( "{0}^2", input_.str() ); }

private:
    Input input_;
};


template< typename Input >
class Cube
{
public:
    constexpr Cube( Input input )
        : input_{ input }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        const auto tmp = input_.eval( args );
        return tmp * tmp * tmp;
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Constant{ 3 } * square( input_ ) * diff< Var >( input_ );
    }

    std::string str() const { return fmt::format( "{0}^3", input_.str() ); }

private:
    Input input_;
};


template< typename Input >
class SquareRoot
{
public:
    constexpr SquareRoot( Input input )
        : input_{ input }
    {
    }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return std::sqrt( input_.eval( args ) );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return Constant{ 0.5 } / sqrt( input_ ) * diff< Var >( input_ );
    }

    std::string str() const { return fmt::format( "sqrt({0})", input_.str() ); }

private:
    Input input_;
};


template< typename Left, typename Right >
class Add
{
public:
    constexpr Add( Left left, Right right )
        : left_{ left }
        , right_{ right }
    {
    }

    constexpr auto left() const { return left_; }
    constexpr auto right() const { return right_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return left_.eval( args ) + right_.eval( args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return diff< Var >( left_ ) + diff< Var >( right_ );
    }

    std::string str() const { return fmt::format( "({0} + {1})", left_.str(), right_.str() ); }

private:
    Left left_;
    Right right_;
};


template< typename Left, typename Right >
class Subtract
{
public:
    constexpr Subtract( Left left, Right right )
        : left_{ left }
        , right_{ right }
    {
    }

    constexpr auto left() const { return left_; }
    constexpr auto right() const { return right_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return left_.eval( args ) - right_.eval( args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return diff< Var >( left_ ) - diff< Var >( right_ );
    }

    std::string str() const { return fmt::format( "({0} - {1})", left_.str(), right_.str() ); }

private:
    Left left_;
    Right right_;
};


template< typename Left, typename Right >
class Multiply
{
public:
    constexpr Multiply( Left left, Right right )
        : left_{ left }
        , right_{ right }
    {
    }

    constexpr auto left() const { return left_; }
    constexpr auto right() const { return right_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return left_.eval( args ) * right_.eval( args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return diff< Var >( left_ ) * right_ + diff< Var >( right_ ) * left_;
    }

    std::string str() const { return fmt::format( "({0} * {1})", left_.str(), right_.str() ); }

private:
    Left left_;
    Right right_;
};


template< typename Left, typename Right >
class Divide
{
public:
    constexpr Divide( Left left, Right right )
        : left_{ left }
        , right_{ right }
    {
    }

    constexpr auto left() const { return left_; }
    constexpr auto right() const { return right_; }

    template< typename... Args >
    constexpr auto eval( std::tuple< Args... > args ) const
    {
        return left_.eval( args ) / right_.eval( args );
    }

    template< typename Var >
    constexpr auto deriv() const
    {
        return ( diff< Var >( left_ ) * right_ - diff< Var >( right_ ) * left_ ) / square( right_ );
    }

    std::string str() const { return fmt::format( "({0} / {1})", left_.str(), right_.str() ); }

private:
    Left left_;
    Right right_;
};


// template< typename Input, int ID, typename T >
// constexpr auto diff( Input input, Variable< ID, T > )
// {
//     return typename input.deriv< Variable< ID, T > >{};
// }


template< typename Input >
constexpr auto simplify( Input input )
{
    return input;
}

template< typename Left >
constexpr auto simplify( Add< Left, Zero > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Add< Zero, Right > input )
{
    return input.right();
}

// template< typename Input >
// constexpr auto simplify( Add< Input, Input > input )
// {
//     return Multiply{ Constant{ 2 }, input.left() };
// }

// template< typename Input, typename L, typename R >
// constexpr auto simplify( Add< Constant< L >, Input > left, Add< Constant< R >, Input > right )
// {
//     return Multiply{ Constant{ left.left().value() + right.right().value() }, left.right() };
// }

template< typename Left >
constexpr auto simplify( Subtract< Left, Zero > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Subtract< Zero, Right > input )
{
    return -input.right();
}

template< typename Left >
constexpr auto simplify( Multiply< Left, Zero > )
{
    return Zero{};
}

template< typename Right >
constexpr auto simplify( Multiply< Zero, Right > )
{
    return Zero{};
}

template< typename Left >
constexpr auto simplify( Multiply< Left, One > input )
{
    return input.left();
}

template< typename Right >
constexpr auto simplify( Multiply< One, Right > input )
{
    return input.right();
}

template< typename C1, typename C2 >
constexpr auto simplify( Multiply< Constant< C1 >, Constant< C2 > > input )
{
    return Constant{ input.left().value() * input.right().value() };
}

template< typename Left, typename Right, typename C >
constexpr auto simplify( Multiply< Left, Multiply< Constant< C >, Right > > input )
{
    return input.right().left() * input.left() * input.right().right();
}

template< typename Left, typename Right, typename C >
constexpr auto simplify( Multiply< Left, Multiply< Right, Constant< C > > > input )
{
    return input.right().right() * input.left() * input.right().left();
}

template< typename Input >
constexpr auto simplify( Negate< Negate< Input > > input )
{
    return input.input().input();
}


template< typename Input >
constexpr auto operator-( Input input )
{
    return simplify( Negate< Input >{ input } );
}

template< typename Left, typename Right >
constexpr auto operator+( Left left, Right right )
{
    return simplify( Add{ left, right } );
}

template< typename Left >
constexpr auto operator+( Left left, int right )
{
    return simplify( Add{ left, Constant{ right } } );
}

template< typename Left, typename Right >
constexpr auto operator-( Left left, Right right )
{
    return simplify( Subtract{ left, right } );
}

template< typename Left, typename Right >
constexpr auto operator*( Left left, Right right )
{
    return simplify( Multiply{ left, right } );
}

template< typename Left >
constexpr auto operator*( Left left, double right )
{
    return simplify( Multiply{ Constant{ right }, left } );
}

template< typename Right >
constexpr auto operator*( double left, Right right )
{
    return simplify( Multiply{ Constant{ left }, right } );
}

template< typename Left, typename Right >
constexpr auto operator/( Left left, Right right )
{
    return simplify( Divide{ left, right } );
}

template< typename Input >
constexpr auto square( Input input )
{
    return simplify( Square{ input } );
}

template< typename Input >
constexpr auto cube( Input input )
{
    return simplify( Cube{ input } );
}

template< typename Input >
constexpr auto sqrt( Input input )
{
    return simplify( SquareRoot{ input } );
}

template< typename Input >
constexpr auto sin( Input input )
{
    return simplify( Sin{ input } );
}

template< typename Input >
constexpr auto cos( Input input )
{
    return simplify( Cos{ input } );
}

constexpr auto orbital_period( auto sma, auto gm )
{
    constexpr double TWOPI = 2 * M_PI;
    return Constant{ TWOPI } * sqrt( cube( sma ) / gm );
}

constexpr auto test_func( auto x, auto y )
{
    return sin( x ) + cos( y );
}
