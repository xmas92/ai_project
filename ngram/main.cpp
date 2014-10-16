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

int main(int argc, const char * argv[]) {
    // insert code here...
    Prefix<3> p;
    p.Next("A");
    p.Next("B");
    p.Next("C");
    auto p1 = p.Reduce();
    auto p2 = p.Reduce().Reduce();
    std::cout << p.First() << std::endl;
    std::cout << p1.First() << std::endl;
    std::cout << p2.First() << std::endl;
    
    NGram<3> _3gram;
    std::istringstream iss ("Lets test and see if we can make a simple 3 gram.");
    
    iss >> _3gram;
    
    return 0;
}
