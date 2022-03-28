#pragma once

#include <vector>
#include <iostream>
#include <adoint_backcompat.h>
#include "server_interface.h"
#include "game_enum.h"
#include "message.h"
#include "Object.h"

/*
 * Goal it to create a server supporting vector of objects. So customer will be
 * able to iterate through synchronized objects and change them.
 */
struct Server : myLibrary::ServerInterface<OBJECT_TYPE>{
    explicit Server(uint16_t port, uint16_t limit);

    bool on_client(std::shared_ptr<myLibrary::Connection<OBJECT_TYPE>> client) override;

    const std::vector<Object>& get_objects();

private:
    std::vector<Object> _objects;
    uint16_t _limit;
};

