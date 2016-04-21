// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "map.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"

using namespace std;
void introMessage();

int main() {
    introMessage();
    Queue <string> q;
    Map<Vector<string>, string> wordmap;
    string filename=toLowerCase(getLine("Input file name?"));
    ifstream input;
    string word;
    input.open(filename);
    while (input.fail()){
        cout << "Unable to open that file.  Try again." << endl;
        filename=getLine("Dictionary file name?");
        input.open(filename);
    }
    while (input >> word){
        q.enqueue(word);
    }

    input.close();

    cout << q << endl;
    int N=stringToInteger(getLine("Value of N?"));


    Vector<string> temp;
    for (int i =0 ; i<= (N); i++){
        if(!q.isEmpty()){
            temp.insert(i,q.dequeue());
        }
    }

    cout << temp << endl;

    wordmap.put(temp.subList(0, 1), temp[N]);


    cout << wordmap << endl;
    return 0;
}

void introMessage(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

