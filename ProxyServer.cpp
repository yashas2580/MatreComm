\
#pragma once
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <memory>
#include <string>

class ProxyServer {
public:
    ProxyServer(const std::string& host, unsigned short port);
    ~ProxyServer();
    void start();
    void stop();

private:
    std::unique_ptr<Poco::Net::ServerSocket> _socket;
    std::unique_ptr<Poco::Net::HTTPServer> _server;
    std::string _host;
    unsigned short _port;
};
