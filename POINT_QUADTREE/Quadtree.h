//
// Created by İsmail Yigit Onal 31214
//

#ifndef TAKEHOME2_QUADRANT_H
#define TAKEHOME2_QUADRANT_H

#include <iostream>
#include <vector>
#include <cmath> //while I apply euclidan algorithm I use functions that belongs to cmath class
using namespace std;

enum Quadrants{NW, NE, SW, SE};

template <class Comparable>
class QuadTree;

template <class Comparable> //template implementation taken by Trees1.pdf
class QuadTreeNode{ //like we do in BST (first create a class for a node
public:
    int x_c;
    int y_c; //x and y coordinates of city

    Comparable city_name; // name of the city

    QuadTreeNode *NW;
    QuadTreeNode *NE;
    QuadTreeNode *SW;
    QuadTreeNode *SE; //pointers to child nodes I implemented it like in give pdf

    QuadTreeNode(int x, int y, const string& name) : x_c(x), y_c(y), city_name(name), NE(NULL), NW(NULL), SE(NULL), SW(NULL) {}
    friend class QuadTree<Comparable>;
};

template <class Comparable>
class QuadTree{ //then create a class for the data structure
private:
    QuadTreeNode<Comparable> *root; //root (first node)

    //codes taken by homework files while writing and creating the algorithms, Pseudocode format of the algorithms given in Point Quadtrees.pdf file implemented as the code format in this cpp file.

    Quadrants PT_COMPARE(QuadTreeNode<Comparable> *P , QuadTreeNode<Comparable> *R){//compare implementation taken by quadranttree.pdf SE, SW, NE, NW
        if(P->x_c < R->x_c){
            if(P->y_c < R->y_c){
                return SW; //south-west (down left)
            }
            else{
                return NW; //North-west (up left)
            }
        }
        else{
            if(P->y_c < R->y_c){
                return SE; //South-east (down right)
            }
            else{
                return NE; //north-east (up right)
            }
        }
    } //by using enumerators we provided function to return SW NW SE NE enum's ( these are the quadrants while we compare the nodes between)
    void PT_INSERT(QuadTreeNode<Comparable> *&R , QuadTreeNode<Comparable> *P){ // I passed R by reference because if the original root pointer update outside the code blocks it updates works as an update in this code block too.

        if(R==NULL){ //if there is no root or node it is inserting P into tree.
            R=P;
            return;
        }

        Quadrants Q = PT_COMPARE(P, R);

        //traversing tree again and again to find any free spot that we can place the P
        if(Q == SE){
            PT_INSERT(R->SE, P);
        }
        else if(Q == SW){
            PT_INSERT(R->SW, P);
        }
        else if(Q == NE){
            PT_INSERT(R->NE, P);
        }
        else{
            PT_INSERT(R->NW, P);
        }

    } //logic is same as quadrant file
public:
    QuadTree(){
        root = NULL;
    } //constructor for the quadtree

    void PUB_INSERT(int x, int y, const Comparable& city_name){ //Insert function for the public identified 2 insert public and private insert like given in the lectures of CS300.
        QuadTreeNode<Comparable> * new_node = new QuadTreeNode<Comparable>(x,y,city_name);
        if (root==NULL){
            root=new_node;
        }
        else{
            PT_INSERT(root, new_node);
        }

    } //this public insert function works with the Check_city in the source file they provide adding each city to quadtree.
    void PT_REGION_SEARCH(QuadTreeNode<Comparable> *quad_node, int x, int y, int radius,
                          vector<Comparable> &found_city, vector<Comparable> &visited_city) const {
        if (quad_node == NULL) {
            return; // no node to process in quadtree
        }

       //for visiing the current node
        visited_city.push_back(quad_node->city_name);

       //calculating the distance by using euclidian algorithm
        float distance = sqrt(pow(quad_node->x_c - x, 2) + pow(quad_node->y_c - y, 2));
        if (distance <= radius) {
            found_city.push_back(quad_node->city_name); //if the location of the city within radius add it into found cities.
        }
        //The working order is like given takehome file (SE, SW, NE, NW).
        // for South-east quadrant
        if (quad_node->SE && x + radius >= quad_node->x_c && y - radius <= quad_node->y_c) {
            PT_REGION_SEARCH(quad_node->SE, x, y, radius, found_city, visited_city);
        }

        // for south-west quadrant
        if (quad_node->SW && x - radius <= quad_node->x_c && y - radius <= quad_node->y_c) {
            PT_REGION_SEARCH(quad_node->SW, x, y, radius, found_city, visited_city);
        }

        // for north-east quadrant
        if (quad_node->NE && x + radius >= quad_node->x_c && y + radius >= quad_node->y_c) {
            PT_REGION_SEARCH(quad_node->NE, x, y, radius, found_city, visited_city);
        }

        // for north-west quadrant
        if (quad_node->NW && x - radius <= quad_node->x_c && y + radius >= quad_node->y_c) {
            PT_REGION_SEARCH(quad_node->NW, x, y, radius, found_city, visited_city);
        }
    }

    void pretty_print(QuadTreeNode<Comparable> *quad_node) const{ // Pretty print the quadtree rooted at root
        if(quad_node==NULL){
            return; //if there is no node do nothing.
        }
        cout<<quad_node->city_name<<endl;
        //this part assume root is not null

        pretty_print(quad_node->SE); //Recursively print the southeast subtree
        pretty_print(quad_node->SW); // Recursively print the southwest subtree
        pretty_print(quad_node->NE); // Recursively print the northeast subtree
        pretty_print(quad_node->NW); // Recursively print the northwest subtree
    } //again taken from homework file

    void Tree_Print() const{
        pretty_print(root);
    } // this is the complete function of the pretty print which we use in the outputs first part. printing the cities.

    void Do_Query(int x, int y, int radius) const {
        vector<Comparable> foundCities;        // Stores cities within the radius
        vector<Comparable> visitedCities;      // Stores all cities visited during the search

        //for performing the region search for the vectors
        PT_REGION_SEARCH(root, x, y, radius, foundCities, visitedCities);

        // Output the list of cities within the radius, or <None> if none are found
        if (foundCities.empty()) {
            cout << "<None>" << endl;
        } else {
            for (int i = 0; i < foundCities.size(); i++) {
                cout << foundCities[i];
                if (i < foundCities.size() - 1) cout << ", ";
            }
            cout << endl;
        }

        // Output the visited cities in comma-separated format
        for (int i = 0; i < visitedCities.size(); i++) {
            cout << visitedCities[i];
            if (i < visitedCities.size() - 1) cout << ", ";
        }
        cout << endl;
    } //works with the Queries_File function on the source file

};

#endif //TAKEHOME2_QUADRANT_H
