#include<iostream>
#include<bits/stdc++.h>
#include<vector>



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
	for (int i=0;i<dist;i++) {
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
                
                //making a graph occupancy grid map occupancy_grid
                vector<vector<int>> occupancy_grid;
                for (int i = 0;i<nMapHeight; i++)
                {
                    vector<int> v;
                    for (int j = 0;j<nMapWidth;j++)
                    {   
                        // v.push_back(pMap[nMapWidth * nMapHeight -j-(i*nMapWidth)]);

                        v.push_back(pMap[convert_to_mp_index(j, i,nMapWidth,nMapHeight)]);
                    
                    }
                    occupancy_grid.push_back(v);

                }


               
                // we already have the grid
                // In the grid the y component can lie between 0 and nMapWidth
                // and the x component can lie between 0 and nMapHeight
                // we make the  start and end as accessible in the grid
                // The last element in the grid will be occupancy_grid[nMapWidth-1][nMapHeight-1]
                // We apply A star
                occupancy_grid[nStartY][nStartX] = 1;
                occupancy_grid[nTargetY][nTargetX] = 1;
                if ((nStartY == nTargetY)  && (nStartX == nTargetX)){
                    return 0;
                }


                // printMatrix(occupancy_grid);

                
                // initilize distances with -1
                // our aim is to fill the distances and the closed list. The closed list has the parent and child
                // relationship which will help us trace the path back

                vector<int> distances(nMapWidth*nMapHeight, -1);
                map<int,node*> closed_list ;




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


                bool target_reached = false;


                while(!q.empty() and target_reached == false)
                {
                    node* p = q.top();
                    q.pop();


                    if (p->idx == target_idx)
                    {
                        // we have reached
                        target_reached = true;
                        distances[p->idx] = p->dist; // get the distance
                        

                        closed_list[p->idx] = p->parent;
                        
                         // need to update the parent earlier                     
                    }
            	    if (distances[target_idx] != -1 && distances[target_idx] <= nOutBufferSize) 
                    {
                        // we print the path 
                        PrintPath(distances[target_idx], pOutBuffer, closed_list ,start_idx,target_idx,nOutBufferSize);
		                return distances[target_idx];
	                }
	                // if the shortest path is longer than the buffer size,  then only return the size 
	                else if (distances[target_idx] > nOutBufferSize)
                    {
		                return distances[target_idx];
	                }

                    

 


                    // the node is neither the target nor visited before
                    int col = index_to_col(p->idx, nMapWidth, nMapHeight);
                    int row = index_to_row(p->idx, nMapWidth, nMapHeight);

                    if (closed_list.count(p->idx) > 0){
                        continue;
                    }

                    distances[p->idx] = p->dist;
                    closed_list[p->idx] = p->parent;
                    // exploring four directions

	                // visit node on the left
                    if (0 <= col - 1) {
                        int neighbour_index = convert_to_mp_index(col - 1, row,nMapWidth, nMapHeight);
                        if (occupancy_grid[row][col-1] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* left_neighbour =  new node;
                            left_neighbour->idx = neighbour_index;
                            left_neighbour->parent = p;
                            left_neighbour->dist = p->dist + 1;
                            left_neighbour->f = (p->dist + 1) + Heuristics(col - 1, row, nTargetX, nTargetY);
                            q.push(left_neighbour);

                        }
                    }
                    // visit node on the right
                    if (col + 1 < nMapWidth) {
                        int neighbour_index = convert_to_mp_index(col + 1, row,nMapWidth, nMapHeight);
                        if (occupancy_grid[row][col+1] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* right_neighbour = new node;
                            right_neighbour->idx = neighbour_index;
                            right_neighbour->parent = p;
                            right_neighbour->dist = p->dist + 1;
                            right_neighbour->f = (p->dist + 1) + Heuristics(col + 1, row, nTargetX, nTargetY);
                            q.push(right_neighbour);

                        }
                    }
                    // visit node below
                    if (0 <= row - 1) {
                        int neighbour_index = convert_to_mp_index(col, row-1,nMapWidth, nMapHeight);
                        if (occupancy_grid[row-1][col] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* down_neighbour = new node;
                            down_neighbour->idx = neighbour_index;
                            down_neighbour->parent = p;
                            down_neighbour->dist = p->dist + 1;
                            down_neighbour->f = (p->dist + 1) + Heuristics(col , row-1, nTargetX, nTargetY);
                            q.push(down_neighbour);
                        }
                    }
                    // visit node above
                    if (row + 1 < nMapHeight) {
                        int neighbour_index = convert_to_mp_index(col, row+1,nMapWidth, nMapHeight);
                        if (occupancy_grid[row+1][col] == 1 && (closed_list.count(neighbour_index)==0)) {
                            node* up_neighbour = new node;
                            up_neighbour->idx = neighbour_index;
                            up_neighbour->parent = p;
                            up_neighbour->dist = p->dist + 1;
                            up_neighbour->f = (p->dist + 1) + Heuristics(col , row+1, nTargetX, nTargetY);
                            q.push(up_neighbour);
                        }

                    }




                }
		        return -1;  
            };




