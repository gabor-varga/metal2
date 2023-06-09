/** Copyright Gabor Varga 2023 */

#ifndef METAL_VARIABLE_HPP
#define METAL_VARIABLE_HPP

#include "Constant.hpp"
#include <tuple>
#include <string>
#include <fmt/core.h>


namespace metal
{

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

} // metal

#endif
