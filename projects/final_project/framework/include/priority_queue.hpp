#ifndef __ILRD_RD176_PRIORITY_QUEUE__
#define __ILRD_RD176_PRIORITY_QUEUE__

#include <queue>
#include <stdexcept>

namespace ilrd
{

template <class T, class Container = std::vector<T>,
          class Compare = std::less<T>>

class PriorityQueue : private std::priority_queue<T, Container, Compare>
// The container must satisfy the
// requirement SequenceContainer,same
// constraint as std::queue
{

    using OwnQueue = PriorityQueue<T, std::priority_queue<T>, std::less<T>>;
    using pq = std::priority_queue<T, Container, Compare>;

  public:
    const T& front() const;

    using pq::empty;
    using pq::pop;
    using pq::push;

}; // class PriorityQueue

template <class T, class Container, class Compare>
const T& PriorityQueue<T, Container, Compare>::front() const
{
    return this->top();
}

} // namespace ilrd

#endif // __ILRD_RD176_PRIORITY_QUEUE__
