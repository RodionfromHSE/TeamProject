#pragma once

#include "fwd.h"
#include "message.h"

namespace net {
    template<typename T>
    struct SynchroniziedHandler {

        bool empty(int id) const {
            std::unique_lock l(_m);
            return _synTable[id].empty();
        };

        Message <T> last_message(int id) {
            std::unique_lock l(_m);
            auto msg = std::move(_synTable[id].back());
            clear_impl(id);
            return msg;
        };

        void clear(int id){
            std::unique_lock l(_m);
            clear_impl(id);
        }

        void add_message(const Message <T> &msg, int id) {
            std::unique_lock l(_m);
            _synTable[id].push_back(msg);
        };

    private:
        void clear_impl(int id){
            _synTable[id].clear();
        }

    private:
        std::mutex _m;
        std::unordered_map<int, std::vector<Message < T>>>
        _synTable;
    };
}
