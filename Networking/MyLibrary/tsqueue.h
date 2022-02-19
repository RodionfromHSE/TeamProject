#ifndef TSQUEUE_H
#define TSQUEUE_H

#include "fwd.h"
#include "message.h"

namespace myLibrary {
    template<typename T>
    struct TSQueue {
        bool empty() const noexcept {
            std::unique_lock l(_m);
            return _msgQueue.empty();
        }

        T &front() const {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            return _msgQueue.front();
        }

        T &back() const {
            std::unique_lock l(_m);
            assert(!_msgQueue.empty());
            return _msgQueue.back();
        }

        void push_front(const T& item) {
            std::unique_lock l(_m);
            _msgQueue.push_front(item);
        }

        void push_back(const T& item) {
            std::unique_lock l(_m);
            _msgQueue.push_back(item);}

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
    };
}


#endif //TEAMPROJECT_TSQUEUE_H
