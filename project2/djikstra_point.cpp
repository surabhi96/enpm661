// #pragma once
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <utility>
#include <queue>
#include <array>
#include <unordered_set>
#include <bits/stdc++.h>
using id = unsigned int; 
using weight = double;
using namespace std;
// function declaration 
void print_spath(unordered_map<id, id>&, id);

// encodes child data: child id and associated edge weight 
// this is used only during initialization / graph formation 
struct child_data{
	weight edge_weight; 
	id child_id;
	child_data(id child_id, weight edge_weight){
		this->child_id = child_id; 
		this->edge_weight = edge_weight;
	}
	child_data() : child_id(0), edge_weight(0){}
};

// encodes the node id and the min distance value required to reach it
struct node_info{
	id node_id; 
	double distance;
	node_info(id node_id, double distance){
		this->node_id = node_id;
		this->distance = distance;
	}
};

class Graph{
public:
	Graph(){}
	using child_list = unordered_map<id, child_data>;
	void add_node(id parent_id, id child_id, weight edge_weight);
	void print();
	child_list get_child_list(id parent_id);
	child_data get_child_data(id parent_id, id child_id);
private: 
	unordered_map<id, child_list> graph; 
};

// Priority queue comparator 
class comparator{
public:
	int operator() (node_info n1, node_info n2){
		// Return true if the newly pushed node distance is greater than the last 
		// node distance; equivalent to min heap 
		return n1.distance > n2.distance; 
	}
};

int main(){
	Graph graph_; 

	int height = 21;
	int width = 10;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			int parent_id = width*i+j+1;
			int child_up = width*(i-1)+j+1;
			int child_down = width*(i+1)+j+1;
			// cout << "parent_id " << parent_id <<  endl;
			// cout << "child_up " << child_up << endl;  
			// left child allowed with edge weight = 1
			if (j > 0) graph_.add_node(parent_id, parent_id-1, 1);
			// top child allowed with edge weight = 1
			if (i > 0) graph_.add_node(parent_id, child_up, 1);
			// down child allowed with edge weight = 1
			if (i < height-1) graph_.add_node(parent_id, child_down, 1);
			// right child allowed with edge weight = 1
			if (j < width-1) graph_.add_node(parent_id, parent_id+1, 1);
			// If up and left then top left with weight = sqrt(2)
			if (i > 0 && j > 0) graph_.add_node(parent_id, child_up-1, sqrt(2));
			// if up and right then top right
			if (i > 0 && j < width-1) graph_.add_node(parent_id, child_up+1, sqrt(2));
			// if bottom and left then bottom left 
			if (i < height-1 && j > 0) graph_.add_node(parent_id, child_down-1, sqrt(2));
			// if bottom and right then bottom right 
			if (i < height-1 && j < width-1) graph_.add_node(parent_id, child_down+1, sqrt(2));
		}
	}
	// graph_.print();

	// add pixel coordinates of start and goal
	pair<int, int> start (0,0);
	pair<int, int> goal (4,1);
	// convert pixel to id encoding 
	int start_id = width * start.first + start.second + 1;
	int goal_id = width * goal.first + goal.second + 1;
	/*This shows that my graph is perfectly formed.
	try for height = 2 and width = 20*/
	// for (auto x : graph_.get_child_list(1))
	// 	cout << x.second.child_id << " " << x.second.edge_weight << endl;
 
 	// I am just going to store nodes(to see if they have been visited)
 	// and their parent id (address)
 	unordered_map<id, id> visited;
 	visited[start_id] = 0; // parent is id 0; i.e non-existant 
 	// make a visited structure os parent and child id's and then push back in a set? 

 	// Initialize all node distances with INT_MAX (Inf)
 	// Note that your weight is of type double; so static_cast<double>
 	// Note the id count is starting from 1; therefore distance[height*width] should be valid
 	vector<double> distance(height*width+1, static_cast<double>(INT_MAX));
 	distance[start_id] = 0; // starting node has distance = 0

 	// To store nodes and its distance 
 	priority_queue<node_info, vector<node_info>, comparator> pq; 
 	pq.push(node_info(start_id, 0));

 	// To store explored nodes
 	unordered_set<id> explored;

 	// To keep track of parent id 
 	id prev_id = 0; 
 	// run until the top of pq is not goal id 
 	while (prev_id != goal_id)
	{
		// cout << "visited " << endl;
		// for (auto x : visited) 
		// 	 cout << x.first << " " << x.second << endl;

		id current_id = pq.top().node_id; 
		pq.pop();
		// This stores my explored (already visited) nodes!
		explored.insert(current_id);
		double parent_weight = distance[current_id];
	 	auto current_list = graph_.get_child_list(current_id); // returns a umap

	 	// push current_id's children in the priority queue if the "if" stmt is true 
	 	for (auto it : current_list)
	 	{
	 		child_data cd = graph_.get_child_data(current_id, it.first);
	 		id c_id = cd.child_id;
	 		
	 		if (explored.find(c_id) == explored.end() // if this node has not been explored previously 
	 			&& distance[c_id] > parent_weight + cd.edge_weight) // and its distance is greater than..
	 		{
	 			// update distance and push in the priority queue
	 			distance[c_id] = parent_weight + cd.edge_weight;
	 			pq.push(node_info(c_id, distance[c_id]));
	 			// update parent of the child
	 			visited[c_id] = current_id;
	 		}
	 	}

	 	prev_id = current_id;
	} 
	// for (auto x : visited) cout << "visited " << x.first <<" "; cout << endl;
	cout << "shortest path node list " ;
	print_spath(visited, goal_id); 
	cout << goal_id << endl;
}

// prints the nodes traversed in shortest path 
// passing by reference avoids the copy of the map which could be heavy work.
void print_spath(unordered_map<id, id>& visited, id goal){
	id new_goal = visited.find(goal)->second;
	if (new_goal == 0) 
		return; 
	print_spath(visited, new_goal);
	cout << new_goal << " ";
}

void Graph::print(){
	for (auto x : graph){
		// for (auto y : x.second){
		// 	cout << "boo" << endl;
		// 	cout << y.first << endl;
		// } 
		cout << x.first << " ";	
	}
	cout << endl;
}

void Graph::add_node(id parent_id, id child_id, weight edge_weight){
	if (graph.find(parent_id) == graph.end())
		graph[parent_id] = child_list();
	else{
		if (graph[parent_id].find(child_id) != graph[parent_id].end()){
			cerr << "Edge already exists from " << parent_id << " to " << child_id << endl;
			return; 
		}
	}
	graph[parent_id][child_id] = child_data(child_id, edge_weight);
}

// Gives the children umap
Graph::child_list Graph::get_child_list(id parent_id){
	if (graph.find(parent_id) == graph.end())
		return {};
	return graph.at(parent_id);
}

child_data Graph::get_child_data(id parent_id, id child_id){
	auto parent_id_ptr = graph.find(parent_id);
	auto child_id_ptr = parent_id_ptr->second.find(child_id);
	if (parent_id_ptr == graph.end()) 
		return child_data();
	return (child_id_ptr != parent_id_ptr->second.end()) ?
		child_id_ptr->second :
		child_data();
}
