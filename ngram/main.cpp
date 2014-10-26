//
//  main.cpp
//  ngram
//
//  Created by Axel Boldt-Christmas on 16/10/14.
//  Copyright (c) 2014 HAL9000. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "ngram.h"
#include "topk.h"
#include "prefix.h"

int main(int argc, const char * argv[]) {
    NGram<3> _3gram;
    std::istringstream iss ("Lets test and see if we can make a simple 3 gram. Can you find a simple 3 gram?");
    iss >> _3gram;
    
    TopK<2> top2(_3gram);
    
    // std::cout << _3gram;
    std::cout << top2;
    
    Prefix<2> p;
    p.Next("simple");
    std::cout << "simple:" << std::endl;
    for (auto s : top2.GetTopK(p)) {
        if (s.empty()) break;
        std::cout << s << std::endl;
    }
    p.Next("3");
    std::cout << "simple 3:" << std::endl;
    for (auto s : top2.GetTopK(p)) {
        if (s.empty()) break;
        std::cout << s << std::endl;
    }
    
    return 0;
}
