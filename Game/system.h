#ifndef TEAMPROJECT_SYSTEM_H
#define TEAMPROJECT_SYSTEM_H

#include <vector>
#include <memory>

struct System {
    virtual void init() {}

    virtual void update() {}

    virtual void shutdown() {}

    virtual ~System() = default;

};

using Systems = std::vector<std::unique_ptr<System>>;

#endif //TEAMPROJECT_SYSTEM_H
