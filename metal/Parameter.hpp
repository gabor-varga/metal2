/** Copyright Gabor Varga 2023 */

#include <compare>


namespace metal
{

class Parameter
{
public:
    Parameter() noexcept
        : id_{ get_next_id() }
    {
    }

    int id() const noexcept { return id_; }

    friend auto operator<=>( const Parameter&, const Parameter& ) = default;

private:
    static int get_next_id();

    int id_;
};

} // namespace metal
