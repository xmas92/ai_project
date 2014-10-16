//
//  prefix.h
//  ngram
//
//  Created by Axel Boldt-Christmas on 16/10/14.
//  Copyright (c) 2014 HAL9000. All rights reserved.
//

#ifndef ngram_prefix_h
#define ngram_prefix_h
#include <string>
#include <array>
#include <algorithm>
/*
 Keep track of N
*/
template <unsigned N>
struct Prefix {
    template<unsigned T>
    friend class Prefix;
private:
    typedef Prefix<N-1> reduce_type;
    typedef std::string key_type;
    typedef std::array<std::string, N> array_type;
    array_type arr;
    void Shift() {
        std::copy_n(arr.begin()+1, N-1, arr.begin());
    }
public:
    reduce_type Reduce() {
        reduce_type r;
        std::copy_n(arr.begin()+1, N-1, r.arr.begin());
        return r;
    }
    key_type First() {
        return arr.front();
    }
    void Next(key_type key) {
        Shift();
        arr.back() = key;
    }
};
template <>
class Prefix<0> {
    
public:
    
};

#endif
