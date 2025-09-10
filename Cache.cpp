\
#pragma once
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>
#include <vector>
#include <memory>
#include <chrono>

struct CacheEntry {
    int status;
    std::vector<std::pair<std::string,std::string>> headers;
    std::shared_ptr<std::vector<unsigned char>> body;
    std::chrono::steady_clock::time_point expiresAt;
    size_t size() const { return body ? body->size() : 0; }
};

class ProxyCache {
public:
    explicit ProxyCache(size_t maxBytes);
    ~ProxyCache();

    std::shared_ptr<CacheEntry> get(const std::string& key);
    void put(const std::string& key, CacheEntry&& entry);
private:
    void evictIfNeeded();

    size_t _maxBytes;
    size_t _curBytes;
    std::mutex _mutex;
    // LRU: front = most recent
    std::list<std::string> _lruList;
    std::unordered_map<std::string, std::pair<CacheEntry, std::list<std::string>::iterator>> _map;
};
