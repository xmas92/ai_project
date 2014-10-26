//
//  main.cpp
//  Tester
//
//  Created by Axel Boldt-Christmas on 26/10/14.
//  Copyright (c) 2014 HAL9000. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <string>
#include <array>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <thread>
#include "prefix.h"
#include "ngram.h"
#include "topk.h"

using namespace std;

#define N 3
#define K 10
#define UNIFORMNEXT
//#define NORMALNEXT
//#define FIXEDNEXT

#ifdef UNIFORMNEXT
#define MINNEXT 10
#define MAXNEXT 12
#endif

#ifdef NORMALNEXT
#define MEANNEXT 11
#define STDDIVNEXT 1
#endif

#ifdef FIXEDNEXT
#define NEXT 11
#endif


int TimeToNextGenerate() {
    random_device rd;
    default_random_engine e(rd());
#ifdef UNIFORMNEXT
    uniform_int_distribution<int> uniform_dist(MINNEXT, MAXNEXT);
    return uniform_dist(e);
#endif
#ifdef NORMALNEXT
    normal_distribution<int> normal_dist(MEANNEXT, STDDIVNEXT);
    return normal_dist(e);
#endif
#ifdef FIXEDNEXT
    return NEXT;
#endif
}

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        cerr << "Usage: Tester corpus testcorpus" << endl;
        return 0;
    }
    ifstream corpus(argv[1]), testcorpus(argv[2]);
    if (!corpus.is_open()) {
        cerr << "File could not be open:" << argv[1] <<  endl;
        return 0;
    }
    if (!testcorpus.is_open()) {
        cerr << "File could not be open:" << argv[2] <<  endl;
        return 0;
    }
    NGram<N> ngram;
    corpus >> ngram;
    NGram<N>::CINALL = false;
    TopK<K> topk(ngram);
    
    uint64_t found = 0;
    uint64_t total = 0;
    uint64_t tkeys = 0;
    uint64_t keyss = 0;
    string word;
    int TTGenerate = TimeToNextGenerate();
    Prefix<N-1> prefix;
    array<string, K> predicted;
    //chrono::milliseconds dura( 1000 );
    //this_thread::sleep_for( dura );
    while (testcorpus >> word) {
        if (TTGenerate-- == 0) {
            // Regenerate TopK
            TTGenerate = TimeToNextGenerate();
            topk.Generate(ngram);
        }
        istringstream iss(word);
        iss >> ngram;
        total++;
        tkeys += word.size();
        if (find(begin(predicted),end(predicted),word) != end(predicted)) {
            found++;
            keyss += word.size();
        }
        prefix.Next(word);
        predicted = topk.GetTopK(prefix);
    }
    cout << "Total Words: " << total << endl;
    cout << "Predicted Words: " << found << endl;
    cout << "Hit Rate: " << found / static_cast<double>(total) << endl;
    cout << "Total Keys: " << tkeys << endl;
    cout << "Saved Keys: " << keyss << endl;
    cout << "Key SS: " << keyss / static_cast<double>(tkeys) << endl;

    return 0;
}
