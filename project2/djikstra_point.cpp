#include <iostream>
#include <unordered_map>
#include <cmath>
#include <utility>
#include <queue>
#include <set>
#include <unordered_set>
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
	child_list get_childlist(id node_id);
private: 
	unordered_map<id, child_list> graph; 
};

void Graph::print_graph(){
	for (auto x : graph){
		// for (auto y : x.second){
		// 	cout << "boo" << endl;
		// 	cout << y.first << endl;
		// } 
		cout << x.first << endl;	
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

// Gives the children umap
Graph::child_list get_childlist(id node_id){
	if (graph.find(id) == graph.end()){
		return {};
	}
	return graph.at(node_id);
}

int main(){
	Graph graph_; 
	// graph_.add_node(1,2,1);
	// graph_.add_node(1,3,1);
	// graph_.print_graph();
	int height = 1;
	int width = 20;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			int parent_id = width*i+j+1;
			int child_up = width*(i-1)+j+1;
			int child_down = width*(i+1)+j+1;
			cout << "parent_id " << parent_id <<  endl;
			cout << "child_up " << child_up << endl;  
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
	// current_node, parent_node, distance 
	pair<int, int> start (3,4);
	pair<int, int> goal (9,9);
	int start_id = width * start.first + start.second + 1;
	int goal_id = width * goal.first + goal.second + 1;
	unordered_set<id> visited_id(start_id);
	set<id> priorty_queue;
	// while(start_id != goal_id){
	// 	for (auto x : get_childlist(start_id))
	// 		priorty_queue.insert()
	// }
	// while ()
	// graph_.print_graph();

}
