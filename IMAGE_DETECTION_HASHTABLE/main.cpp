#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define Loading_F 0.5 // I Defined loading factor (number of elements/size) and assigned it as 0.5 to use in rehash function
//HashTable implementation made from HashTables.pdf and HashTables2.pdf we used both of them in our lectures

//I handled collusions by using lineer probing method. So, I mostly used HashTables2.pdf which we mostly talked about lineer probing solution

//I erased remove and operator= functions from the class because I don't use them.
class HashTable {
public:
    explicit HashTable(const string &notFound, int size = 101)
        : ITEM_NOT_FOUND(notFound), array(nextPrime(size)) {
        makeEmpty();
    }

    HashTable(const HashTable &rhs) //construct hashtable
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
          array(rhs.array), currentSize(rhs.currentSize) {}
    //Return true if currentPos exists and is active.
    //Return the matching item, or ITEM_NOT_FOUND, if not found
    const string &find(const string &x) const {
        int currentPos = findPos(x);
        if (isActive(currentPos)){
            return array[currentPos].element;
        }
        return ITEM_NOT_FOUND;
    }
    //Make the hash table logically empty.
    void makeEmpty() {
        for (int i = 0; i < array.size(); ++i) {
            array[i].info = EMPTY;
        }
    }

    //Insert item x into the hash table. If the item is already present, then do nothing.
    void insert(const string &x) {
        // Insert x as active
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return;
        array[currentPos] = HashEntry(x, ACTIVE);

        // Enlarge the hash table if necessary
        if (++currentSize >= array.size() * Loading_F)
            rehash();
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

private:
    struct HashEntry {
        string element;
        EntryType info;

        HashEntry(const string &e = string(), EntryType i = EMPTY)
            : element(e), info(i) {}
    };

    vector<HashEntry> array;
    int currentSize;
    const string ITEM_NOT_FOUND;

    bool isActive(int currentPos) const {
        return array[currentPos].info == ACTIVE;
    }
    //Method that performs quadratic probing resolution.
    //Return the position where the search for x terminates.
    int findPos(const string &x) const {
        int num = 1;
        int currentPos = hash(x, array.size());

        while (array[currentPos].info != EMPTY && array[currentPos].element != x) {
            currentPos += num; // Quadratic probing
            num += 2;
            if (currentPos >= array.size())
                currentPos -= array.size();
        }
        return currentPos;
    }
    //Expand the hash table.
    void rehash() {
        vector<HashEntry> oldArray = array;

        // Create a new table with size (double-sized but the next prime).
        array.resize(nextPrime(2 * oldArray.size()));

        for (int j = 0; j < array.size(); j++) {
            array[j].info = EMPTY; // Mark all new positions as EMPTY
        }

        // Copy table over
        currentSize = 0; // Reset current size
        for (int i = 0; i < oldArray.size(); i++) {
            if (oldArray[i].info == ACTIVE) {
                insert(oldArray[i].element); // Reinsert only active elements
            }
        }
    }
    //It computes a polynomial with the characters as the coefficients and 37 as the variable value, using Horner’s method.
    int hash(const string &key, int tableSize) const {
        int hashVal = 0;
        for (char ch : key)
            hashVal = 37 * hashVal + ch;

        hashVal %= tableSize;
        if (hashVal < 0){
            hashVal += tableSize; //Since overflows may occur, any negative sum is fixed here.
        }
        return hashVal;
    }
    // Internal method to return a prime number at least as large as n.
    int nextPrime(int n) {
        if (n % 2 == 0)
            n++;
        while (!isPrime(n))
            n += 2;
        return n;
    }
    // Internal method to test if a positive number is prime.
    bool isPrime(int n) {
        if (n == 2 || n == 3)
            return true;
        if (n == 1 || n % 2 == 0)
            return false;
        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0)
                return false;
        return true;
    }
};

string RLE(const string &flattened) {
    string rle = "";        // Initialize an empty string for the RLE result
    int count = 1;          // Initialize count as 1
    char current = flattened[0]; // Set current to the first character of the flattened input
    
    for (int i = 1; i < flattened.size(); i++) { //for each character in flattened starting from the second do
        if (flattened[i] == current) { //if character equals current then
            count++; // Increment count by 1
        } else {
            //Append count and current (as ”W” or ”B”) to rle
            rle += to_string(count);
            if(current == '0'){
                rle += "B";
            }
            else{
                rle += "W";
            }
            current = flattened[i]; //Set current to the new character
            count = 1; // Reset count
        }
    }
    // Append count and current (as ”W” or ”B”) to rle (for the last sequence)
    rle += to_string(count);
    if(current == '0'){
        rle += "B";
    }
    else{
        rle += "W";
    }
    return rle; //return rle
}


//now we should flatten the image. For RLE algorithm we need flatten function
string flatten(const string &filen, bool original_version) {
    ifstream file(filen);
    if (!file.is_open()) {
        return "";
    } //if we cannot open the file return empty string

    string flattened = "", line;
    while (getline(file, line)) {
        string temp = "";
        for(int i=0; i<line.size(); i++){
            if(line[i] != '\r'){
                temp += line[i];
            } //here I removed '\r' characters from the line because when we don't do this it redundantly prints r in end of each line.
        }
        if(original_version){ //If the original version is true, it keeps the line and adds space for the next line
            flattened += temp + '\n';
        }
        else{
            //if original version is false then we combine lines into a single string.
            flattened += temp;
        }
    }
    file.close();
    return flattened; //closed the file and returned flattened string.
}


string write_image(const string &file_name){
    //flattened the string to remove line structures from Rle encoding
    string flattened = flatten(file_name,false);
    if(flattened.empty()){
        return ""; //if flatened string is empty then we return empty string.
    }
    string rle =RLE(flattened);
    return rle;
}

void Imageinsert(HashTable &hashTable){
    string inp;
    bool is_query = false; //I will use it to quit when user enter query
    while(!is_query){
        cout<<"Enter image number to insert into the hash table (or 'query' to continue):"<<endl;
        cin>>inp;
        if(inp=="query"){
            //exit loop if user enters query
            is_query=true;
        }
        else{
            //for corrected file name
            string file_name = "image"+inp+".txt";
            string rle = write_image(file_name);
            if(!rle.empty()){
                //inserting rle encoding into hash table
                hashTable.insert(rle);
                cout<<"Image "<<inp<<" inserted into the hash table."<<endl;
            }
        }
    }
}
void asked_query(HashTable &hashTable){
    string inp;
    bool is_exit = false; //I will use it for quiting when user enters exit as input
    while(!is_exit){
        cout<<"Enter image number to query (or 'exit' to quit):"<<endl;
        cin>>inp;
        if(inp=="exit"){
            is_exit=true; //if input is exit then exit
        }
        else{
            //for corrected file name
            string file_name = "query"+inp+".txt";
            
            //In this part I get the both original and flattened versions of query image
            string original_version = flatten(file_name,true);
            string flattened = flatten(file_name,false);
            if(!flattened.empty()){
                //generating RLE encoding in this part
                string rle = RLE(flattened);
                const string &result = hashTable.find(rle); //trying to find the rle in hash table (searching it) O(1)
                
                if(result != "ITEM_NOT_FOUND"){
                    //if string is found then we print the original version of the image.
                    cout << "RLE String for " << file_name << " found in hash table." << endl;
                    cout<<original_version;
                }
                
                else{
                    //if string is not found then we send a error mesage to user.
                    cout << "No match for the image with encoding: " << rle << "\n";
                }
            }
        }
    }
}

int main()
{
    HashTable hashTable("ITEM_NOT_FOUND"); //here by using constructor that I created in my HashTable implementation, I created a default not found hash table.
    
    //Then inserted the image by using the function that I created before.
    Imageinsert(hashTable);
    
    //In this part I checked images in hash table.
    asked_query(hashTable);
    
    //final cout statment
    cout<<"Exiting the program!"<<endl;
    
    return 0;
}
