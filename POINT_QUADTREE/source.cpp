//Ismail Yigit Onal 31214
//SOURCE.CPP file 

#include <iostream>
#include "Quadtree.h"
#include <fstream> //for reading the txt files and check
#include <sstream>
#include <string>
using namespace std;
template <class Object>
void Check_City(QuadTree<Object>& quadTree, const string& fname){ //this function is going to work for cities.txt it will check and load information.
    ifstream city_file(fname);
    if(!city_file.is_open()){
        cout<<"File "<<fname<<" is not open."<<endl; //give the error message specified for file
        return; //stop the program
    }

    int x_c, y_c;
    city_file >> x_c >> y_c;


    string  city_name;
    int x, y;
    while(city_file>>city_name>>x>>y){ //while it reads the coordinates and name from the file it inserts the elements into quadtree
        quadTree.PUB_INSERT(x, y, city_name);
    }
    city_file.close(); //at the end after the process end we close the file.
}
template<class Object>
void Queries_File(QuadTree<Object>& quadTree, const string& fname){ //this function is going to work for queries0.txt it will check and load information.
    ifstream queries_file(fname);
    if(!queries_file.is_open()){
        cout<<"File "<<fname<<" is not open."<<endl; //give the error message
        return; //stop the program
    }
    string line;
    while (getline(queries_file, line)) {  //getting the query file line by line in x y and radius format
        istringstream iss(line);
        int x, y, radius;
        char comma; // for ignoring commas

        if ((iss >> x >> comma && comma == ',' && iss >> y >> comma && comma == ',' && iss >> radius)) {
            quadTree.Do_Query(x, y, radius);
            cout << endl;
        }
    } // is a way to take x and y and radius and it is a way to ignore the commas ( ,) by this function I am able to seperate the commas.

    queries_file.close();
}

int main(){
    QuadTree<string> quadTree;
    Check_City(quadTree, "cities.txt"); //Loading cities.txt file

    quadTree.Tree_Print();
    cout<<endl;

    Queries_File(quadTree, "queries0.txt"); //Loading queries0.txt file

    return 0;
}
