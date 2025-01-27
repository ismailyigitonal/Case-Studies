//Ismail Yigit Onal 31214

#include <iostream>
#include <vector>
#include "stack.h"
#include <fstream>

using namespace std;
struct Queen{
    int row, col;
    Queen (int r, int c) : row(r), col(c) { }  //while we implement queen's positions into the stack we will use this.
}; //structure for saving positions of queen

bool SafetyCheck(vector<vector<int>>& safeM, int row, int col){
    if(safeM[row][col]==0){
        return true; //means it is safe, so another queen can be added into the chess board (0 = safety)
    }
    return false; //means it is not safe
} //works together with SafetyUpdation

void SafetyUpdate(vector<vector<int>>& safeM, int row, int col, int input){
    //normally in default matrix every cell is 0 ,so it has NxN 0's. However, we will fill 1'S after putting queen into rows if there is danger
    for(int i=0; i<input; i++){
        safeM[i][col] += 1; //complete column donated 1
        safeM[row][i] += 1; //complete row donated with 1

        if (row + i < input && col + i < input) safeM[row + i][col + i] += 1;  // Top left to bottom right diagonal donated 1
        if (row - i >= 0 && col - i >= 0) safeM[row - i][col - i] += 1;  // Bottom right to top left diagonal donated 1
        if (row + i < input && col - i >= 0) safeM[row + i][col - i] += 1;  // Top right to bottom left diagonal donated 1
        if (row - i >= 0 && col + i < input) safeM[row - i][col + i] += 1;  // Bottom left to top right diagonal donated 1
    }
    //safety matrix is to control and optimization
}
void Unsafe(vector<vector<int>>& safeM, int row, int col, int input){
    //for decrementing when we pop the queen ( guarding the safe matrix)
    for(int i=0; i<input; i++){
        safeM[i][col] -= 1;
        safeM[row][i] -= 1;

        if (row + i < input && col + i < input) safeM[row + i][col + i] -= 1;
        if (row - i >= 0 && col - i >= 0) safeM[row - i][col - i] -= 1;
        if (row + i < input && col - i >= 0) safeM[row + i][col - i] -= 1;
        if (row - i >= 0 && col + i < input) safeM[row - i][col + i] -= 1;
    } //works in same logic with SafetyUpdate just to functionalize the things and do not use too much number.
}
void ZeroP(vector<vector<int>>& safeM, vector<vector<int>>&chessBoard, int input){
    //function to fill 2D vectors with 0's
    for(int i=0; i<input; i++){
        vector<int> v1;
        for(int j=0; j<input; j++){
            v1.push_back(0);
        }
        safeM.push_back(v1);
        chessBoard.push_back(v1);
    } //got more then 1 vector to fill with 0's so it is better to fill with that function.
}
void PrintRec(vector<vector<int>> validsolution, int input){
    //Print Function for printing the solutions into txt file
    int a=validsolution.size(); //how much solution I have
    ofstream myFile;
    string fileName = to_string(input)+"queens_solutions.txt";
    myFile.open(fileName, ios::out);
    if(myFile.is_open()){
        myFile<<"Total solutions for N="<<input<<": "<<a<<endl;
        myFile<<endl;
        for(int i=0; i<a; i++){
            myFile<<"Solution "<<i+1<<": ["; //solution name
            for(int j=0; j<input; j++){
                myFile<<validsolution[i][j];
                if(j !=input-1){
                    myFile<<", ";
                }
            }
            myFile << "]" <<endl;
        }
        myFile.close();
    }

}
void Solving_func(int input){
    vector<vector<int>> chessBoard;
    vector<vector<int>> safeM;
    ZeroP(safeM, chessBoard, input); //filled the vectors with 0 by using function
    Stack<Queen> s;// stack for store the cells that we put the queens in.
    vector<vector<int>> validsolution; // 2d vector for storing only valid solutions.
    vector<int> whichrow(input, -1); //1d vector for storing all the columns of the solutions
    int row=0; // set row into 0 because we will start from the index 0

    bool maxfound=false;
    while(maxfound==false){
        bool findrow =false;
        for(int column=whichrow[row]+1; column<input; column++){
            if(SafetyCheck(safeM, row, column)==true){
                chessBoard[row][column]=1; //added the queen into the chessboard
                SafetyUpdate(safeM, row, column, input); //updated the safety
                s.push(Queen(row,column)); //pushed current (row,column) into the stack for backtracking
                whichrow[row]=column; //added current column to 1d vector to store solution
                findrow = true; //found a position in current row
                break;
            }
        }
        int last_row = input-1; //if we can place a queen into the last row it means we found a solution.
        if(findrow==true){
            if(row==last_row){ //if current row is in the last row then we found a solution
                validsolution.push_back(whichrow);//pushed the valid solution into the vector;
                Queen last = s.top(); //took the element at the top (the queen at the latest row in the feasible solution)
                s.pop();
                chessBoard[last.row][last.col] = 0; //removed queen from the original board
                Unsafe(safeM, last.row, last.col, input); //decrement the value (updated the safe matrix)
                row = last.row; //updated the row with last queens row
                whichrow[row]=last.col; //we are storing columns for the answer
            }
            else{
                row++; //jump into the next row
                whichrow[row]=-1; // set -1 again the current
            }
        }
        else{
            if(s.isEmpty()){
                break; //there is no position in the current row and stack is empty then break
            }
            Queen last = s.top();
            s.pop(); //removing the queen from the row
            chessBoard[last.row][last.col] = 0;
            Unsafe(safeM, last.row, last.col, input); //used unsafe because we removed queen
            row = last.row; //backtracking and getting back into the previous row
            whichrow[row]=last.col;
        }
    }
    PrintRec(validsolution, input);
}
int main() {
    int input;
    cout << "Enter the value of N for the N-Queens problem: ";
    cin >> input;

    if (input <= 0) {
        cout << "Invalid input. Please enter a positive integer." << endl;
        return 0;
    }
    else{
        cout<<"Solutions have been saved to ’"<<input<<"queens_solutions.txt’";
        cout<<endl;
        Solving_func(input);
    }
    return 0;
}
