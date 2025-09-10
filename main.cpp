\
#include "ProxyServer.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <condition_variable>

static std::atomic<bool> g_stop{false};
static std::condition_variable g_cv;
static std::mutex g_cv_m;

void signal_handler(int) {
    g_stop = true;
    g_cv.notify_one();
}

int main(int argc, char** argv) {
    std::string host = "0.0.0.0";
    unsigned short port = 8080;
    if (argc >= 2) host = argv[1];
    if (argc >= 3) port = static_cast<unsigned short>(std::stoi(argv[2]));

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    try {
        ProxyServer server(host, port);
        std::cout << "Starting proxy on " << host << ":" << port << std::endl;
        server.start();

        // wait until signal
        {
            std::unique_lock<std::mutex> lk(g_cv_m);
            g_cv.wait(lk, [](){ return g_stop.load(); });
        }

        std::cout << "Shutdown requested, stopping server..." << std::endl;
        server.stop();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
