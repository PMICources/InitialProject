#ifndef HIGHLOADCUP_MAINLISTENER_H
#define HIGHLOADCUP_MAINLISTENER_H

#include <Poco/Util/ServerApplication.h>

#include <iostream>
#include <vector>

class MainListener final : public Poco::Util::ServerApplication {
    virtual int main(const std::vector<std::string>& args) override;
};


#endif //HIGHLOADCUP_MAINLISTENER_H
