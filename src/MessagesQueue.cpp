#include "MessagesQueue.hpp"

namespace {
    static uint64_t id = 0;
}

uint64_t nextId() {
    return id++;
}

Message::Message(const uint64_t id, const std::string &dest_, const std::string &message_) :
        dest_(dest_), message_(message_), id_(id) {}

Message::Message(const std::string &dest_, const std::string &message_) :
        dest_(dest_), message_(message_), id_(nextId()) {}

std::string Message::dest() const {
    return dest_;
}

std::string Message::message() const {
    return message_;
}

uint64_t Message::id() const {
    return id_;
}


void MessagesQueue::addMessage(const Message & m) {
    std::lock_guard<std::mutex> guard(lock_);
    messagesContainer_[m.dest()].push(m);
}

bool MessagesQueue::hasNext(const std::string &user) const {
    std::lock_guard<std::mutex> guard(lock_);
    return messagesContainer_.count(user) && !messagesContainer_.at(user).empty();
}

Message MessagesQueue::getNext(const std::string &user) {
    std::lock_guard<std::mutex> guard(lock_);
    Message res(messagesContainer_[user].front());
    messagesContainer_[user].pop();
    return res;
}

MessagesQueue::MessagesQueue() { }

void MessagesQueue::clear() {
    std::lock_guard<std::mutex> guard(lock_);
    messagesContainer_.clear();
}


template<>
MessagesQueue& singleton() {
    static MessagesQueue mq;
    return mq;
}



void clearMessageQueue() {
    id = 0;
    auto& mq = singleton<MessagesQueue>();
    mq.clear();
}