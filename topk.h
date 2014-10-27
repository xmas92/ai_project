//
//  topk.h
//  ngram
//
//  Created by Axel Boldt-Christmas on 26/10/14.
//  Copyright (c) 2014 HAL9000. All rights reserved.
//

#ifndef ngram_topk_h
#define ngram_topk_h
#include <unordered_map>
#include <string>
#include <list>
#include <array>
#include "prefix.h"
#include "ngram.h"


static int indent = 0;

template <
unsigned K,
typename key_type = std::string
>
class TopK {
    template<unsigned,typename>
    friend class TopK;
    typedef TopK<K,key_type> self;
    typedef std::pair<key_type, self> map_pair_type;
    typedef std::unordered_map<key_type, self> map_type;
    
    template <int N, typename DUMMY = void> struct Func {
        static std::array<key_type, K> func(Prefix<N> prefix, self & topk) {
            if (prefix.First().empty())
                return Func<N-1>::func(prefix.Reduce(), topk);
            if (topk.map.count(prefix.First()) == 0)
                return std::array<key_type, K>();
            return Func<N-1>::func(prefix.Reduce(), topk.map[prefix.First()]);
        }
    };
    
    template <typename DUMMY> struct Func<1, DUMMY> {
        static std::array<key_type, K> func(Prefix<1> prefix, self & topk) {
            if (topk.map.count(prefix.First()) == 0)
                return std::array<key_type, K>();
            std::array<key_type, K> r;
            auto & t = topk.map[prefix.First()].map;
            std::transform(t.begin(), t.end(), r.begin(), [](const map_pair_type& p) { return p.first; });
            return r;
        }
    };
    
    map_type map;
    unsigned ngram_size;
public:
    TopK() {
        
    }
    template<unsigned N>
    TopK(NGram<N, key_type>& ngram) {
        Generate(ngram);
    }
    template<unsigned N>
    void Generate(NGram<N, key_type> ngram) {
        ngram_size = N;
        map = map_type(ngram.NumberUniqueKeys());
        for (auto k : ngram.GetKeys()) {
            std::list<key_type> l;
            l.push_front(k);
            map[k].GenerateK(ngram, l);
        }
    }
    template<unsigned N>
    std::array<key_type, K> GetTopK(Prefix<N> prefix) {
        return Func<N>::func(prefix, *this);
    }
    template<unsigned N>
    void GenerateK(NGram<N, key_type> ngram, std::list<key_type> l) {
        ngram_size = N;
        map = map_type(K);
        for (auto k : ngram.TopK<K>(l)) {
            if (k.empty())
                return;
            l.push_back(k);
            map[k].GenerateK(ngram, l);
            l.pop_back();
        }
    }
    friend std::ostream &operator<<(std::ostream &os, TopK<K> &topk) {
        for (auto p : topk.map) {
            for (int i = 0; i < indent; i++)
                os << "\t";
            os << p.first << ":" << std::endl;
            indent++;
            os << p.second;
            indent--;
        }
        return os;
    }
};


#endif
