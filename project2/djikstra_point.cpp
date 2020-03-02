#include <iostream>
#include <unordered_map>
#include <math>
using id = unsigned int; 
using weight = double;
using namespace std;

struct node_connection{
	weight edge_weight; 
	id node_id;
	node_connection(id node_id, weight edge_weight){
		this->node_id = node_id; 
		this->edge_weight = edge_weight;
	}
	node_connection():node_id(0), edge_weight(0){}
};

class Graph{
public:
	Graph(){}
	using child_list = unordered_map<id, node_connection>;
	void add_node(id parent, id child, weight edge_weight);
	void print_graph();
private: 
	unordered_map<id, child_list> graph; 
};

void Graph::print_graph(){
	for (auto x : graph){
		for (auto y : x.second){
			cout << "boo" << endl;
			cout << y.first << endl;
		} 	
	}
}

void Graph::add_node(id parent, id child, weight edge_weight){
	if (graph.find(parent) == graph.end())
		graph[parent] = child_list();
	else{
		if (graph[parent].find(child) != graph[parent].end()){
			cerr << "Edge already exists from " << parent << " to " << child << endl;
			return; 
		}
	}
	graph[parent][child] = node_connection(child, edge_weight);
}

int main(){
	Graph graph_; 
	// graph_.add_node(1,2,1);
	// graph_.add_node(1,3,1);
	// graph_.print_graph();
	int height = 200;
	int width = 300;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			// left child allowed with edge weight = 1
			if (j > 0) graph_.add_node((width*i+j+1), (width*i+j), 1);
			// top child allowed with edge weight = 1
			if (i > 0) graph_.add_node((width*i+j+1), (width*(i-1)+j+1), 1);
			// down child allowed with edge weight = 1
			if (i < height-1) graph_.add_node((width*i+j+1), (width*(i+1)+j+1), 1);
			// right child allowed with edge weight = 1
			if (j < width-1) graph_.add_node((width*i+j+1), (width*(i)+j+2), 1);
			// If up and left then top left with weight = sqrt(2)
			if (i > 0 && j > 0) graph_.add_node((width*i+j+1), (width*(i-1)+j), 1);
			// if up and right then top right
			if (i > 0 && j < width-1) graph_.add_node((width*i+j+1), (width*(i-1)+j+2), 1);
			// if bottom and left then bottom left 
			if (i < height-1 && j > 0) graph_.add_node((width*i+j+1), (width*(i+1)+j), 1);
			// if bottom and right then bottom right 
			if (i < height-1 && j < width-1) graph_.add_node((width*i+j+1), (width*(i+1)+j+2), 1);
		}
	}
}
