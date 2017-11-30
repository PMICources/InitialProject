#include <future>

#include <gtest/gtest.h>

#include <src/MessageAdder.hpp>
#include <src/MessageGetter.hpp>

#include <json.hpp>

#include "Mocks.hpp"

using json = nlohmann::json;


void sendRequest(const std::string& user, const std::string& message) {
    MessageAdder adder;

    std::stringstream ss;
    ss << json{{"dest", user}, {"message", message}};
    const auto body = ss.str();


    ServerRequestMock req;
    ServerResponseMock resp;

    std::istringstream strBody(body);
    std::ostringstream res;

    setReqBody(req, strBody);
    setRespHandler(resp, res);

    adder.handleRequest(req, resp);
}

Poco::Net::HTTPResponse::HTTPStatus getRequest(const std::string& user) {
    std::ostringstream query;
    query << "/message?user=" << user;
    MessageGetter getter;
    std::string uri(query.str());
    ServerRequestMock req;
    ServerResponseMock resp;

    std::ostringstream res;

    setMocks(req, "GET", uri);
    setRespHandler(resp, res);

    getter.handleRequest(req, resp);
    return resp.getStatus();
}


TEST(TEST_MULTITHREAD, TEST_MULTITHREAD_ADD) {

    std::atomic<long> addedCount (0);

    std::vector<std::future<void>> pool;

    for (int i = 0; i < 2; ++i) {
        pool.push_back(std::async(std::launch::async, [&]{
            for (int j = 0; j < 100; ++j) {
                std::stringstream ss;
                ss << "user";
                sendRequest(ss.str(), "message");
                addedCount++;
            }
        }));
    }

    for (const auto& f : pool) {
        f.wait();
    }

    int gettedCount = 0;

    Poco::Net::HTTPResponse::HTTPStatus status =
            Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;

    do {
        status = getRequest("user");
        if (status == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
            ++ gettedCount;
        }
    } while (status == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);

    //ASSERT_EQ(gettedCount, addedCount);
}

/*
TEST(TEST_MULTITHREAD, TEST_MULTITHREAD_GET) {

    int addedCount = 0;

    std::vector<std::future<void>> pool;

    for (int j = 0; j < 10000; ++j) {
        std::stringstream ss;
        ss << "user";
        sendRequest(ss.str(), "message");
        addedCount++;
    }

    std::atomic<int> gettedCount (0);

    for (int i = 0; i < 2; ++i) {
        pool.push_back(std::async(std::launch::async, [&] {
            Poco::Net::HTTPResponse::HTTPStatus status =
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
            do {
                status = getRequest("user");
                if (status == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
                    ++gettedCount;
                }
            } while (status == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        }));
    }

    for (const auto& f : pool) {
        f.wait();
    }

    ASSERT_EQ(gettedCount, addedCount);
}
*/