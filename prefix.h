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
template <unsigned N, typename key_type = std::string>
struct Prefix {
    template <unsigned,typename>
    friend class Prefix;
    typedef Prefix<N-1> reduce_type;
    typedef Prefix<N+1> expand_type;
private:
    typedef std::array<key_type, N> array_type;
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
    expand_type Expand() {
        expand_type r;
        std::copy_n(arr.begin(), N, r.arr.begin()+1);
        return r;
    }
    expand_type Expand(key_type key) {
        expand_type r;
        std::copy_n(arr.begin(), N, r.arr.begin());
        r.arr.back() = key;
        return r;
    }
    key_type First() {
        return arr.front();
    }
    key_type& Last() {
        return arr.back();
    }
    void Next(key_type key) {
        Shift();
        arr.back() = key;
    }
};

template <typename key_type>
struct Prefix<10,key_type> {
    template <unsigned,typename>
    friend class Prefix;
    typedef std::array<key_type, 10> array_type;
    array_type arr;
};

template <typename key_type>
struct Prefix<0,key_type> {
    typedef std::array<key_type, 0> array_type;
    array_type arr;
    
    Prefix<0> Reduce() {
        return *this;
    }
    
    Prefix<1> Expand() {
        return Prefix<1>();
    }
    
    Prefix<1> Expand(key_type key) {
        Prefix<1> r;
        r.arr[0] = key;
        return r;
    }
    
    key_type First() {
        return key_type();
    }
    key_type Last() {
        return arr.front();
    }
};

#endif
