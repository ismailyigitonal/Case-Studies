#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


template <class Object>
class Queue {
private:
    vector<Object> array;
    int front;
    int back;
    int count;
    int capacity;

    void doubleCapacity() {
        vector<Object> newArray(2 * capacity);
        for (int i = 0; i < count; ++i) {
            newArray[i] = array[(front + i) % capacity];
        }
        capacity *= 2;
        array = newArray;
        front = 0;
        back = count - 1;
    }

public:
    Queue(int size = 10) : capacity(size), front(0), back(-1), count(0) {
        array.resize(capacity);
    }

    void enqueue(const Object& x) {
        if (count == capacity) {
            doubleCapacity();
        }
        back = (back + 1) % capacity;
        array[back] = x;
        count++;
    }

    Object dequeue() {
        if (isEmpty()) {
            cout<<"Queue is empty"<<endl;
        }
        Object frontItem = array[front];
        front = (front + 1) % capacity;
        count--;
        return frontItem;
    }

    bool isEmpty() const {
        return count == 0;
    }

    Object getFront() const {
        if (isEmpty()) {
            cout<<"Queue is empty"<<endl;
        }
        return array[front];
    }
}; // I only declared the functions that I will use when using the queues in my algorithm.


bool checkisBipartite(vector<vector<int>>& adj, int V, vector<int>& color) {
    Queue<int> q;
    for (int i = 1; i <= V; i++) {
        if (color[i] == -1) {
            color[i] = 1;  //starting with color 1
            q.enqueue(i);
            while (!q.isEmpty()) {
                int node = q.dequeue();
                for (int j = 0; j < adj[node].size(); j++) {
                    int neighbor = adj[node][j];  // go neighbor using index
                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[node];  //asssigned the opposite color
                        q.enqueue(neighbor);
                    } else if (color[neighbor] == color[node]) {
                        return false;  //same color detected
                    }
                }
            }
        }
    }
    return true;  //no conflicts found
    // return bipartite
}


//I used Depth-first algorithm (DFS)
bool dfs(int u, vector<vector<int>>& adj, vector<int>& match, vector<bool>& visited) {
    for (int i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];  //v adjacent
        if (!visited[v]) {
            visited[v] = true;
            if (match[v] == -1 || dfs(match[v], adj, match, visited)) {
                match[v] = u;  // is a match
                return true;
            }
        }
    }
    return false;  //no augmentation
}


int maxBipartiteMatching(vector<vector<int>>& adj, int V) {
    vector<int> match(V, -1); //all vertices are unmatched all of them are -1
    int result = 0; //using it to find max matching size

    vector<bool> visited(V, false); //visited array to keep visited vertices during BFS.

    vector<bool> fromSetA(V, false); //boolean vector
    for (int i = 1; i < V; ++i) { // !! I started from 1 to avoid from sentinel
        if (!visited[i]) { //if vertex not visited
            Queue<int> q;
            q.enqueue(i);
            visited[i] = true;
            fromSetA[i] = true; // assign vertix to set a

            
            while (!q.isEmpty()) {
                int u = q.dequeue();
                for (int j = 0; j < adj[u].size(); j++) {
                    int v = adj[u][j];
                    if (!visited[v]) { //if the neighbor has not been visited.
                        visited[v] = true;
                        fromSetA[v] = !fromSetA[u];
                        q.enqueue(v);
                    }
                }
            }
        }
    }

    // resetting array here
    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }

    // try to find augmenting paths and increase the matching size.
    for (int u = 1; u < V; ++u) {
        if (fromSetA[u] && match[u] == -1) { //starting from unmatched vertix
            //resetting the visited matrix
            for (int i = 0; i < V; i++) {
                visited[i] = false;
            }
            // if there is a match add result
            if (dfs(u, adj, match, visited)) {
                result++;
            }
        }
    }

    return result; //total number matches
}




void readGraph(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found." << endl;
        return;
    } // if there is no corresponding file then print no found.

    int V, E;
    file >> V >> E;
    vector<vector<int>> adj(V + 1); // I used 2d array here
    vector<int> color(V + 1, -1);

    for (int i = 0; i < E; ++i) {
        int u, v;
        file >> u >> v; // read the u and v
        if (u < V + 1 && v < V + 1) {
            adj[u].push_back(v);
            adj[v].push_back(u); // add them (push back) into the vector
        }
    }
    file.close(); // here we read the inside of the files and to process them we are reading them.

    if (!checkisBipartite(adj, V, color)) {
        cout << "The graph is not bipartite." << endl;
    } else {
        cout << "The graph is bipartite." << endl;
        int matchingSize = maxBipartiteMatching(adj, V);
        cout << "Maximum matching size: " << matchingSize << endl;
    } // here we are processing the files, we find are they bipartite or not if they are bipartite we are finding thr max matching size by the function we implemented.
    
} //function for reading and processing the graph

int main() {
    while (true) {
        string input;
        cout << "Enter the graph name (e.g., 'graph1' or 'exit' to terminate): ";
        cin >> input;
        if (input == "exit") break;
        readGraph(input + ".txt");
    }
    return 0;
}
