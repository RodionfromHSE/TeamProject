//#include "connection.h"
//#include "server.h"
#include "tsqueue.h"
#include "message.h"
#include "fwd.h"
//#include "connection.h"
//#include "connection.h"

enum class Number{
    one,
    two,
    three
};

int main(){
    myLibrary::Message<Number> msg;
    myLibrary::TSQueue<myLibrary::Message<Number>> q;
    for (int i = 0; i < 3; ++i) {
        myLibrary::Message<Number> tmpMsg;
        auto tmpStr = "Bla-bla-bla";
        tmpMsg << tmpStr;
        q.push_back(std::move(tmpMsg));
    }
    while (!q.empty()) {
//        myLibrary::Message<Number> tmp;
        auto tmp = q.pop_front();
        std::cout << tmp << '\n';
    }
    msg.header.id = Number::three;
    std::string str = "Hi";
    msg << str;
    std::cout << msg << '\n';
    str = "garbage";
    msg >> str;
    std::cout << msg << '\n';
    std::cout << str << '\n';
}
