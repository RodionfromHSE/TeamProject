#include <catch.hpp>
//#include <synchronizied.h>
#include "tsqueue.h"
#include "client.h"
#include "synchronizied.h"

TEST_CASE("Should add money to account", "[account]") {
    net::TSQueue<int> tsq;
    tsq.push_back(100);
    CHECK(!tsq.empty());
}

struct TestClient : net::Client{
    TestClient(){
        // connect
    }


    void connect() override {}

    void send() override {}
};

TEST_CASE("Should all money to account with debt", "[account]") {
    net::Synchronizied<int> x(std::make_shared<net::Client>());
    CHECK(x.is_connected());
    x.set(5);
    CHECK(x.get() == 5);
    /*
     * Some server's job...
     */
    CHECK(x.get() == 10);


}