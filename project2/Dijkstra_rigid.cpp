// #pragma once
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <utility>
#include <queue>
#include <array>
#include <unordered_set>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
#include <vector>
using id = unsigned int; 
using weight = double;
using namespace std;
using namespace cv;
int width = 300;
int height = 200;
int division = 3;

// function declaration 
void print_spath(unordered_map<id, id>&, id);
Mat color_pixel(Mat, const string, int, int);
void MyLine(Mat, Point, Point);
Point id2pixel(id);
Point in_img_frame(Point);
bool is_obstacle(int, int);
bool is_valid(Point);
bool is_obstacle_there(int, int,int,int);
vector<pair<int, int>> my_path;

// bool CircleObstacle(int, int);
// bool EllipseObstacle(int, int);

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
Mat frame = cv::Mat(height*division, width*division, CV_8UC3, Scalar(255,255,255));
int main(){
	std::chrono::high_resolution_clock::time_point s, e;

			// add pixel coordinates of start(0, 0) and goal (3,10)
	// Point start_w (190, 199);
	// Point goal_w (198, 199);
	Point start_w (150, 130);
	Point goal_w (150, 70);
	int r; //radius
	int c;//clearence
	cout << "Enter x coordinate of start point in range 0 to 300 "; cin >> start_w.x;
	cout << "Enter y coordinate of start point in range 0 to 200 "; cin >> start_w.y;
	cout << "Enter x coordinate of goal point in range 0 to 300 "; cin >> goal_w.x;
	cout << "Enter y coordinate of goal point in range 0 to 200 "; cin >> goal_w.y;
	cout << "Enter the radius of the robot "; cin >> r;
	cout << "Enter the clearence of the robot "; cin >> c;
	s = std::chrono::high_resolution_clock::now();  


	// // Input image
	// Mat frame = cv::Mat(height*division, width*division, CV_8UC3, Scalar(255,255,255));

	// for (int i = 0; i < height; i++){
	// 	for (int j = 0; j < width; j++){
	// 		if (is_obstacle_there(j, i,r,c))
	// 			frame = color_pixel(frame, "clearence", j, i);
	// 	}
	// }
	
	// for (int i = 0; i < height; i++){
	// 	for (int j = 0; j < width; j++){
	// 		if (is_obstacle(j, i))
	// 			frame = color_pixel(frame, "obstacle", j, i);
	// 	}
	// }

	// imshow("Dijkstra's path planning algorithm", frame);
	// waitKey(0);

	for (int i = 1; i < width; i++){
		MyLine(frame, Point(division*i,0), Point(division*i, division*height));
	}
	for (int i = 1; i < height; i++){
		MyLine(frame, Point(0, i*division), Point(division*width, i*division));
	}

	Graph graph_; 

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			int parent_id = width*i+j+1;
			int child_up = width*(i-1)+j+1;
			int child_down = width*(i+1)+j+1;

			Point p1 = id2pixel(parent_id);
			Point p2 = id2pixel(child_up);
			Point p3 = id2pixel(child_down);
			Point p4 = id2pixel(parent_id+1);
			Point p5 = id2pixel(parent_id-1);
			Point p6 = id2pixel(child_up-1);
			Point p7 = id2pixel(child_down-1);
			Point p8 = id2pixel(child_up+1);
			Point p9 = id2pixel(child_down+1);



			// cout << "parent_id " << parent_id <<  endl;
			// cout << "child_up " << child_up << endl;  
			// left child allowed with edge weight = 1
			if (parent_id)
			if (!is_obstacle_there(p1.x,p1.y,r,c) &&
				!is_obstacle_there(p2.x,p2.y,r,c) &&
				!is_obstacle_there(p3.x,p3.y,r,c) &&
				!is_obstacle_there(p4.x,p4.y,r,c) &&
				!is_obstacle_there(p5.x,p5.y,r,c) &&
				!is_obstacle_there(p6.x,p6.y,r,c) &&
				!is_obstacle_there(p7.x,p7.y,r,c) &&
				!is_obstacle_there(p8.x,p8.y,r,c) &&
				!is_obstacle_there(p9.x,p9.y,r,c) 
				){
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
	}


	Point start = in_img_frame(start_w);
	Point goal = in_img_frame(goal_w);
	cout << "start " << start.x << " " << start.y << endl;
	cout << "goal " << goal.x << " " << goal.y << endl;

	if (is_obstacle_there(start.x, start.y,r,c) || is_obstacle_there(goal.x, goal.y,r,c)){
		cerr << "One or more of your chosen points lie in the obstacle" << endl;
		exit(0);
	}
	if (!is_valid(start) || !is_valid(goal)){
		cerr << "One or more of your chosen points lie outside the image" << endl;
		exit(0);
	}

	// frame = color_pixel(frame, "start", start.x, start.y);
	// frame = color_pixel(frame, "goal", goal.x, goal.y);
	// imshow("Dijkstra's path planning algorithm", frame);
	// 		waitKey(0);
	// convert pixel to id encoding 
	int start_id = width * start.y + start.x + 1;
	int goal_id = width * goal.y + goal.x + 1;
	// cout << "start " << start_id << "end " << goal_id << endl;
	/*This shows that my graph is perfectly formed.
	try for height = 2 and width = 20*/
	// for (auto x : graph_.get_child_list(1))
	// 	cout << x.second.child_id << " " << x.second.edge_weight << endl;
 
	// I am just going to store nodes(to see if they have been visited)
	// and their parent id (address)
	unordered_map<id, id> visited;
	vector<pair<int, int>> exp;
	visited[start_id] = 0; // parent is id 0; i.e non-existant 

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
			Point child_pix = id2pixel(c_id);
			
			exp.push_back(make_pair(child_pix.x, child_pix.y)); 
			// frame = color_pixel(frame, "visited", child_pix.x, child_pix.y);
			// frame = color_pixel(frame, "goal", goal.x, goal.y);
			// frame = color_pixel(frame, "start", start.x, start.y);

			// imshow("Dijkstra's path planning algorithm", frame);
			// waitKey(1);

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
	// imshow("Dijkstra's path planning algorithm", frame);
	// waitKey(0);
	e = std::chrono::high_resolution_clock::now();
	double time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
    std::cout << "Code took " << time_elapsed << " miliseconds to execute\n";

    Mat frame = cv::Mat(height*division, width*division, CV_8UC3, Scalar(255,255,255));

    for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (is_obstacle_there(j, i,r,c))
				frame = color_pixel(frame, "clearence", j, i);
		}
	}
	
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (is_obstacle(j, i))
				frame = color_pixel(frame, "obstacle", j, i);
		}
	}

	// imshow("Dijkstra's path planning algorithm", frame);
	// waitKey(0);

	for (int i = 1; i < width; i++){
		MyLine(frame, Point(division*i,0), Point(division*i, division*height));
	}
	for (int i = 1; i < height; i++){
		MyLine(frame, Point(0, i*division), Point(division*width, i*division));
	}

	frame = color_pixel(frame, "start", start.x, start.y);
	frame = color_pixel(frame, "goal", goal.x, goal.y);
	// imshow("Dijkstra's path planning algorithm", frame);
	// 	waitKey(0);

    for (auto it = exp.begin(); it != exp.end(); it++){
    	frame = color_pixel(frame, "visited", it->first, it->second);

  //   	frame = color_pixel(frame, "start", start.x, start.y);
		// frame = color_pixel(frame, "goal", goal.x, goal.y);
		imshow("Dijkstra's path planning algorithm", frame);
		waitKey(1);
    }
    for (auto it = my_path.begin(); it != my_path.end(); it++){
    	frame = color_pixel(frame, "path", it->first, it->second);
    	imshow("Dijkstra's path planning algorithm", frame);
		waitKey(1);
    }
	waitKey(0);
}
// prints the nodes traversed in shortest path 
// passing by reference avoids the copy of the map which could be heavy work.
void print_spath(unordered_map<id, id>& visited, id goal){
	id new_goal = visited.find(goal)->second;
	
	if (new_goal == 0) 
		return; 
	print_spath(visited, new_goal);
	cout << new_goal << " ";
	Point child_pix = id2pixel(new_goal);
	my_path.push_back(make_pair(child_pix.x, child_pix.y));
	// frame = color_pixel(frame, "path", child_pix.x, child_pix.y);
	// imshow("Dijkstra's path planning algorithm", frame);
	// waitKey(1);
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

	// Point p = id2pixel(child_id);
	// // cout << "p.x " <<p.x << "p.y "<<p.y<<endl;
	// frame = color_pixel(frame, "obstacle", p.x, p.y);
	// imshow("Dijkstra's path planning algorithm", frame);
	// waitKey(1);
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

void MyLine(Mat img, Point start, Point end)
{
	int thickness = 0.1;
	int lineType = LINE_8;
	line( img,
		start,
		end,
		Scalar(200,200,200),
		thickness,
		lineType );
}

Mat color_pixel(Mat image, const string node, int x, int y){
	Vec3b color;
	int split;
	// cout << node << endl;
	if(node == "start") {
		split = division*1;
		color = Vec3b(0,0,255);
	}
	if(node == "goal") {
		split = division*1; 
		color = Vec3b(34,139,34);
	}
	if(node == "visited") {
		split = division;
		color = Vec3b(222, 232, 8);
	}
	if(node == "path") {
		split = division;
		color = Vec3b(0,0,0);
	}
	if(node == "obstacle") {
		split = division;
		color = Vec3b(200,200,200);
	}
	if(node == "clearence") {
		split = division;
		color = Vec3b(150,150,150);
	}
	for (int i = 0; i < split; i++){
		for (int j = 0; j < split; j++){
			// cout << "i " << division*x+i << "j " << division*y+j << endl;
			// if(node == "visited"){
			// 	cout << "i " << division*x+i << "j " << division*y+j << endl;
			// }
			// image plot takes y coordinate first!
			image.at<Vec3b>(division*y+j, division*x+i) = color;
		}
	}
	return image; 
}

Point id2pixel(id id_){
	// cout << "id " << id_ << endl;
	id_ = id_ - 1;
	Point p;
	 
	p.y = floor(id_/width);
	p.x = id_ - width*p.y;
	// cout << "xy " << p.x << " " << p.y << " " << endl;
	return p; 
}

Point in_img_frame(Point in){
	Point out; 
	out.x = in.x-1;
	out.y = height-in.y-1;
	if (out.x<0) out.x=0; 
	if (out.y<0) out.y=0;
	return out;
}

bool is_valid(Point in){
	if (in.x < 0 || in.x > 300) 
		return false;
	if (in.y < 0 || in.y > 200)
		return false;
	return true; 
}

bool is_obstacle(int x, int y) 
{
	// Rhombus
	if ((x*(-3/5.0)+y-55 < 0) && (x*(3/5.0)+y-325 < 0) && 
		(x*(-3/5.0)+y-25 > 0) && (x*(3/5.0)+y-295 > 0))
	{
		return true;
	}
	// polygon - rhombus
	else if (x*(7/5.0)+y-120 > 0 && x*(-6/5.0)+y+10 < 0 && 
		x*(6/5.0)+y-170 < 0 && x*(-7/5.0)+y+90 > 0)
	{
		return true;
	}
	// polygon - triangle1
	else if (y-15 > 0 && x*(7/5.0)+y-120 < 0 && x*(-7/5.0)+y+20 < 0)
	{
		return true;
	}
	// polygon - triangle2
	else if (y+(13*x)-340 > 0 && x+y-100 < 0 && x*(-7/5.0)+y+20 > 0)
	{
		return true;
	}
	// rectangle -angled
	else if ((200-y) - (1.73)*x + 135 > 0 && (200-y) + (0.58)*x - 96.35  <= 0 && 
		(200-y) - (1.73)*x - 15.54 <= 0 && (200-y) + (0.58)*x - 84.81 >= 0)
	{
		return true;
	}
	// circle 
	else if ((pow((x-225),2) + pow((y-50),2) < (625)))
	{
		return true;
	}
	// ellipse
	else if ((((pow((x-150),2)/(1600)) + ( pow((y-100),2)/(400)))) <= 1)
	{
		return true;
	}
	
	return false;
}


bool is_obstacle_there(int x, int y,int r,int c) 
{
  // Rhombus
  if ((x*(-3/5.0)+y-55+((-r-c)/sin(1.57-atan(-3/5.0))) < 0) && (x*(3/5.0)+y-325 +((-r-c)/sin(1.57-atan(3/5.0)))< 0) && 
    (x*(-3/5.0)+y-25+((r+c)/sin(1.57-atan(-3/5.0))) > 0) && (x*(3/5.0)+y-295 +((r+c)/sin(1.57-atan(3/5.0))))> 0)
  {
    // cout<<"rhom"<<endl;
    return true;}


  // rectangle -angled
  else if ((200-y) - (1.73)*x + 135+ ((r+c)/sin(1.57-atan(1.73)))> 0 && (200-y) + (0.58)*x - 96.35 +((-r-c)/sin(1.57-atan(0.58))) <= 0 && 
    (200-y) - (1.73)*x - 15.54+((-r-c)/sin(1.57-atan(1.73))) <= 0 && (200-y) + (0.58)*x - 84.81+((r+c)/sin(1.57-atan(0.58))) >= 0)
  {
  // cout<<"rectangle"<<endl;
    return true;
}
  // circle 
  else if ((pow((x-225),2) + pow((y-50),2) < pow((25+r+c),2)))

    {
      // cout<<"circle"<<endl;
    return true;
  }

  // ellipse
  else if ((((pow((x-150),2)/pow((40+r+c),2)) + ( pow((y-100),2)/  pow((20+r+c),2)  ))) <= 1)
  {
    // cout<<"ellipse"<<endl;
    return true;
  }
  else if(x*(7/5.0)+y-120> 0 && x*(-6/5.0)+y+10 +((-r-c)/sin(1.57-atan(-6/5.0)))< 0 && 
    x*(6/5.0)+y-170 +((-r-c)/sin(1.57-atan(6/5.0)))< 0 &&               x*(-7/5.0)+y+90 +((r+c)/sin(1.57-atan(-7/5.0)))> 0)
  {
  // cout<<"rhom poly"<<endl;
    return true;}

   else if (y-15+r+c> 0 && x*(7/5.0)+y-120< 0 && x*(-7/5.0)+y+20 < 0)
  {
    // cout<<"poly t1"<<endl;
    return true;
  }
   else if (y+(13*x)-340 +((r+c)/sin(1.57-atan(13)))> 0 && x+y-100+((-r-c)/sin(1.57-atan(1)))< 0 && x*(-7/5.0)+y+20> 0)
  {
    // cout<<"poly t2"<<endl;
    return true;
  }

  
  return false;
}
