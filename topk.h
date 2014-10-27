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
    
    template <unsigned N, typename DUMMY = void> struct GetTopK_s {
        static std::array<key_type, K> func(Prefix<N> prefix, self & topk) {
            if (prefix.First().empty())
                return GetTopK_s<N-1>::func(prefix.Reduce(), topk);
            if (topk.map.count(prefix.First()) == 0)
                return std::array<key_type, K>();
            return GetTopK_s<N-1>::func(prefix.Reduce(), topk.map[prefix.First()]);
        }
    };
    
    template <typename DUMMY> struct GetTopK_s<1, DUMMY> {
        static std::array<key_type, K> func(Prefix<1> prefix, self & topk) {
            if (topk.map.count(prefix.First()) == 0)
                return std::array<key_type, K>();
            std::array<key_type, K> r;
            auto & t = topk.map[prefix.First()].map;
            std::transform(t.begin(), t.end(), r.begin(), [](const map_pair_type& p) { return p.first; });
            return r;
        }
    };
    
    template <unsigned NP, unsigned NG, typename DUMMY = void> struct GenerateK_s {
        static void func(NGram<NG, key_type>& ngram, Prefix<NP> prefix, self & topk) {
            topk.ngram_size = NG;
            topk.map = map_type(K);
            for (auto k : ngram.TopK<K>(prefix)) {
                if (k.empty())
                    return;
                GenerateK_s<NP+1, NG>::func(ngram, prefix.Expand(k), topk.map[k]);
            }
        }
    };
    
    template <unsigned NG, typename DUMMY> struct GenerateK_s<10, NG, DUMMY> {
        static void func(NGram<NG, key_type>& ngram, Prefix<10> prefix, self & topk) {
            return;
        }
    };
    
    map_type map;
    unsigned ngram_size;
public:
    TopK() {
        
    }
    uint64_t Size() {
        uint64_t size = static_cast<uint64_t>(map.size());
        for (auto p : map) {
            size += p.second.Size();
        }
        return size;
    }
    template<unsigned N>
    TopK(NGram<N, key_type>& ngram) {
        Generate(ngram);
    }
    template<unsigned N>
    std::array<key_type, K> GetTopK(Prefix<N> prefix) {
        return GetTopK_s<N>::func(prefix, *this);
    }
    template<unsigned N>
    void Generate(NGram<N, key_type>& ngram) {
        ngram_size = N;
        map = map_type(ngram.NumberUniqueKeys());
        Prefix<1> prefix;
        for (auto k : ngram.GetKeys()) {
            prefix.Last() = k;
            GenerateK_s<1, N>::func(ngram, prefix, map[k]);
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
