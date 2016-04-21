// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"


using namespace std;

void introMessage(); // Prints Intro Messages Only
void createDictfromfile();
void printErrormessages(string word1, string word2, Lexicon& dictionary);
Vector<string>  possibleNeighbor(string word1, Lexicon& dictionary );
Vector<string>  removeWordfromvec(string word, Vector<string> ladder);
bool wordPresent(Queue< Vector<string> > q, string word);


int main() {
    introMessage();

    createDictfromfile();

    cout << "Have a nice day." << endl;
    return 0;
}


/*Defn: Function to Print Intro messages*/
void introMessage(){
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
}

/*Defn: Create a Lexicon from input dictionary filename*/
void createDictfromfile() {
    Lexicon dictionary;
    string filename=toLowerCase(getLine("Dictionary file name?"));
    ifstream input;
    input.open(filename);
    while (input.fail()){
        cout << "Unable to open that file.  Try again." << endl;
        filename=getLine("Dictionary file name?");
        input.open(filename);
    }
    dictionary.addWordsFromFile(filename);
    cout <<""<< endl;
    string word1=toLowerCase(getLine("Word #1 (or Enter to quit):"));
    if( (word1=="")  ) {
        return;
    }
    string word2=toLowerCase(getLine("Word #2 (or Enter to quit):"));
    if( (word2=="")  ) {
        return;
    }
    printErrormessages(word1, word2, dictionary );
    Queue<Vector <string>> q;
    Vector<string> s,wordladder,copy;
    s.insert(0,word1);
    q.enqueue(s);
    while (!q.isEmpty()) {
        wordladder=q.dequeue();
        if (wordladder[wordladder.size()-1]==word2) {
            cout<<"A ladder from "<<word2 <<" back to "<<word1<<":"<< endl;
            for (int i=wordladder.size()-1; i >=0 ; i--) {
                cout<<wordladder[i]<< " ";
            }
            cout<<endl;
            return;
        } else {
            for (string word : possibleNeighbor(wordladder[wordladder.size()-1], dictionary)){
                if (!wordPresent(q, word)) {
                    copy=wordladder;
                    copy.add(word);
                    q.enqueue(copy);
                }
            }
        }
    }
    cout<<"No ladder found\n";

}

/*Defn: Check words and print error messages accordingly*/
void printErrormessages(string word1, string word2, Lexicon& dictionary ){

        bool checkifwordsindict=(!dictionary.contains(word1) || !dictionary.contains(word2));
        while(checkifwordsindict){
            cout << "The two words must be found in the dictionary." << endl;
            cout <<""<< endl;
            word1=toLowerCase(getLine("Word #1 (or Enter to quit):"));
            word2=toLowerCase(getLine("Word #2 (or Enter to quit):"));
            checkifwordsindict=(!dictionary.contains(word1) || !dictionary.contains(word2));
        }
        bool checklengthofwords=!(word1.length()== word2.length());
        while(checklengthofwords){
            cout << "The two words must be the same length." << endl;
            cout <<""<< endl;
            word1=toLowerCase(getLine("Word #1 (or Enter to quit):"));
            word2=toLowerCase(getLine("Word #2 (or Enter to quit):"));
            checklengthofwords=!(word1.length()== word2.length());
        }
        bool chkdiffwords=(word1== word2);
        while(chkdiffwords){
            cout << "The two words must be different." << endl;
            cout <<""<< endl;
            word1=toLowerCase(getLine("Word #1 (or Enter to quit):"));
            word2=toLowerCase(getLine("Word #2 (or Enter to quit):"));
            checklengthofwords=!(word1.length()== word2.length());
        }


}

Vector<string> possibleNeighbor(string word1,Lexicon& dictionary ){
    Vector<string> vec;
    string alhabets="abcdefghijklmnopqrstuvwxyz";
    string temp=word1;
    for (int i = 0; i < word1.length(); i++){
        for (int j = 0; j < alhabets.length(); j++){
            temp[i]=alhabets[j];
            if  ((dictionary.contains(temp)) && temp != word1){
                vec.add(temp);
            }
        }
        temp=word1;
    }

    return vec;
}

bool wordPresent(Queue<Vector<string> > q, string word){
    int i=q.size();
    while (i >0) {
        Vector<string> vec=q.dequeue();
        for(int j=0;j < vec.size();j++){
            if (vec[j]==word) {
                return true;
            }
        }
        i--;
    }
    return false;
}


