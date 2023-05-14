/** Copyright Gabor Varga 2023 */

#include <string>


namespace metal
{

template< typename ErrorType, typename... Params >
void check( bool condition, Params&&... params )
{
    if ( !condition )
    {
        throw ErrorType{ params... };
    }
}

}
