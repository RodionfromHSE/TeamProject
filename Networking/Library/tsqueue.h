#pragma once

#include "fwd.h"

namespace net {
    template<typename T>
    struct TSQueue {
        void wait(){
            std::unique_lock l(m_waitMutex);
            m_cv.wait(l, [this](){ return !empty(); });
        }

        bool empty() const noexcept {
            std::unique_lock l(m_m);
            return m_msgQueue.empty();
        }

        std::size_t size() const noexcept{
            std::unique_lock l(m_m);
            return m_msgQueue.size();
        }

        T &front() {
            std::unique_lock l(m_m);
            assert(!m_msgQueue.empty());
            return m_msgQueue.front();
        }

        T &back() {
            std::unique_lock l(m_m);
            assert(!m_msgQueue.empty());
            return m_msgQueue.back();
        }

        void push_front(const T& item) {
            std::unique_lock l(m_m);
            m_msgQueue.push_front(item);

            std::unique_lock<std::mutex> ul(m_waitMutex);
            m_cv.notify_one();
        }

        void push_back(const T& item) {
            std::unique_lock l(m_m);
            m_msgQueue.push_back(item);

            std::unique_lock<std::mutex> ul(m_waitMutex);
            m_cv.notify_one();
        }

        T pop_front() {
            std::unique_lock l(m_m);
            assert(!m_msgQueue.empty());
            auto tmpMsg = std::move(m_msgQueue.front());
            m_msgQueue.pop_front();
            return tmpMsg;
        }

        T pop_back() {
            std::unique_lock l(m_m);
            assert(!m_msgQueue.empty());
            auto tmpMsg = std::move(m_msgQueue.back());
            m_msgQueue.pop_back();
            return tmpMsg;
        }

    private:
        std::deque<T> m_msgQueue;
        mutable std::mutex m_m;
        mutable std::mutex m_waitMutex;
        mutable std::condition_variable m_cv;
    };
}
