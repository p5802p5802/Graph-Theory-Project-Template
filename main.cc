/* 
    Your main program goes here
*/
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <map>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"
#include "fstream"

using namespace std;

//find shortest path between two vertices
vector<vector<int> > shortest_path(int start, int end, int v_num, vector<vector<int> >& e_arr){
    //variable declaration
    vector <vector<int> > e_arr_bonus;
    vector <int> path_buffer;
    vector <int> previous_node;
    int node_now;
    int node_pre_index;
    int node_pre;
    int fin_flag = 0;
    int counter = 0;
    int add_flag = 0;

    //initialize e_arr_bonus;
    for(int i=0;i< v_num;i++){
     vector <int> e_arr_bonus_temp; 
     for(int j=0;j< v_num;j++)
       e_arr_bonus_temp.push_back(0);
     e_arr_bonus.push_back(e_arr_bonus_temp);
    }
    
    //find shortest path
    node_now = start;
    path_buffer.push_back(start);
    previous_node.push_back(-1);

    while(!fin_flag){
      for(int i=0;i< v_num;i++)
        if(e_arr[node_now][i]){
          path_buffer.push_back(i);
          previous_node.push_back(counter);
        }
      counter++;
      node_now = path_buffer[counter];
      for(int i;i<path_buffer.size();i++)
        if(path_buffer[i] == end)
          fin_flag = 1;
    }    
    node_now = end;
    for(int i=0;i<path_buffer.size();i++)
      if(path_buffer[i] == end){
        node_pre_index = i;
        break;
      }
    while(node_now != start){
      node_pre_index = previous_node[node_pre_index];
      node_pre = path_buffer[node_pre_index];
      e_arr_bonus[node_pre][node_now]++;
      node_now = node_pre;
    }
/*
    //print edge array bonus
    for(int i=0;i< v_num;i++){
      for(int j=0;j< v_num;j++)
        cout<< e_arr_bonus[i][j]<<" ";
      cout<<endl;
    }
*/
    return e_arr_bonus;
}

// create NetworkManager first
NetworkManager *nm = new NetworkManager();

int main(int argc, char** argv){
    // build basic topo
    nm->interpret(argv[1]);
    Vertex *v = nm->get_all_nodes();

    //variable declaration
    ofstream solution;
    vector <string>  v_arr;
    vector <vector<int> > e_arr;
    vector <vector<int> > e_arr_bonus;
    vector <int> degree_arr;
    vector <int> depth_arr;
    vector <int> depth_buffer;
    vector <int> path_buffer;
    int edge_num = 0;
    int node_now = 0;
    int fin_flag = 0;
    int add_flag = 0;
    int next_node = 0;
    int next_node_depth = 0;
    int count_in = 0;
    int count_out = 0;
    int start = 0;
    int end = 0;

    //open file
    solution.open("solution.txt");

    //bulid vertex array
    for(int i=0; v!=NULL; i++)
    {
        v_arr.push_back(v->name);
        v = v->next;       
    }
    
    //bulid edge array
    for(int i=0;i< v_arr.size();i++){
      vector <int> e_arr_temp; 
      for(int j=0;j< v_arr.size();j++)
        if(i==j) e_arr_temp.push_back(0);
        else {
          e_arr_temp.push_back(!nm->connected_d(v_arr[i],v_arr[j]));
          if(!nm->connected_d(v_arr[i],v_arr[j]))
            edge_num++;
        }
      e_arr.push_back(e_arr_temp);
    }

    //build degree array
    for(int i=0;i< v_arr.size();i++){
      count_out = 0;
      count_in = 0;
      for(int j=0;j< v_arr.size();j++){
        if(e_arr[i][j]) count_out++;
        if(e_arr[j][i]) count_in++;
      }
      degree_arr.push_back(count_out-count_in);
    }

    //print degree array
    cout<<"------ degree array ------"<<endl;
    for(int i=0;i< degree_arr.size();i++)
      cout<<"degree_arr["<< i <<"]= "<< degree_arr[i]<<endl;

    //edge_bonus
    fin_flag = 0;
    while(!fin_flag){
      fin_flag = 1;
      add_flag = 0; 
      //find end node
      for(int i=0;i< degree_arr.size();i++)
        if(degree_arr[i] > 0){
          degree_arr[i]--;
          end = i;
          add_flag = 1;
          break;
        }
      //find start node
      for(int i=0;i< degree_arr.size();i++)
        if(degree_arr[i] < 0){
          degree_arr[i]++;
          start = i;
          break;
        }

      //print degree array change
      cout<<"------ change of degeree array ------"<<endl;
      for(int i=0;i< degree_arr.size();i++)
        cout<<"degree_arr["<< i <<"]= "<< degree_arr[i]<<endl;

      //add edge
      if(add_flag){
        e_arr_bonus = shortest_path(start,end,v_arr.size(),e_arr);
        for(int i=0;i< v_arr.size();i++){
          for(int j=0;j< v_arr.size();j++)
            if(e_arr_bonus[i][j]){
              e_arr[i][j] = e_arr[i][j] + e_arr_bonus[i][j];
              edge_num ++;
            }
        }
      }
      //check
      for(int i=0;i< degree_arr.size();i++)
        if(degree_arr[i] != 0){
          fin_flag = 0;
          break;
        }
    }

    //print edge array
    cout<<"------ edge array ------"<<endl;
    for(int i=0;i< v_arr.size();i++){
      for(int j=0;j< v_arr.size();j++)
        cout<< e_arr[i][j]<<" ";
      cout<<endl;
    }
    cout<<"edge_num = "<< edge_num<<endl;
    

    //bulid depth array
    for(int i=0;i< v_arr.size();i++)
      depth_arr.push_back(0);
    depth_arr[node_now] = 1;

    fin_flag = 0;
    while(!fin_flag){
      fin_flag = 1;
      for(int i=0;i< v_arr.size();i++)
        if(e_arr[i][node_now] && depth_arr[i] == 0){
          depth_arr[i] = depth_arr[node_now] +1;
          depth_buffer.push_back(i);
        }
      for(int i=0;i< v_arr.size();i++)
        if(!depth_arr[i]){
          fin_flag = 0;
          node_now = depth_buffer[0];
          depth_buffer.erase(depth_buffer.begin());
          break;
        }
    }
   
    //print depth array
    cout<<"-------depth array-------"<<endl;
    for(int i=0;i< depth_arr.size();i++)
      cout<<"depth_arr["<< i <<"]= "<< depth_arr[i]<<endl;
   

    //go through all graph
    node_now = 0;
    while(edge_num){
      next_node_depth = 0;
      path_buffer.push_back(node_now);
      for(int i=0;i< v_arr.size();i++)
        if(e_arr[node_now][i] && depth_arr[i]>next_node_depth){
          next_node_depth = depth_arr[i];
          next_node = i;
        }
      e_arr[node_now][next_node]--;
      node_now = next_node;
      edge_num --;                   
    }
    path_buffer.push_back(node_now);

    //print path
    cout<<"----------path----------"<<endl;
    cout<<"path:";
    cout<<" "<<v_arr[path_buffer[0]];
    for(int i=1;i<path_buffer.size();i++)
      cout<<"--> "<<v_arr[path_buffer[i]];
    cout<<endl;

    //output path
    solution<<"path:";
    solution<<" "<<v_arr[path_buffer[0]];
    for(int i=1;i<path_buffer.size();i++)
      solution<<"--> "<<v_arr[path_buffer[i]];
    solution<<endl;

    // using gplot to export a dot file, and then using graphviz to generate the figure
    Gplot *gp = new Gplot();
    gp->gp_add(nm->elist);
    gp->gp_dump(true);
    gp->gp_export("topo");

    solution.close();

    return 0;
}
