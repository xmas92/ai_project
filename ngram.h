//
//  ngram.h
//  ngram
//
//  Created by Axel Boldt-Christmas on 16/10/14.
//  Copyright (c) 2014 HAL9000. All rights reserved.
//

#ifndef ngram_ngram_h
#define ngram_ngram_h
#include <unordered_map>
#include <utility>
#include <string>
#include <cassert>
#include <type_traits>
#include <limits>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdint>
#include "prefix.h"

template <
unsigned N,
typename key_type = std::string,
typename count_type = uint32_t,
typename total_type = uint64_t,
typename = typename std::enable_if<std::is_integral<count_type>::value, count_type>::type,
typename = typename std::enable_if<std::is_integral<total_type>::value, total_type>::type,
typename = typename std::enable_if<std::is_convertible<count_type, total_type>::value>::type
>
class NGram {
    template<unsigned,typename,typename,typename,typename,typename,typename>
    friend class NGram;
    typedef NGram<N> self;
    typedef NGram<N-1> nextgram_type;
    typedef Prefix<N,key_type> prefix_type;
    typedef std::pair<nextgram_type, count_type> pair_type;
    typedef std::pair<key_type, count_type> keycount_type;
    typedef std::unordered_map<key_type, pair_type> map_type;
    
    map_type map;
    total_type total;
    
    void AddPrefix(prefix_type prefix) {
        key_type word = prefix.First();
        if (word.empty()) return;
        assert((++total) != std::numeric_limits<total_type>::max());
        assert((++map[word].second) != std::numeric_limits<count_type>::max());
        map[word].first.AddPrefix(prefix.Reduce());
    }
public:
    std::vector<key_type> GetKeys() {
        std::vector<key_type> r;
        r.reserve(map.size());
        for (auto p : map)
            r.push_back(p.first);
        return r;
    }
    std::vector<keycount_type> GetKeyCount() {
        std::vector<keycount_type> r;
        r.reserve(map.size());
        for (auto p : map)
            r.push_back(std::make_pair(p.first,p.second.second));
        return r;
    }
    template<unsigned K>
    std::array<key_type, K> TopK(std::list<key_type> given) {
        assert(given.size() != 0);
        if (given.size() == 1) {
            std::array<key_type, K> r;
            auto vec = map[given.front()].first.GetKeyCount();
            std::sort(vec.begin(), vec.end(), [](keycount_type a, keycount_type b) {
                          return a.second > b.second;
            });
            std::transform(vec.begin(), vec.begin()+std::min<unsigned>(K, static_cast<unsigned>(vec.size())), r.begin(), [](const keycount_type& p) { return p.first; });
            return r;
        } else {
            given.pop_front();
            return map[given.front()].first.template TopK<K>(given);
        }
    }
    
    void PrintNGram(std::ostream &os, unsigned depth) {
        assert(depth <= N);
        if (depth == 0) return;
        if (depth == 1)
            for (auto & p : map)
                os << p.first << " : " << p.second.second << std::endl;
        else
            for (auto & p : map)
                p.second.first.PrintNGram(os, depth-1, p.first + " ");
    }
    void PrintNGram(std::ostream &os, unsigned depth, std::string prefix) {
        assert(depth <= N);
        if (depth == 0) return;
        if (depth == 1)
            for (auto & p : map)
                os << prefix << p.first << " : " << p.second.second << std::endl;
        else
            for (auto & p : map)
                p.second.first.PrintNGram(os, depth-1, prefix + p.first + " ");
        
    }
    friend std::ostream &operator<<(std::ostream &os, NGram<N> &ngram) {
        for(unsigned n = 1; n <= N; n++) {
            os << n << "-gram!" << std::endl;
            ngram.PrintNGram(os,n);
        }
        return os;
    }
    friend std::istream &operator>>(std::istream &is, NGram<N> &ngram) {
        prefix_type prefix;
        key_type nextKey;
        while (is >> nextKey) {
            prefix.Next(nextKey);
            ngram.AddPrefix(prefix);
        }
        while (!prefix.First().empty()) {
            prefix.Next(key_type());
            ngram.AddPrefix(prefix);
        }
        return is;
    }
};

template <
typename key_type,
typename count_type,
typename total_type
>
class NGram<0, key_type,count_type,total_type> {
    typedef Prefix<0,key_type> prefix_type;
    typedef std::pair<key_type, count_type> keycount_type;
    typedef std::unordered_map<key_type, uint32_t> map_type;
    
    count_type count;
public:
    void PrintNGram(std::ostream &os, unsigned depth, std::string prefix) {}
    void AddPrefix(prefix_type prefix) {
        assert((++count) != std::numeric_limits<count_type>::max());
    }
    template<unsigned K>
    std::array<key_type, K> TopK(std::list<key_type> given) {
        return std::array<key_type, K>();
    }
    std::vector<keycount_type> GetKeyCount() {
        return std::vector<keycount_type>();
    }
    
};

#endif
