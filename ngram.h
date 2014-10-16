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
    template<unsigned,typename>
    friend class NGram;
    typedef NGram<N> self;
    typedef NGram<N-1> nextgram_type;
    typedef Prefix<N,key_type> prefix_type;
    typedef std::pair<nextgram_type, count_type> pair_type;
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
    friend std::istream &operator>>(std::istream &is, NGram<N> &ngram) {
        prefix_type prefix;
        key_type nextKey;
        while (is >> nextKey) {
            prefix.Next(nextKey);
            ngram.AddPrefix(prefix);
        }
        while (prefix.First().empty()) {
            ngram.AddPrefix(prefix);
            prefix.Next(key_type());
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
    typedef std::unordered_map<key_type, uint32_t> map_type;
    
    count_type count;
    
    void AddPrefix(prefix_type prefix) {
        assert((++count) != std::numeric_limits<count_type>::max());
    }
public:
    
};

#endif
