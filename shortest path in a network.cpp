#include<bits/stdc++.h>

using namespace std;


/*
 ==> Vertex class which is used to represent vertex of the Graph.
 */

 class Edge;

 class Vertex
 {
   public:
       string name;
       Vertex *prev;
       string status;
       list <Edge> adj;
       double dist;

       Vertex(){}
       Vertex (string str)
       {
         name=str;
         status="up";
         reset();
       }
       void reset()
       {
         dist=DBL_MAX;
         prev=NULL;
       }

 };

 /*
 ==> Edge class which is used to represent Edge of the Graph.
 */

 class Edge
 {
  public:
   Vertex destName;           //Destination node name
   double distance;           // length of path
   Edge(Vertex v,double dist)
   {
    destName=v;
    distance=dist;
   }
 };

 /*
 ==> Path class stores name and distance of the path.
 */

class Path
{
 public:
  string str;
  double dist;
  Path(string str,double dist)
    {
     this->str=str;
     this->dist=dist;
    }
};

 /*
 ==> Pair class represents the string pairs.
 */

 class Pair
 {
   string str1;
   string str2;
   public:
       Pair(string str1,string str2)
       {
        this->str1=str1;
        this->str2=str2;
       }
       string key()
       {
        return str1;
       }
       string value()
       {
        return str2;
       }
 };


 bool operator<(const Path& p1,const Path& p2)
 {

   return p1.dist>p2.dist;
 }


 /*
 ==> Build the network Graph and perform operations on the network.
 */

 class Graph
 {
   map<string,Vertex> vertexMap;    // to store the information about the vertices.
   list<Pair> edgeList;             // add edges to the linked list.
   set<string> visited;         // store visited nodes and sort alphabetically.

   /*------  If a Vertex name is not present, add it to the Vertex Map. In either case return the Vertex.    ------*/

   Vertex getVertex(string vertexName)
   {
     if(vertexMap.find(vertexName)==vertexMap.end())
     {
      // Vertex temp(vertexName);
       vertexMap[vertexName]=Vertex(vertexName);
     }
     return vertexMap[vertexName];
   }

   /*------  Add New Edge to the Graph.    ------*/

   public:
       void addEdge(string sourceName, string destinationName, double distance)
       {
         if(vertexMap.find(sourceName)==vertexMap.end())
         {
          // Vertex temp(vertexName);
          vertexMap[sourceName]=Vertex(sourceName);
         }
         if(vertexMap.find(destinationName)==vertexMap.end())
         {
          // Vertex temp(vertexName);
          vertexMap[destinationName]=Vertex(destinationName);
         }
         //Vertex v=getVertex(sourceName);       // create source vertex if it does not exist
         //Vertex w=getVertex(destinationName);  // create destination vertex if it does not exist
         if(vertexMap[sourceName].adj.size()==0)
         {
          vertexMap[sourceName].adj.push_back(Edge(vertexMap[destinationName],distance));
          return;
         }
         auto it =vertexMap[sourceName].adj.begin();
         while(it!=vertexMap[sourceName].adj.end())           // just check if the edge exist in the list , if yes than update it.
         {
          Edge temp=*it;
          if(temp.destName.name==vertexMap[destinationName].name)
          {
            it->distance=distance;
            return ;
          }

          it++;
         }

        vertexMap[sourceName].adj.push_back(Edge(vertexMap[destinationName],distance));
       }

    /*------  Check for down or disabled edge.    ------*/

    bool isEdgeDown(string source, string destination)
    {
      int check=0;
      auto it=edgeList.begin();
       if(edgeList.size()!=0)
       {
        while(it!=edgeList.end())
        {
          Pair temp=*it;
          if(temp.key()==source && temp.value()==destination)
          {
           check=1;
           break;
          }
         it++;
        }
       }

       if(check==0)
            return false;
       return true;
    }

   /*------  This function will print all the contents of the graph in Alphabetical order.
             If a vertex or a edge is down then it will append the string "Down" near it.     ------*/

   private:

      void print()
      {
       cout<<"\n";
       for(auto it=vertexMap.begin();it!=vertexMap.end();it++)
         visited.insert(it->first);                                // go through all the keys of the hashmap, and add them to visited.

        auto it=visited.begin();
        while(it!=visited.end())
        {
         string adjacentNode=*it;
         cout<<"  "<<adjacentNode<<" neighbors are : \n" ;                           // Print the adjacent Node in Hand
         if(vertexMap[adjacentNode].status!="up")
             cout<<" down";
         set <string> neighbors;
         map<string,double> relation;
         for(Edge key: vertexMap[adjacentNode].adj)
         {
           neighbors.insert(key.destName.name);             // add all the neighbors to the this set.
           relation[key.destName.name]=key.distance;
         }
         auto itr=neighbors.begin();
         while(itr!=neighbors.end())
         {
          string node=*itr;
          cout<<node<<"\t"<<relation[node];
          if(isEdgeDown(adjacentNode,node))
                cout<<" down";
           cout<<"\n";
          itr++;
         }
         cout<<"\n";
         it++;
        }

      }

    /*------  Initializes the vertex output info prior to running any shortest path algorithm.    ------*/

    void clearAll()
    {
      for (auto it = vertexMap.begin();it!=vertexMap.end();it++)      // Basically what it does is it sets the distance of all the nodes in the dijikstra's
                                                                      //table to infinity and there parent's as Null;
      {
       it->second.reset();
      }
    }

    /*------  Process a request entered by user, to exist please enter quit query.    ------*/

   public:
                                                    // just one time check it error may occur in this one
      bool processRequest(Graph& g)
      {
       string str;
       cout<<"\n Enter your Request : ";
       getline(cin,str);
       vector<string> query;
       stringstream ss(str);
       string word;
       while(getline(ss,word,' '))
         query.push_back(word);

       //boost::split(query,str,boost::is_any_of(" "));

       if(query[0]=="print")
             g.print();
       else if(query[0]=="path")
        {
         g.dijkstras(query[1]);
         g.printPath(query[2]);
        }
		else if(query[0]=="edgedown")
            g.edgeDown(query[1],query[2]);

		else if (query[0]== "vertexdown")
            g.vertexDown(query[1]);

		else if (query[0]== "reachable")
            g.checkReachable();

		else if (query[0]=="edgeup")
            g.edgeUp(query[1],query[2]);

		else if (query[0]== "vertexup")
            g.vertexUp(query[1]);

		else if (query[0]== "deleteedge")
		    g.deleteEdge(query[1],query[2]);

		else if (query[0]== "addedge")
		     {
		      double d;
		      istringstream(query[3]) >> d;
              g.addEdge(query[1], query[2], d);
             }
		else if (query[0]=="quit")
                         return false; //exit query
		else
             cout<<"\n Not valid query ";

        return true;
      }

     /*------  Dijkstra's: shortest-path algorithm using MinHeap.    ------*/

    string getMinimum(map<string,double> table)
    {
     string str="";
     double minimum=DBL_MAX;
     for(auto it=table.begin();it!=table.end();it++)
     {
       if(it->second<minimum)
       {
        minimum=it->second;
        str=it->first;
       }

     }
     if(str=="")
     {
      return table.begin()->first;
     }

     return str;
    }

   void dijkstras(string startVertex)
   {
     if(vertexMap.find(startVertex)==vertexMap.end())
     {
      cout<<"\n Initial Vertex is NULL ";               // If initial Vertex Not found.
      return;
     }

     if(vertexMap[startVertex].status !="up")                     //If vertex is disabled, return.
     {
      cout<<"\n Initial Vertex is down";
      return;
     }

    clearAll();
    map<string,double> table;
    for(auto it=vertexMap.begin();it!=vertexMap.end();it++)
    {
     table[it->first]=it->second.dist;
    }

    table[startVertex]=0.0;
    vertexMap[startVertex].dist=0.0;
    vertexMap[startVertex].prev=NULL;
    set<string> check;
    while(!table.empty())
    {

     string str=getMinimum(table);
     check.insert(str);
     for(Edge edge : vertexMap[str].adj)
     {
      if(check.find(edge.destName.name)!=check.end())
        continue;

      if(vertexMap[edge.destName.name].status!="up" || isEdgeDown(str,edge.destName.name))
        continue;

      if(edge.distance+table[str]<table[edge.destName.name])
      {
       table[edge.destName.name]=edge.distance+table[str];
       vertexMap[edge.destName.name].dist=table[edge.destName.name];
       vertexMap[edge.destName.name].prev =&vertexMap[str];
      }

     }

      table.erase(str);
    }


   }
   // end of dijkstras.

   /*------  Recursive root to print the shortest path.    ------*/
   private:
     void printPath(Vertex dest)
     {
       Vertex *prev =(dest.prev);
       if(prev==NULL)
       {
        cout<<"\n Source : "<<dest.name;
        return;
       }
       printPath(*prev);
       cout<<" "<<dest.name;
     }

   /*------  Driver routine to print total distance.    ------*/
   public:
     void printPath(string destinationName)
     {
       if(vertexMap.find(destinationName)==vertexMap.end())
       {
        cout<<"\n Destination vertex not found ";
        return;
       }
       Vertex v=vertexMap[destinationName];
       if(v.dist==DBL_MAX)
       {
        cout<<"\n Unreachable Destination : "<<destinationName;
        return;
       }
       cout<<"\n";
       printPath(v);
       cout<<"\n The total distance is : "<<v.dist;
       cout<<"\n";
     }

   /*------  Disable the edge and we cannot further traverse through that route.    ------*/
     void edgeDown(string source,string destination)
     {
      // First check if this edge exist in edge list or not,if yes: do nothing, else add it to the edge list.
      list<Pair>::iterator it=edgeList.begin();
      while(it!=edgeList.end())
      {
       Pair temp=*it;
       if(temp.key()==source && temp.value()==destination)
            return;
        it++;
      }

      Pair p(source,destination);
      edgeList.push_back(p);
     }

    /*------  Enable the edge and we can further traverse through that route.    ------*/
      void edgeUp(string source,string destination)
      {
       int pos=-1;
       list<Pair>::iterator address;
       list<Pair>::iterator it;
       for(it=edgeList.begin();it!=edgeList.end();it++)
       {
        Pair temp=*it;
        if(temp.key()==source && temp.value()==destination)
        {
         address=it;
         pos=1;
         break;
        }
       }
       if(pos==1)
        {
         edgeList.erase(address);
         cout<<"\n The Edge from "<<source<<"to "<<destination<<"is enabled. ";
         cout<<"\n";
        }
       else
       {
         cout<<"\n The Edge from "<<source<<" to "<<destination<<" does not exist. ";
         cout<<"\n";
       }


      }
    /*------   Update status of vertex to down.If vertex is made down, it will disable all the Edges connected to it.    ------*/
      void vertexDown(string s)
      {
        if(vertexMap.find(s)==vertexMap.end())
        {
         cout<<"\n There is no such vertex ";
         return;
        }
        vertexMap[s].status="down";
      }

    /*------   Update status of vertex to Up.If vertex is made up, it will enable all the Edges connected to it.    ------*/
      void vertexUp(string s)
      {
        if(vertexMap.find(s)==vertexMap.end())
        {
         cout<<"\n There is no such vertex ";
         return;
        }
        vertexMap[s].status="up";
      }
    /*------   Delete Edge from the Graph.    ------*/
      void deleteEdge(string source,string destination)
      {
        // First check that both of the nodes are a part of Graph of or not.
        if(vertexMap.find(source)==vertexMap.end() || vertexMap.find(destination)==vertexMap.end())
          {
           cout<<"\n This edge does not exist.";
           return;
          }
          auto it=vertexMap[source].adj.begin();
       while(it!=vertexMap[source].adj.end())
        {
         string temp=it->destName.name;
         if(temp==destination)
            break;
         it++;
        }
       if(it!=vertexMap[source].adj.end())
          vertexMap[source].adj.erase(it);
      }

    private:
  /*------   Recursive routine called by checkReachable() method.    ------*/
     void reachable(string node)
      {
       set<string> adjacent;
       for(Edge key: vertexMap[node].adj)
       {
        if(isEdgeDown(node,key.destName.name) || key.destName.status=="down")
            continue;
        adjacent.insert(key.destName.name);
       }
       for(string s:adjacent)
        {
         if(visited.find(s)!=visited.end())
            continue;
          visited.insert(s);
          reachable(s);
        }
      }
  /*------   It displays the vertices accessible from each of the vertex in alphabetical order.    ------*/
     void checkReachable()
     {
       set<string>uniquekeys;
       for(auto it=vertexMap.begin();it!=vertexMap.end();it++)
       {
        string temp=it->first;
        if(vertexMap[temp].status=="down")
            continue;
         uniquekeys.insert(temp);
       }
       auto it=uniquekeys.begin();
       while(it!=uniquekeys.end())
       {
        string firstNode=*it;
        visited.clear();
        visited.insert(*it);
        cout<<" \n"<<firstNode <<"\n";
        reachable(firstNode);
        for(string str:visited)
        {
          if(str!=firstNode)
            cout<<" "<<str;
        }
        it++;
       }
     }
 };
  /*
    Now we will read a file that contains edges, Build the graph from it, perform action related to queries given and finally stops at quit query.
  */
int main()
{
  Graph g;
  string path_of_file;
  vector <string> result;
  string line;
  cout<<"\n Enter Path of file ";
  getline(cin,path_of_file);
  ifstream In(path_of_file.c_str());
  try
  {
   if(!In)
   {
    cerr<<"\n Could not open file : "<<path_of_file;
    throw "Program Terminated";
   }

   while(getline(In,line))
   {
      result.clear();
     // split the string on the basis of space.
     stringstream ss(line);
     string word;
     while(ss>>word)
        result.push_back(word);
     try
     {
       if(result.size()!=3)
       {
         cerr<<"\n Skipping ill formated line : "<<line;
         throw line;
       }
       string source = result[0];
       string destination = result[1];
       double d;
       istringstream(result[2]) >> d;
       g.addEdge(source, destination, d);
       g.addEdge(destination,source, d);
     }
     catch (string line)
     {
      cout<<"\n The format of the line should be \"source destination distance\". ";
     }
   }
  }
  catch(string str)
   {
    cout<<str<<"\n";
    In.close();
    exit(1);
   }
  In.close();
  while(g.processRequest(g));
  return 0;
}
//H:/Material science and technology/example.txt
