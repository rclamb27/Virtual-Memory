#pragma once
#include <unordered_map>
#include <queue>

#define SIZE 32

class LRU
{
private:
    std::unordered_map<int, int> map;
    using HashItr = std::unordered_map<int, int>::iterator;
    std::queue<int> lru_cache;
    int hits, misses;
public:
    LRU() : hits(0), misses(0) { }
  
    bool contains(int pn)
    {
        auto f = map.find(pn);
        if(f == map.end()){ ++misses; return false; }
        ++hits;
        return true;
    }
  
    void push(int pn, int phys_addy){
        if(map.size() > SIZE)
        {
            auto lru = lru_cache.front();
            lru_cache.pop();
            map.erase(lru);
        }
        map[pn] = phys_addy;
        lru_cache.push(pn);
    }

    decltype(map)::mapped_type operator[](int pn)
    {
        return map[pn];
    }

    decltype(hits)   getHits()  
    { 
      return this->hits;                                
    }
    decltype(misses) getMisses()
    { 
      return this->misses; 
    }
};
