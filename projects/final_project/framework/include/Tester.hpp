#include "handleton.hpp"

namespace ilrd
{
class Tester
{
  public:
    Tester(int x) { (void)x; }
    friend class Handleton<Tester>;
};
} // namespace ilrd
