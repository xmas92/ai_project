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
#define WPM 80
#define WPS (WPM/60.0)

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
    typedef chrono::nanoseconds NS;
    typedef chrono::duration<double, std::ratio<1>> S;
    auto tp = chrono::high_resolution_clock::now();
    NGram<N> ngram;
    corpus >> ngram;
    chrono::nanoseconds dur = chrono::high_resolution_clock::now() - tp;
    cout << "Total Words: " << ngram.Total() << endl;
    cout << "1-grams: " << ngram.NumberUniqueKeys() << endl;
    cout << "N-Gram Build Time: " << chrono::duration_cast<S>(dur).count() << " s ("  << dur.count() << " ns)" << endl;
    NGram<N>::CINALL = false;
    tp = chrono::high_resolution_clock::now();
    TopK<K> topk(ngram);
    dur = chrono::high_resolution_clock::now() - tp;
    cout << "Top-K Build Time: " << chrono::duration_cast<S>(dur).count() << " s ("  << dur.count() << " ns)" << endl;
    
    array<uint64_t, 2> found = {0,0};
    uint64_t  total = 0;
    uint64_t  tkeys = 0;
    array<uint64_t, 2>  keyss = {0,0};
    string word;
    Prefix<N-1> prefix;
    array<array<string, K>, 2> predicted;
    //chrono::milliseconds dura( 1000 );
    //this_thread::sleep_for( dura );
    uint64_t WordsSinceLastGen = 0;
    while (testcorpus >> word) {
        if (chrono::duration_cast<S>(dur).count()*WPS < WordsSinceLastGen) {
            // Regenerate TopK
            WordsSinceLastGen = 0;
            tp = chrono::high_resolution_clock::now();
            topk.Generate(ngram);
            dur = chrono::high_resolution_clock::now() - tp;
            cout << "Top-K Build Time: " << chrono::duration_cast<S>(dur).count() << " s ("  << dur.count() << " ns)" << endl;
        }
        istringstream iss(word);
        iss >> ngram;
        total++;
        tkeys += word.size();
        if (find(begin(predicted[0]),end(predicted[0]),word) != end(predicted[0])) {
            found[0]++;
            keyss[0] += word.size();
        } else {
            WordsSinceLastGen++;
        }
        if (find(begin(predicted[1]),end(predicted[1]),word) != end(predicted[1])) {
            found[1]++;
            keyss[1] += word.size();
        }
        prefix.Next(word);
        predicted[0] = topk.GetTopK(prefix);
        predicted[1] = ngram.TopK<K>(prefix);
    }
    cout << "Total Words: " << total << endl;
    cout << "Predicted Words  (Top-K): " << found[0] << endl;
    cout << "Predicted Words (N-Gram): " << found[1] << endl;
    cout << "Hit Rate  (Top-K): " << found[0] / static_cast<double>(total) << endl;
    cout << "Hit Rate (N-Gram): " << found[1] / static_cast<double>(total) << endl;
    cout << "Total Keys: " << tkeys << endl;
    cout << "Saved Keys  (Top-K): " << keyss[0] << endl;
    cout << "Saved Keys (N-Gram): " << keyss[1] << endl;
    cout << "Key SS  (Top-K): " << keyss[0] / static_cast<double>(tkeys) << endl;
    cout << "Key SS (N-Gram): " << keyss[1] / static_cast<double>(tkeys) << endl;

    return 0;
}
