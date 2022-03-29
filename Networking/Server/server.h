#pragma once

#include <vector>
#include <iostream>
#include "server_interface.h"
#include "game_enum.h"
#include "message.h"
#include "object.h"

/*
 * Goal it to create a server supporting vector of objects. So customer will be
 * able to iterate through synchronized objects and change them.
 */
struct Server : myLibrary::ServerInterface<OBJECT_TYPE>{
    explicit Server(uint16_t port, uint16_t limit = 100);

    bool on_client(std::shared_ptr<myLibrary::Connection<OBJECT_TYPE>> client) override;

    void handle_message(std::shared_ptr<myLibrary::Connection<OBJECT_TYPE>> client, myLibrary::Message<OBJECT_TYPE> &msg) override;

    const std::vector<Object>& get_objects();

private:
    std::vector<Object> _objects;
    uint16_t _limit;
};

