#include <bits/stdc++.h>
#include <omp.h>

#define n 7

using namespace std;

list<int>q;
vector<int>weight(n,1000);
bool visited[n];

struct Comparator {
    // Compare 2 Edges objects using weight
    bool operator ()(const int &e1, const int &e2){
        return weight[e1] < weight[e2];
    }
};

//Display the list/priority queue - Debugging Function
void showlist(list <int> g) { 
    list <int> :: iterator it; 
    for(it = g.begin(); it != g.end(); ++it) 
        cout<<*it<<" "; 
    cout<<endl; 
}

//Display the vector/weights - Debugging Function
void showvec(vector <int> g) { 
    vector <int> :: iterator it; 
    for(it = g.begin(); it != g.end(); ++it) 
        cout<<*it<<" "; 
    cout<<endl; 
}

void displayList(list<int> g) {
	for(auto x : g)
		cout << x << " ";
	cout << endl;
}

void displayVec(vector<int> g) {
	for(auto x : g)
		cout << x << " ";
	cout << endl;
}

void bfs(int adj_matrix[n][n]) {
	if(q.empty())
		return;

    q.sort(Comparator());
		
	//pop first element and display it
	int cur_node = q.front();
    q.pop_front();
	cout << cur_node;
	
	//For every element in the row of the adjacency matrix
	#pragma omp parallel for shared(visited,q,weight)
	for(int i = 0 ; i < n ; i++) {
		//If an unvisited Edge exists
		if(adj_matrix[cur_node][i] > 0 && visited[i] == false) {

			//Replace the weight if it is larger
		    if(weight[i] > adj_matrix[cur_node][i]) {
		        weight[i] = adj_matrix[cur_node][i];
		    }

			//Push the destination of the smallest edge onto the queue
			q.push_back(i);
			visited[i] = true;
		}
	}
	
	bfs(adj_matrix);
}

int main() {
	
	//Get the maximum number of threads
  	int th = omp_get_max_threads();
  	cout << "Number of Threads = " << th << endl;

	//Set the Adjacency Matrix
	int adj_matrix[n][n] = {
							{0,10,15,0,0,0,0},
							{10,0,25,25,0,0,0},
							{15,25,0,0,40,0,0},
							{0,25,0,0,20,0,0},
							{0,0,40,20,0,5,0},
							{0,0,0,0,5,0,20},
							{0,0,0,0,0,20,0}
						   };
	
	//Initialize visited boolean array
	for(int i = 0 ; i < n ; i++) {
		visited[i] = false;
	}

	//Set the start node
	int start_node = 3;

	//Set up the queue, weight and visited
	q.push_back(start_node);
    weight[start_node] = 0;
	visited[start_node] = true;
	
	//Best first Search
	bfs(adj_matrix);

	return 0;
}