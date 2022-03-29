#include "server.h"

int main() {
    Server server(60'000);
    while (true)
        server.update();
}
