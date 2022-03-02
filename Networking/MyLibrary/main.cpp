#include "connection.h"
#include "server.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"
#include <ostream>
//#include "connection.h"
//#include "connection.h"



enum class Number{
    one,
    two,
    three
};

int main(){
    myLibrary::ServerInterface<Number> server(60000);
    server.run();
    while (true) {
        server.update(-1);
    }

    return 0;
}
