#include "Tools/Reporter/Reporter_monitor.hpp"

namespace aff3ct
{
namespace tools
{
template <class M_t>
Reporter_monitor<M_t>::Reporter_monitor(const M &monitor)
: spu::tools::Reporter(), monitor(monitor)
{
}
}
}
