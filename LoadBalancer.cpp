#pragma once
#include <vector>
#include <string>
#include <mutex>

class LoadBalancer {
public:
    LoadBalancer(const std::vector<std::string>& backends);
    std::string nextBackend();

private:
    std::vector<std::string> _backends;
    size_t _idx;
    std::mutex _mutex;
};
