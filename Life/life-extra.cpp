// Author: Snehal Patil.
// Homework 1: Game of life Extra Features.
// Features Added:
// 1. Creating a Random Colony
// 2. Display animated GRID


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void introMessage(); // Prints Intro Messages Only
void createGridfromfile();// Display the Updated Grid based on user inputs
Grid<char> updateGrid(int row, int col, Grid<char>& matrix);// Reads the user input .txt file and copies to a GRID
int updateCount(int r, int c,int count,int nr, int nc, Grid<char>& matrix_temp );// Updates the Grid based on user Input
char checkInput(string userinput);   // Function to check for invalid input form user
void displayNewgrid(Grid<char>& matrix_display,string line ); // FUnction that displays the new grid by creating a temp file and destroying it after grid displayed
Grid<char> createRandomgrid(int row, int col);// Function to create Random Grid
void createLifegui(LifeGUI& gui,Grid<char>& matrix_display); // Function to create Life GUI

int main() {
    introMessage();
    createGridfromfile();
    cout << "Have a nice Life!" << endl;
    return 0;
}

/*Defn: Function to Print Intro messages*/

void introMessage(){
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << "- Enter 'random' if you want to creat a random grid." << endl;
}

/*Defn: Function to Create Grid by reading the specified user input file*/
void createGridfromfile() {
    string line;
    char userinput;
    string filename=getLine("Grid input file name?");
    ifstream input;
    input.open(filename);
    bool checkifrandom=(filename != "random");
    while ((input.fail())&& checkifrandom ){
        cout << "Unable to open that file.  Try again." << endl;
        filename=getLine("Grid input file name?");
        input.open(filename);
    }
    int row=0;
    int col=0;
    LifeGUI gui;
    Grid<char> matrix(row, col);
    Grid<char> matrix_display(row, col);
    matrix_display.fill('-');

    if (filename=="random"){
        row = randomInteger(3,10);
        col = randomInteger(3,10);
        matrix.resize(row, col);
        matrix_display.resize(row, col);
        matrix=createRandomgrid(row,col);
        matrix_display=updateGrid(row, col,matrix);
        displayNewgrid(matrix_display,line );
        gui.resize(row, col);
        createLifegui(gui,matrix_display);
    }else {
        getline(input, line);
        row = stringToInteger(line);
        getline(input, line);
        col = stringToInteger(line);
        matrix.resize(row, col);
        for (int i = 0; i < matrix.height(); i++) {
            getline(input, line);
            cout << line << endl;
            for (int j = 0; j < line.size(); j++) {
                matrix[i][j]= line[j];
            }
        }
        input.close();
        gui.resize(row, col);
        createLifegui(gui,matrix);
    }
    string getinputfromuser=getLine("a)nimate, t)ick, q)uit?");
    userinput=checkInput(getinputfromuser);
    while( userinput!='q' && userinput!='Q' ){
        Grid<char> matrix_display(row, col);
        matrix_display.fill('-');
        if(userinput=='t' || userinput=='T') {
            matrix_display=updateGrid(row, col,matrix);
            displayNewgrid(matrix_display,line );
            gui.resize(row, col);
            createLifegui(gui,matrix_display);
            getinputfromuser=getLine("a)nimate, t)ick, q)uit?");
            userinput=checkInput(getinputfromuser);
        } else if (userinput=='a' || userinput=='A'){
            string prompt,reprompt;
            int inputframenumber=getInteger( prompt = "How many frames?" , reprompt="Illegal integer format. Try again." );
            for (int i=0;i<inputframenumber;i++){
                matrix_display=updateGrid(row, col,matrix);
                displayNewgrid(matrix_display,line );
                pause(50);
                clearConsole();
                gui.resize(row, col);
                createLifegui(gui,matrix_display);
            }
            getinputfromuser=getLine("a)nimate, t)ick, q)uit?");
            userinput=checkInput(getinputfromuser);
        }  else {
            getinputfromuser=getLine("a)nimate, t)ick, q)uit?");
            userinput=checkInput(getinputfromuser);
        }
    }
}


/*Defn: Function to update the grid by implementing rules to create or destroy bacteria*/
/*Return: Takes in row,col and Grid from createGridfromfile(), peforms the rules update and returns the updated Grid */
/*Algorithm for finding the neighbors: I divide the matrix in to three main sections to count the neighbors
1. Count neighbor for corner most elements (right top, left top, right bottom.left bottom)
2. Count neighbor for the top row, bottom row (excluding 1)
3. Count neighbor of right col, left col
4. Count neighbor of remainig elements
*/

Grid<char> updateGrid(int row, int col,Grid<char>& matrix ){
    Grid<char> matrix_temp(row, col);
    matrix_temp.fill('-');
    Grid<char> matrix_final(row, col);
    matrix_final.fill('-');
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            matrix_temp[i][j]= matrix[i][j];
        }
    }
    for (int r = 0; r < matrix.height(); r++) {
        for (int c = 0; c < matrix.width(); c++) {
            int count=0;
            if (r==0 && c==0){
                for (int nr = (r); nr <= r + 1; nr++) {
                    for (int nc = (c); nc <= c + 1; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }
            else if ((r==matrix.height()-1) && (c==matrix.width()-1)){
                for (int nr = (r-1); nr <= r ; nr++) {
                    for (int nc = (c-1); nc <= c ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }  else if (r==0 && c==(matrix.width()-1)){
                for (int nr = (r); nr <= r+1 ; nr++) {
                    for (int nc = (c-1); nc <= c ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }  else if (r==(matrix.height()-1) && c==0){
                for (int nr = (r-1); nr <= r ; nr++) {
                    for (int nc = (c); nc <= c+1 ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }  else if (r==0 && c > 0){
                for (int nr = (r); nr <= r+1 ; nr++) {
                    for (int nc = (c); nc <= c+1 ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }   else if (r>0 && c == 0){
                for (int nr = (r-1); nr <= r+1 ; nr++) {
                    for (int nc = (c); nc <= c+1 ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }  else if ( r==(matrix.height()-1) && c > 0){
                for (int nr = (r-1); nr <= r ; nr++) {
                    for (int nc = (c); nc <= c+1 ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }   else if ( r>0 && c == (matrix.width()-1)){
                for (int nr = (r-1); nr <= r ; nr++) {
                    for (int nc = (c-1); nc <= c ; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }
            else {
                for (int nr = (r - 1); nr <= r + 1; nr++) {
                    for (int nc = (c - 1); nc <= c + 1; nc++) {
                        count= updateCount(r, c,count ,nr,nc, matrix_temp);
                    }
                }
            }
            if(count <=1 || count >=4 ){
                matrix_final[r][c]= '-';
            }  else if((count ==2)){
                matrix_final[r][c]= matrix_temp[r][c];
            }  else if((count ==3)){
                matrix_final[r][c]= 'X';
            }
        }
    }
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            matrix[i][j]=matrix_final[i][j];
        }
    }
    return matrix;
}


/*Defn: Function to count the number of neighbors for the specified */
/*Return: Takes in row,col, neighboring row and col, Grid from createGridfromfile(), performs a count on the cells present excluding the cell
 under under test and returns the coun to caller*/
int updateCount(int r, int c,int count,int nr, int nc, Grid<char>& matrix_temp ){
    char temp=matrix_temp[r][c];
    matrix_temp[r][c]='-';
    if ((matrix_temp[nr][nc]=='X')){
        count++;
    }
    matrix_temp[r][c]=temp;
    return count;

}

/*Defn: Function to print error message and get upper and lower case letters from user */
/*Return: The userinput*/
char checkInput(string userinput){
    int checklength= (userinput.length());
    bool checkcharachter=((userinput[0] != 'a') && (userinput[0] != 't') && (userinput[0] != 'q' ) && (userinput[0] != 'A') && (userinput[0] != 'T') && (userinput[0] != 'Q' ));
    while(checkcharachter || (checklength > 2) ){
        checklength= (userinput.length());
        checkcharachter=((userinput[0] != 'a') && (userinput[0] != 't') && (userinput[0] != 'q' ) && (userinput[0] != 'A') && (userinput[0] != 'T') && (userinput[0] != 'Q' ));
        cout << "Invalid choice; please try again." << endl;
        string getinputfromuser=getLine("a)nimate, t)ick, q)uit?");
        userinput=checkInput(getinputfromuser);
        checklength= (userinput.length());
        checkcharachter=((userinput[0] != 'a') && (userinput[0] != 't') && (userinput[0] != 'q' ) && (userinput[0] != 'A') && (userinput[0] != 'T') && (userinput[0] != 'Q' ));
    }
    return userinput[0];
}

/*Defn: Function that displays the final grid. It takes in the updated grid and prints it out to a temp file */
void displayNewgrid(Grid<char>& matrix_display,string line ){
    ofstream myfile;
    myfile.open ("temp.txt");
    for (int i = 0; i < matrix_display.height(); i++) {
        for (int j = 0; j < matrix_display.width(); j++) {
            if ((matrix_display[i][j]=='-') ||(matrix_display[i][j]=='X')){
                myfile << matrix_display[i][j];
            }
        }
        myfile << '\n';
    }
    myfile.close();
    ifstream display;
    display.open("temp.txt");
    while (getline(display, line)) {
        cout << line << endl;
    }
    display.close();
}

/*Defn: Function to create a random grid */
/*Return: Returns a random matrix*/
Grid<char> createRandomgrid(int row,int col){
    int rand = randomInteger(0,2);
    Grid<char> matrix_random(row, col);
    for (int i = 0; i < matrix_random.height(); i++) {
        rand = randomInteger(0,1);
        for (int j = 0; j < matrix_random.width(); j++) {
            rand = randomInteger(0,1);
            if(rand == 0){
                matrix_random[i][j]='-';

            }else {
                matrix_random[i][j]='X';
            }
        }
    }
    return matrix_random;
}

/*Defn: Function to create a Life GUI */
void createLifegui(LifeGUI& gui,Grid<char>& matrix_display){

    for (int i = 0; i < matrix_display.height(); i++) {
        for (int j = 0; j < matrix_display.width(); j++) {
            if (matrix_display[i][j]=='X'){
                gui.drawCell(i,j, true);
            } else {
                gui.drawCell(i, j, false);
            }
        }
    }
}


