// #pragma once
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <utility>
#include <queue>
#include <unordered_set>
#include <bits/stdc++.h>
using id = unsigned int; 
using weight = double;
using namespace std;

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
	child_data* get_child_data(id parent_id, id child_id);
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
	// graph_.add_node(1,2,1);
	// graph_.add_node(1,3,1);
	// graph_.print();
	int height = 2;
	int width = 20;
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
	// add pixel coordinates of start and goal
	pair<int, int> start (0,0);
	pair<int, int> goal (1,8);
	// convert pixel to id encoding 
	int start_id = width * start.first + start.second + 1;
	int goal_id = width * goal.first + goal.second + 1;
	/*This shows that my graph is perfectly formed.
	try for height = 2 and width = 20*/
	// for (auto x : graph_.get_child_list(22))
	// 	cout << x.second.child_id << endl;
 
 	// I am just going to store nodes(to see if they have been visited)
 	// and their parent id (address)?
 	unordered_map<id, id*> visited;
 	visited[start_id] = nullptr;

 	// Initialize all node distances with INT_MAX (Inf)
 	// Note that your weight is of type double; so static_cast<double>
 	std::vector<double> distance(height*width, static_cast<double>(INT_MAX));
 	distance[start_id] = 0; // starting node has distance = 0

 	// To store node and its distance 
 	priority_queue<node_info, vector<node_info>, comparator> pq; 
 	pq.push(node_info(start_id, 0));

 	while (!pq.empty())
	{
		id current_id = pq.front().first; 
		pq.pop();
		// visited[current_id] = 

	 	auto current = graph_.get_child_list(current_id); // returns a umap
	 	for (auto it : current){
	 		child_data * cd = graph_.get_child_data(current_id, it.first);
	 		auto c_id = cd->child_id;
	 		auto parent_weight = distance[start_id];
	 		if (visited.find(c_id) == visited.end() && distance[c_id] > parent_weight + cd->edge_weight){
	 			distance[c_id] = parent_weight + cd->edge_weight;
	 			pq.push(node_info(c_id, distance[c_id]));
	 		}
	 	}

	}

}

void Graph::print(){
	for (auto x : graph){
		// for (auto y : x.second){
		// 	cout << "boo" << endl;
		// 	cout << y.first << endl;
		// } 
		cout << x.first << endl;	
	}
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

child_data* Graph::get_child_data(id parent_id, id child_id){
	auto parent_id_ptr = graph.find(parent_id);
	auto child_id_ptr = parent_id_ptr->second.find(child_id);
	if (parent_id_ptr == graph.end()) 
		return nullptr;
	return (child_id_ptr != parent_id_ptr->second.end()) ?
		&child_id_ptr->second :
		nullptr;
}
