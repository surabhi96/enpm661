#include <iostream>
#include <bits/stdc++.h>
#include <array>
#include <string>
#include <queue>
#include <deque>
#include <vector>
using namespace std;
using mat = array <array<int, 3>, 3>;
using arr = array <int, 9>;
using vec = vector<int>;

class tree
{
public:
	int node_id;
	mat input_mat;
	tree * parent_node;
	tree(int node_id, mat input_mat, tree * parent_node)
	{
		this->node_id = node_id;
		this->input_mat = input_mat;
		this->parent_node = parent_node;
	}
};

struct element_ind
{
	int i; int j;
}ind;

arr take_input()
{
	int inp, count = 0;
	// arr input{2,1,3,5,4,6,7,8,0}; // positive test case
	arr input;

	while(count < 9)
	{
		cout << "Enter element " << count+1 << " : ";  
		cin >> inp;
		cout << endl;
		if (inp < 9 && inp >= 0)
		{			
			if (find(&input.front(), &input.back(), inp) == &input.back()){
				input.at(count) = inp;
				count ++;
			}
			else cout << "You have already entered this number, Enter another valid number" << endl;
		}
		else cout << "Enter a valid number between 0 and 8" << endl;
	}
	return input;
}

bool is_valid(arr input_arr)
{
	int inversion_cnt = 0;
	for (int i = 0; i < 8; i++){
		for (int j = i+1; j < 9; j++){
			if (input_arr.at(i) && input_arr.at(j) && input_arr.at(i)>input_arr.at(j))
			{
				inversion_cnt ++;
			}
		}
	}
	return (~inversion_cnt%2);
}

element_ind find_index(mat input_mat){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (input_mat[i][j] == 0){
				ind.i = i; 
				ind.j = j;
				return ind;
			}
		}
	}	
}

mat get_config(mat input_mat, string mv)
{
	element_ind index = find_index(input_mat);
	int i = index.i;
	int j = index.j;
	int temp = input_mat[i][j];

	if (mv == "up" && i != 0) {		
		input_mat[i][j] = input_mat[i-1][j];
		input_mat[i-1][j] = temp;
		return input_mat;
	}
	else if (mv == "down" && i != 2){
		input_mat[i][j] = input_mat[i+1][j];
		input_mat[i+1][j] = temp;
		return input_mat;
	} 
	else if (mv == "left" && j != 0){
		input_mat[i][j] = input_mat[i][j-1];
		input_mat[i][j-1] = temp;
		return input_mat;
	} 
	else if (mv == "right" && j != 2){
		input_mat[i][j] = input_mat[i][j+1];
		input_mat[i][j+1] = temp;
		return input_mat;
	}
	else return {};
}

bool is_present(mat child_mat, deque <mat> q)
{ 
	return (find(q.begin(), q.end(), child_mat) != q.end());
}

int main()
{
	arr input_arr = take_input();

	if(is_valid(input_arr))
	{
		cout << "Solving puzzle ... Wait for a bit ..." << endl;
		mat goal_mat {{	{1, 2, 3},
						{4, 5, 6},
						{7, 8, 0}	}};

		vec goal_vec (goal_mat[0].begin(), goal_mat[2].end());
		// cout << distance(goal_mat, it) << "vnsonvioWHNB/" << endl;

		mat input_mat {{{input_arr.at(0),input_arr.at(1),input_arr.at(2)},
						{input_arr.at(3),input_arr.at(4),input_arr.at(5)},
						{input_arr.at(6),input_arr.at(7),input_arr.at(8)}	}};

		array <string, 4> moves {"up", "down", "left", "right"};
		int node_id = 0;
		// Each node of the tree has information about node id, base mat, parent mat
		tree * root_node = new tree(node_id, input_mat, NULL);
		queue <tree*> open;
		queue <tree*> closed;
		deque <mat> visited {input_mat};
		tree * child_node; 
		open.push(root_node);
		bool flag = 0; // flag becomes true when the puzzle is solved 

		while(!open.empty() && !flag)
		{
			tree * current_node = open.front();
			open.pop();
			closed.push(current_node);
			mat current_mat = current_node->input_mat;

			// Convert input mat to vec
			vec input_vec (current_mat[0].begin(), current_mat[2].end());
			if (input_vec == goal_vec){
				// cout << "Wohoo! Puzzle solved!" << endl;
				flag = 1;
				break;
			} 
			else{
				for (auto mv : moves)
				{
					mat child_mat = get_config(current_mat, mv); // child mat is the explored mat

					if (!child_mat.empty() && !is_present(child_mat, visited))
					{
						node_id ++;
						child_node = new tree(node_id, child_mat, current_node);
						open.push(child_node);
						visited.push_back(child_mat);
						vec child_vec (child_mat[0].begin(), child_mat[2].end());

						if (child_vec == goal_vec){
							// cout << "Wohoo! Puzzle solved!" << endl;
							flag = 1;
							break; // get out of for loop
						}
					}
					
				}
			}
		}
		cout << "Wohoo! Puzzle solved!" << endl;
		cout << "Do you want to checkout the sequence? (Y/N)" << endl;
		char dezire; cin >> dezire;

		if (dezire == 'Y' || dezire == 'y'){
			vector<mat> seq;
			while(child_node->parent_node){
				seq.push_back(child_node->input_mat);
				child_node = child_node->parent_node;
			}
			seq.push_back(child_node->input_mat);
			reverse(seq.begin(), seq.end());
			for (int itr = 0; itr < seq.size(); itr ++){
				for (int k = 0; k < 3; k++){
					for (int p = 0; p < 3; p++){
						cout << seq[itr][k][p] << " ";
					}
					cout << endl;
				}
				cout << endl;
			}
		}

	}

	else cout << "Can't solve puzzle for the given input" << endl;

	return 0;
}
