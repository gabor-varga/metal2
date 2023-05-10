/** Copyright Gabor Varga 2023 */

#include <array>
#include <stdlib.h>
#include <stdexcept>


namespace metal
{

template< typename T, int StackSize_ >
class SmallVector
{
public:
    static_assert( StackSize_ > 0 );

    static constexpr int StackSize = StackSize_;

    enum class Mode
    {
        Stack,
        Heap
    };

    /** Default constructor*/
    SmallVector()
        : mode_{ Mode::Stack }
        , heap_{ nullptr }
        , data_{ &stack_[0] }
        , capacity_{ StackSize }
        , size_{ 0 }
    {
    }

    /** Sized constructor */
    explicit SmallVector( const int size )
        : mode_{ size > StackSize ? Mode::Heap : Mode::Stack }
        , heap_{ mode_ == Mode::Heap ? malloc( sizeof( T ) * size ) : nullptr }
        , data_{ mode_ == Mode::Heap ? heap_ : &stack_[0] }
        , capacity_{ mode_ == Mode::Heap ? size : StackSize }
        , size_{ size }
    {
    }

    /** Member access */
    const T* data() const { return data_; }
    int capacity() const { return capacity_; }
    int size() const { return size_; }

    /** Unsafe element access */
    const T& operator[]( const int i ) const { return data_[i]; }
    T& operator[]( const int i ) { return data_[i]; }

    /** Safe element access */
    const T& at( const int i ) const
    {
        if ( i >= size_ )
            throw std::runtime_error( "Invalid index" );
        return data_[i];
    }
    T& at( const int i )
    {
        if ( i >= size_ )
            throw std::runtime_error( "Invalid index" );
        return data_[i];
    }

private:
    // std::array< T, StackSize > stack_;
    T stack_[StackSize];
    Mode mode_;
    T* heap_;
    T* data_;
    int capacity_;
    int size_;
};

} // namespace metal
