#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include "maps.h"
using namespace std;




class node { 
public: 
    int idx, dist;
    int  f;
    // h is dist from node to end node target
    // g is the dist from the start node
    // f  = g  + h
    node* parent;


};


struct Compare_F {
    bool operator()(node* p1, node* p2)
    {   
        // priority queue comparator
        return p1->f > p2->f;
    }
};


int manhattan_distance(int col, int row, int nTargetX, int nTargetY)
{
    return max(col - nTargetX, nTargetX - col) + max(row - nTargetY, nTargetY - row);
}


int convert_to_mp_index(int col, int row, int nMapWidth, int nMapHeight)
{           
    return (row*nMapWidth) + col ;
};

int index_to_row(int map_index, int nMapWidth, int nMapHeight)
{   

    return floor(map_index / nMapWidth); 
}

int index_to_col(int map_index, int nMapWidth, int nMapHeight)
{   
    return (map_index % nMapWidth);
}


void PrintPath(int dist, int* pOutBuffer, map<int,node*> closed_set ,int start_idx, int target_idx,const int nOutBufferSize) {


	int index = target_idx;
    // cout<<"target_index"<<target_idx<<endl;
	for (int i=0;i<dist;i++) {
        cout<<"index"<<index<<endl;
		pOutBuffer[dist-1-i] = index;
		node* p  = closed_set[index];
        index = p->idx;
	}
}



int Heuristics(int col, int row, int nTargetX, int nTargetY)
{
    return max(col - nTargetX, nTargetX - col) + max(row - nTargetY, nTargetY - row);
}





void printMatrix(vector<vector<int>> v)
{

	for(int i = 0; i < v.size(); i++)
	{		
		for(int j = 0; j <v[0].size(); j++)
		{
			cout<<"\t"<<v[i][j]<<" ";

		}	
		cout<<endl;
	}
};



int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize)
            {
                
                //making a graph adjacency matrix edges
                vector<vector<int>> edges;
                for (int i = 0;i<nMapHeight; i++)
                {
                    vector<int> v;
                    for (int j = 0;j<nMapWidth;j++)
                    {   
                        // v.push_back(pMap[nMapWidth * nMapHeight -j-(i*nMapWidth)]);

                        v.push_back(pMap[convert_to_mp_index(j, i,nMapWidth,nMapHeight)]);
                    
                    }
                    // reverse(v.begin(),v.end());
                    edges.push_back(v);

                }
                // reverse(edges.begin(), edges.end());


               
                // we already have the grid
                // In the grid the y component can lie between 0 and nMapWidth
                // and the x component can lie between 0 and nMapHeight
                // we make the  start and end as accessible in the grid
                // The last element in the grid will be edges[nMapWidth-1][nMapHeight-1]
                // We apply A star
                edges[nStartY][nStartX] = 1;
                edges[nTargetY][nTargetX] = 1;
                if ((nStartY == nTargetY)  && (nStartX == nTargetX)){
                    return 0;
                }

                // if nStartY<0  && nStartY>

                printMatrix(edges);

                
                // initilize distances with -1
                // our aim is to fill the distances and the closed set. The closed set has the parent and child
                // relationship which will help us trace the path back

                vector<int> distances(nMapWidth*nMapHeight, -1);
                map<int,node*> closed_list ;
                set<int> openlist;



                priority_queue<node*, vector<node*>,Compare_F> q;
                // Start node will have f=0
                // f = g + h cost to go plus cost till the goal
                node* startnode =  new node;
                startnode->idx  = convert_to_mp_index(nStartX,nStartY,nMapWidth,nMapHeight);
                startnode->f = 0;
                startnode->dist = 0;
                startnode->parent = NULL;

                int target_idx = convert_to_mp_index(nTargetX,nTargetY,nMapWidth,nMapHeight);
                int start_idx = convert_to_mp_index(nStartX,nStartY,nMapWidth,nMapHeight);
                q.push(startnode);
                openlist.insert(startnode->idx); // throw all visited nodes in open list

                bool target_reached = false;
                cout<< "target_idx "<< target_idx<<endl;
                cout<<"starting index "<<start_idx<<endl;





                while(!q.empty() and target_reached == false)
                {
                    node* p = q.top();
                    q.pop();


                    if (p->idx == target_idx)
                    {
                        // we have reached
                        // cout<<"p ->idx "<<p->idx<<endl;
                        target_reached = true;
                        distances[p->idx] = p->dist; // get the distance
                        

                        closed_list[p->idx] = p->parent;
                        
                         // need to update the parent earlier
                        cout<<"closed_set "<<p->idx<<endl;                        
                    }
            	    if (distances[target_idx] != -1 && distances[target_idx] <= nOutBufferSize) 
                    {
                        // we print the path 
                        cout<<"distances[target_idx] "<<distances[target_idx]<<endl;
                        cout<<"nOutBufferSize "<<nOutBufferSize<<endl;
                        PrintPath(distances[target_idx], pOutBuffer, closed_list ,start_idx,target_idx,nOutBufferSize);
		                return distances[target_idx];
	                }
	                // if the shortest path is longer than the buffer size,  then only return the size 
	                else if (distances[target_idx] > nOutBufferSize)
                    {
		                // return distances[target_idx];
                        return -1;
	                }

                    

 


                    // the node is neither the target nor visited before
                    int col = index_to_col(p->idx, nMapWidth, nMapHeight);
                    int row = index_to_row(p->idx, nMapWidth, nMapHeight);


                    // cout<<"row "<< index_to_row(11, nMapWidth, nMapHeight);
                    // cout<<"col "<<index_to_col(11, nMapWidth, nMapHeight);



                    if (closed_list.count(p->idx) > 0){
                        continue;
                    }

                    distances[p->idx] = p->dist;
                    closed_list[p->idx] = p->parent;
                    cout<<"closed_set "<<p->idx<<endl;
                    // exploring four directions

	                // visit node on the left
                    if (0 <= col - 1) {
                        int neighbour_index = convert_to_mp_index(col - 1, row,nMapWidth, nMapHeight);
                        if (edges[row][col-1] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* left_neighbour =  new node;
                            left_neighbour->idx = neighbour_index;
                            left_neighbour->parent = p;
                            left_neighbour->dist = p->dist + 1;
                            left_neighbour->f = (p->dist + 1) + Heuristics(col - 1, row, nTargetX, nTargetY);
                            q.push(left_neighbour);
                            openlist.insert(neighbour_index);

                        }
                    }
                    // visit node on the right
                    if (col + 1 < nMapWidth) {
                        int neighbour_index = convert_to_mp_index(col + 1, row,nMapWidth, nMapHeight);
                        if (edges[row][col+1] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* right_neighbour = new node;
                            right_neighbour->idx = neighbour_index;
                            right_neighbour->parent = p;
                            right_neighbour->dist = p->dist + 1;
                            right_neighbour->f = (p->dist + 1) + Heuristics(col + 1, row, nTargetX, nTargetY);
                            q.push(right_neighbour);
                            openlist.insert(neighbour_index);

                        }
                    }
                    // visit node below
                    if (0 <= row - 1) {
                        int neighbour_index = convert_to_mp_index(col, row-1,nMapWidth, nMapHeight);
                        // cout<<neighbour_index<<endl;
                        if (edges[row-1][col] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* down_neighbour = new node;
                            down_neighbour->idx = neighbour_index;
                            down_neighbour->parent = p;
                            down_neighbour->dist = p->dist + 1;
                            down_neighbour->f = (p->dist + 1) + Heuristics(col , row-1, nTargetX, nTargetY);
                            q.push(down_neighbour);
                            openlist.insert(neighbour_index);

                        }
                    }
                    // visit node above
                    if (row + 1 < nMapHeight) {
                        int neighbour_index = convert_to_mp_index(col, row+1,nMapWidth, nMapHeight);
                        // cout<<neighbour_index<<endl;
                        if (edges[row+1][col] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* up_neighbour = new node;
                            up_neighbour->idx = neighbour_index;
                            up_neighbour->parent = p;
                            up_neighbour->dist = p->dist + 1;
                            up_neighbour->f = (p->dist + 1) + Heuristics(col , row+1, nTargetX, nTargetY);
                            q.push(up_neighbour);
                            openlist.insert(neighbour_index);

                        }

                    }




                }
		        return -1;
	            
                




            };


void LogResult(const int inStartIndex, const int inStepAmount, const int* inPathBuffer)
{
	if (inStepAmount == -1)
		std::cout << "Unreachable solution!" << std::endl;
	else
	{
		std::cout << "Found solution with " << inStepAmount << " elements" << std::endl;

		// std::cout << inStartIndex << " -> ";

		for (int i = 0; i < inStepAmount; ++i)
		{
			std::cout << inPathBuffer[i];

			if (i != inStepAmount - 1)
				std::cout << " -> ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
};

int main()
{
    
    // unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
    // int pOutBuffer[12];
    // int k  = FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);
    // cout<<k;

    // // unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
    // // int pOutBuffer[7];
    // // int k = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
    // // cout<<k;
    // for (int i =0;i<k;i++)
    // {
    //     cout<<pOutBuffer[i]<<endl;
    // }

    // // case 0
	int case_zero_result = FindPath(0, 0, 3, 2, gCaseZeroMap, 4, 3, gCaseZeroBuffer, gCaseZeroBufferSize);
	LogResult(4 * 0 + 0, case_zero_result, gCaseZeroBuffer);

	// // // // Case #1
	int case_one_result = FindPath(2, 0, 0, 2, gCaseOneMap, 3, 3, gCaseOneBuffer, gCaseOneBufferSize);
	LogResult(3 * 0 + 2, case_one_result, gCaseOneBuffer);

	// // // Case #2 - test with a larger map
	int result = FindPath(0, 0, 4, 5, gLargerMap, 5, 6, gLargerBuffer, gLargerBufferSize);
	LogResult(0, result, gLargerBuffer);

	// // // Case #3 - map with only one available path, but we can't move diagonally so it is invalid
	int one_available_result = FindPath(0, 0, 1, 1, gOneAvailableMap, 2, 2, gOneAvialableBuffer, gOneAvailableBufferSize);
	LogResult(0, one_available_result, gOneAvialableBuffer);

	// // Case #4 - output buffer is too small to hold the output buffer
	int small_buffer_result = FindPath(0, 0, 3, 3, gSmallBufferMap, 4, 4, gSmallBufferBuffer, gSmallBufferBufferSize);
	LogResult(0, small_buffer_result, gSmallBufferBuffer);

	// // Case #5 - huge map
	int huge_map_result = FindPath(0, 0, 0, 19, gHugeMap, 20, 20, gHugeBuffer, gHugeBufferSize);
	LogResult(0, huge_map_result, gHugeBuffer);

    // // case #6 
    int map_neil_6 =  FindPath(0, 0, 0, 1, g_neil_6_Map,2, 2,gHugebuffer_neil_6 , gHugebuffer_neil_6_size);
    LogResult(0,map_neil_6 , gHugebuffer_neil_6);
    return 0;
    
}



