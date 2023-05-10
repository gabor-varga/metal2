/** Copyright Gabor Varga 2023 */

#include <catch2/catch_test_macros.hpp>

#include <memory>


struct FunctionCalls
{
    int ctor = 0;
    int dtor = 0;
    int copy = 0;
    int move = 0;
    int cass = 0;
    int mass = 0;

    auto operator<=>( const FunctionCalls& ) const = default;
};

inline std::ostream& operator<<( std::ostream& os, const FunctionCalls& object )
{
    os << "ctor " << object.ctor << std::endl;
    os << "dtor " << object.dtor << std::endl;
    os << "copy " << object.copy << std::endl;
    os << "move " << object.move << std::endl;
    os << "cass " << object.cass << std::endl;
    os << "mass " << object.mass << std::endl;
    return os;
}


template< typename Derived >
struct MockBase
{
    int value() const { return static_cast< const Derived& >( *this ).value(); }
};

template< typename Derived >
std::ostream& operator<<( std::ostream& os, const MockBase< Derived >& object )
{
    os << object.value();
    return os;
}


template< int Id >
struct Mock : public MockBase< Mock< Id > >
{
    explicit Mock( int value )
        : value_{ std::make_unique< int >( value ) }
    {
        function_calls.ctor++;
    }

    template< typename Derived >
    explicit Mock( const MockBase< Derived >& other )
        : value_{ std::make_unique< int >( other.value() ) }
    {
    }

    ~Mock() { function_calls.dtor++; }

    Mock( const Mock& other )
        : value_{ std::make_unique< int >( other.value() ) }
    {
        function_calls.copy++;
    }

    // Mock( const Mock& other ) = delete;

    Mock( Mock&& other ) noexcept
        : value_{ std::move( other.value_ ) }
    {
        function_calls.move++;
    }

    Mock& operator=( const Mock& other )
    {
        value_ = std::make_unique< int >( other.value() );
        function_calls.cass++;
    }

    Mock& operator=( Mock&& other ) noexcept
    {
        value_ = std::move( other.value_ );
        function_calls.mass++;
    }

    bool is_valid() const { return value_ != nullptr; }

    int value() const
    {
        if ( value_ )
            return *value_;
        else
            throw std::runtime_error( "Invalid Mock object" );
    }

    static void reset() { function_calls = FunctionCalls{}; }

    std::unique_ptr< int > value_ = nullptr;

    static FunctionCalls function_calls;
};


template< int Id >
class MockAddition : public MockBase< MockAddition< Id > >
{
public:
    MockAddition( const MockBase< Mock< Id > >& lhs, const MockBase< Mock< Id > >& rhs )
        : lhs_{ lhs }
        , rhs_{ rhs }
    {
    }

    int value() const { return lhs_.value() + rhs_.value(); }

private:
    const MockBase< Mock< Id > >& lhs_;
    const MockBase< Mock< Id > >& rhs_;
};

template< int Id >
auto operator+( const MockBase< Mock< Id > >& x, const MockBase< Mock< Id > >& y )
{
    return MockAddition< Id >{ x, y };
}
