#ifndef TSQUEUE_H
#define TSQUEUE_H

#include "fwd.h"
#include "message.h"

namespace net {
    template<typename T>
    struct TSQueue {
        void wait(){
            std::unique_lock l(_waitMutex);
            _cv.wait(l, [this](){ return !empty(); });
        }

        bool empty() const noexcept {
            std::unique_lock l(_m);
            return _msgQueue.empty();
        }

        std::size_t size() const noexcept{
            std::unique_lock l(_m);
            return _msgQueue.size();
        }

        T &front() {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            return _msgQueue.front();
        }

        T &back() {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            return _msgQueue.back();
        }

        void push_front(const T& item) {
            std::unique_lock l(_m);
            _msgQueue.push_front(item);

            std::unique_lock<std::mutex> ul(_waitMutex);
            _cv.notify_one();
        }

        void push_back(const T& item) {
            std::unique_lock l(_m);
            _msgQueue.push_back(item);

            std::unique_lock<std::mutex> ul(_waitMutex);
            _cv.notify_one();
        }

        T pop_front() {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            auto tmpMsg = std::move(_msgQueue.front());
            _msgQueue.pop_front();
            return tmpMsg;
        }

        T pop_back() {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            auto tmpMsg = std::move(_msgQueue.back());
            _msgQueue.pop_back();
            return tmpMsg;
        }

    private:
        std::deque<T> _msgQueue;
        mutable std::mutex _m;
        mutable std::mutex _waitMutex;
        mutable std::condition_variable _cv;
    };
}


#endif //TEAMPROJECT_TSQUEUE_H
