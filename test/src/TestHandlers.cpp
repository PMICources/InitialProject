#include <gtest/gtest.h>

#include <src/MessageAdder.hpp>
#include <src/MessageGetter.hpp>
#include <src/MessagesQueue.hpp>

#include <json.hpp>

#include "Mocks.hpp"

using json = nlohmann::json;


TEST(TEST_REQUEST_HANDLERS, TEST_SIMPLE_GETTER) {
    clearMessageQueue();
    auto& st = singleton<MessagesQueue>();
    st.addMessage(Message("anton", "mess"));

    {
        MessageGetter getter;
        std::string uri("/message?user=anton");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        ASSERT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_OK);
        json responce = json::parse(res.str());
        ASSERT_EQ(responce["dest"], "anton");
        ASSERT_EQ(responce["message"], "mess");
    }
}

TEST(TEST_REQUEST_HANDLERS, TEST_CONTAIN_MESSAGE) {
    clearMessageQueue();

    MessageAdder adder;
    MessageGetter getter;

    {
        ServerRequestMock req;
        ServerResponseMock resp;

        std::istringstream body(R"({"dest":"anton", "message":"hi, anton"})");
        std::ostringstream res;

        setReqBody(req, body);
        setRespHandler(resp, res);

        adder.handleRequest(req, resp);
        json responce = json::parse(res.str());
        //ASSERT_EQ(0, responce["id"].get<int>());
        //ASSERT_EQ(std::string("anton got message with id 0"),
         //         responce["message"].get<std::string>());
        std::cout << "123";
    }

    {
        std::string uri("/message?user=anton");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        ASSERT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_OK);
        json responce = json::parse(res.str());
        ASSERT_EQ(responce["dest"], "anton");
        ASSERT_EQ(responce["message"], "hi, anton");
    }
}


TEST(TEST_REQUEST_HANDLERS, TEST_MESSAGE_FOR_ANOTHER_USER) {
    clearMessageQueue();
    MessageAdder adder;
    MessageGetter getter;

    {
        ServerRequestMock req;
        ServerResponseMock resp;

        std::istringstream body(R"({"dest":"anton", "message":"hi, anton"})");
        std::ostringstream res;

        setReqBody(req, body);
        setRespHandler(resp, res);

        adder.handleRequest(req, resp);
    }

    {
        std::string uri("/message?user=pavel");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        ASSERT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    }
}

TEST(TEST_REQUEST_HANDLERS, TEST_REMOVE_MESSAGE) {
    clearMessageQueue();

    {
        MessageAdder adder;
        ServerRequestMock req;
        ServerResponseMock resp;

        std::istringstream body(R"({"dest":"anton", "message":"hi, anton"})");
        std::ostringstream res;

        setReqBody(req, body);
        setRespHandler(resp, res);

        adder.handleRequest(req, resp);
    }

    {
        MessageGetter getter;
        std::string uri("/message?user=anton");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        ASSERT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_OK);
        json responce = json::parse(res.str());
        ASSERT_EQ(responce["dest"], "anton");
        ASSERT_EQ(responce["message"], "hi, anton");
    }

    {
        MessageGetter getter;
        std::string uri("/message?user=anton");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        EXPECT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    }

    {
        MessageGetter getter;
        std::string uri("/message?user=anton");
        ServerRequestMock req;
        ServerResponseMock resp;

        std::ostringstream res;

        setMocks(req, "GET", uri);
        setRespHandler(resp, res);

        getter.handleRequest(req, resp);
        EXPECT_EQ(resp.getStatus(), Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    }
}



