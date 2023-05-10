/** Copyright Gabor Varga 2023 */

#include "Parameter.hpp"
#include <atomic>


namespace metal
{

int Parameter::get_next_id()
{
    static std::atomic< int > next_id = 0;
    return ++next_id;
}

} // namespace metal
