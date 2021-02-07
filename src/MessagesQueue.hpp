#ifndef HIGHLOADCUP_MESSAGESQUEUE_H
#define HIGHLOADCUP_MESSAGESQUEUE_H

#include <queue>
#include <map>
#include <mutex>
#include <string>

class MessagesQueue;

template<typename T>
T& singleton();

template<>
MessagesQueue& singleton<MessagesQueue>();

uint64_t nextId();
uint64_t& totalCounter();


void clearMessageQueue();

class Message {
 public:
    Message(const uint64_t id, const std::string &dest_,
            const std::string &message_);
    Message(const std::string &dest_, const std::string &message_);
    Message(const Message&) = default;
    Message(Message&&) = default;

    std::string dest() const;
    std::string message() const;
    uint64_t id() const;

 private:
    std::string dest_;
    std::string message_;
    uint64_t id_;
};

class MessagesQueue {
 public:
    void addMessage(const Message&);
    bool hasNext(const std::string& user) const;
    Message getNext(const std::string& user);
    void clear();
 private:
    MessagesQueue();
    std::map<std::string, std::queue<Message>> messagesContainer_;
    mutable std::mutex lock_;
    friend MessagesQueue& singleton<MessagesQueue>();
};



#endif // HIGHLOADCUP_MESSAGESQUEUE_H
