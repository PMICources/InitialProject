#include "MessageAdder.hpp"
#include "MessagesQueue.hpp"

#include <json.hpp>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Logger.h>

using json = nlohmann::json;

namespace {
    json generateResponceText(const uint64_t id, const std::string message) {
        json resp = {
                {"id", id},
                {"message", message}
        };
        return resp;
    }
}


void MessageAdder::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    Poco::Logger& logger = Poco::Logger::get("MessageGetter");

    std::stringstream str;
    Poco::StreamCopier::copyStream(request.stream(), str);
    std::string body(str.str());

    logger.debug("Got add message request with body " + body);

    auto b = json::parse(body.begin(), body.end());

    auto& queue = singleton<MessagesQueue>();

    auto m = Message(b["dest"], b["message"]);

    queue.addMessage(m);

    json resp = generateResponceText(m.id(),
                                     b["dest"].get<std::string>() +
                                             std::string(" got message with id ") +
                                             std::to_string(m.id()));

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    std::ostream& ostr = response.send();

    ostr << resp.dump();
}
