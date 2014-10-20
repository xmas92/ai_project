//
//  babygram.h
//  ai_project
//
//  Created by Niklas Rönnholm on 18/10/14.
//  Copyright (c) 2014 gcloud. All rights reserved.
//

#ifndef ai_project_babygram_h
#define ai_project_babygram_h


class Babygram{
    
public:
    //Constructor
    Babygram(int n, float q, Ngram g[]); /*PARAMETERS
                               * n will decide what maximum depth the n-grams will have. I.E. n=2 will support up to 2-grams
                               * q will decide for what quota of occurence it will store uncommon n-grams
                               * g will contain all the ngrams
                               */
    
    //Predict a word!
    string Search(string w[]); /*PARAMETERS
                                *  w what previous words w(in order) has been written
                                */
    
    //This method is run to add occurrence of a word
    bool writeWord(string s);   /*
                                 * if the word s occurs in the trie, add its occurrence index, else
                                 * increase the unknown word counter. if the unknown word counter
                                 * exceeds the word counter, the trie needs to be reconstructed and
                                 * the method will return false. if all is ok, it returns true.
                                 */
private:
    int maximum_occurrence;
    int unknown_occurrence_counter;
    tnode trie_root_node;
    
    void addNgram(Ngram n);
}

Babygram::Babygram(int n, float q, Ngram g[]){
    trie_root_node = new tnode();
    for (int i = 0; i<g.size(); i++) {
        if(g[i].occurenceCount>maximum_occurrence){
            maximum_occurrence = g[i].occurenceCount;
        }
    }
    maximum_occurrence = maximum_occurrence*q;
    
    for (int i = 0; i<g.size(); i++) {
        if(g[i].occurenceCount>maximum_occurrence){
            addNgram(g[i]);
        }
    }
}

Babygram::addNgram(Ngram n){
    //When adding one n-gram, all other n-grams of lower n have already been added.
    //Search for the penultimate word in the given n-gram, and add
    //the last word in the n-gram with the n-grams occurrence count.
}


struct tnode {
    string word;            //word in the trie
    int occurrence;         //occurrence count of the ngram down to this word
    vector<tnode> subnodes;      //this contains the following words, is null if node is leaf
}


#endif
