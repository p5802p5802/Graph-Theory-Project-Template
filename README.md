# E24046307 Project report
## 圖的設定
1. 圖為有向圖
2. 每個邊的權重值皆為1
3. 起始點為最小輸入點.（ex: 當圖的點為b,c,d 則以b為起始點）

## 演算法思路
1. 讀取圖片後,轉成二維的點點關係圖
2. 利用該點點關係圖,取得各點的indegree與outdegree
3. 若各點的indegree = outdegree則此問題為一筆劃問題
4. 若indegree ！= outdegree 則在degree不相等的兩點間增加edge 使各點的indegree = outdegree
5. 完成一筆劃問題

## 程式碼解釋
```
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <map>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"
#include "fstream"

using namespace std;
```
此段程式碼宣告所需的標頭檔

```
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
```

此function的目的在於找兩點間的最短路徑 並回傳哪些路徑需要多走一次  
e_arr_bonus 用於紀錄兩點間的所經路徑,之後用於加入edge matrix中  
path_buffer 用於紀錄兩點間的所經節點  
previous_node 用於紀錄前一個節點在path_buffer中的位置  
node_now 用於紀錄現在的節點位置  
node_pre_index 用於紀錄前一個節點在path_buffer中的位置  
node_pre 用於紀錄前一個節點是誰  
fin_flag 用於紀錄while loop是否完成目標工作  
counter 用於紀錄目前BFS演算法走到哪  

```
//initialize e_arr_bonus;
    for(int i=0;i< v_num;i++){
     vector <int> e_arr_bonus_temp; 
     for(int j=0;j< v_num;j++)
       e_arr_bonus_temp.push_back(0);
     e_arr_bonus.push_back(e_arr_bonus_temp);
    }
```
把e_arr_bonus 的元素初始化成0  
```
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
    
    return e_arr_bonus;
}
```
這段程式碼利用BFS找輸入兩點間的最短路徑並將經過的路徑紀錄到e_arr_bonus並回傳之  
```
// create NetworkManager first
NetworkManager *nm = new NetworkManager();
```
宣告一個新的NetworkManager
```
int main(int argc, char** argv){
    // build basic topo
    nm->interpret(argv[1]);
    Vertex *v = nm->get_all_nodes();
```
把輸入的圖特徵檔餵給NetworkManager,藉以得到圖的各種資訊
```
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
```
solution 用於打開輸出檔案  
v_arr 用於紀錄各點  
e_arr 用於紀錄各邊  
e_arr_bonus 用於紀錄shortest_path的回傳值  
degree_arr 用於紀錄各點degree  
depth_arr 用於紀錄各點與起點的距離,距離較大則優先訪問（DFS演算法）  
path_buffe 用於紀錄所經路徑  
edge_num 用於紀錄邊的總數  
node_now 用於紀錄現在的點  
fin_flag 用於紀錄while loop是否完成目標工作  
add_flag 用於紀錄是否需要進行shortest_path的運算  
next_node 用於紀錄下一個點是誰  
next_node_depth 用於紀錄下一個點的深度  
count_in 用於紀錄目前的點的indegree  
count_out 用於紀錄目前的點的outdegree  
start 用於紀錄shortest_path起始點  
end 用於紀錄shortest_path終點  
```
//open file
    solution.open("solution.txt");
```
打開輸出檔solution.txt
```
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
```
利用nm->connected_d判斷兩點間是否有邊,並建立邊的關係圖
```
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
```
利用上面的關係圖取得各點之間的degree關係（如果>0,則outdegree > indegree;如果<0,則outdegree < indegree）
```
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
```
利用上述取得的degree關係,若非所有degree等於0,則將degree為正的點與degree為負的點之間兩兩配對  
並利用shortest_path加到邊的關係圖中
```
//print edge array
    cout<<"------ edge array ------"<<endl;
    for(int i=0;i< v_arr.size();i++){
      for(int j=0;j< v_arr.size();j++)
        cout<< e_arr[i][j]<<" ";
      cout<<endl;
    }
    cout<<"edge_num = "<< edge_num<<endl;
```
在終端機上印出邊的關係圖
```
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
```
利用BFS演算法計算出各點離起點的深度並紀錄之
```
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
```
利用上面取得的深度關係進行DFS演算法,處理一筆劃問題
```
//print path
    cout<<"----------path----------"<<endl;
    cout<<"path:";
    cout<<" "<<v_arr[path_buffer[0]];
    for(int i=1;i<path_buffer.size();i++)
      cout<<"--> "<<v_arr[path_buffer[i]];
    cout<<endl;
```
在終端機上印出路徑結果
```
//output path
    solution<<"path:";
    solution<<" "<<v_arr[path_buffer[0]];
    for(int i=1;i<path_buffer.size();i++)
      solution<<"--> "<<v_arr[path_buffer[i]];
    solution<<endl;
```
在solution.txt輸出路徑結果
```
// using gplot to export a dot file, and then using graphviz to generate the figure
    Gplot *gp = new Gplot();
    gp->gp_add(nm->elist);
    gp->gp_dump(true);
    gp->gp_export("topo");

    solution.close();

    return 0;
```
把圖特徵轉成dot檔

## 程式執行方式
```
make 
./main.out topo.txt //topo.txt can change into any filename you need
```

## 程式執行結果
1. topo1.txt  
![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo.png)


![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo_result.png)


2. topo2.txt  
![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo2.png)


![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo2_result.png)


3. topo3.txt  
![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo3.png) 


![image](https://github.com/p5802p5802/Graph-Theory-Project-Template/blob/master/topo3_result.png)


