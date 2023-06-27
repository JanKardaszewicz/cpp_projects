//
// Jan Kardaszewicz 407313

#include <utility>
#include "../include/storage_types.hpp"
Package PackageQueue::pop()
{
    Package result;
    switch (_type)
    {
        case PackageQueueType::LIFO:
            result = std::move(*_queue.end());
            _queue.pop_back();
            break;
        case PackageQueueType::FIFO:
            result = std::move(*_queue.begin());
            _queue.pop_front();
            break;
        default:
            break;
    }
    return result;
}