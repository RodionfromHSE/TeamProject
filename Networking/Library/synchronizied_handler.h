#pragma once

#include "fwd.h"
#include "message.h"

namespace net {
    template<typename T>
    struct SynchronizedHandler {
        [[nodiscard]] bool empty(int id) {
            std::unique_lock l(m);
            return synTable[id].empty();
        };

        [[nodiscard]] Message<T> last_message(int id) {
            std::unique_lock l(m);
            auto msg = std::move(synTable[id].back());
            clear_impl(id);
            return msg;
        };

        void clear(int id){
            std::unique_lock l(m);
            clear_impl(id);
        }

        void add_message(const Message<T> &msg, int id, bool isNewbie = false) {
            std::unique_lock l(m);
            if (isNewbie){
                newbies.push_back(id);
                return;
            }
            synTable[id].push_back(msg);
        };

        std::vector<int> get_newbies(){
            auto newbiesTmp = std::move(newbies);
            newbies.clear();

            return std::move(newbiesTmp);
        }

    private:
        void clear_impl(int id){
            synTable[id].clear();
        }

    private:
        std::mutex m;
        std::unordered_map<int, std::vector<Message <T>>> synTable;
        std::vector<int> newbies;
    };
}
