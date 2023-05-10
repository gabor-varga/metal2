/** Copyright Gabor Varga 2023 */

namespace metal
{

class Parameter
{
public:
    Parameter()
        : id_{ get_next_id() }
    {
    }

    int id() const { return id_; }

private:
    static int get_next_id();

    int id_;
};


inline bool operator<( const Parameter& x, const Parameter& y )
{
    return x.id() < y.id();
}

} // namespace metal
