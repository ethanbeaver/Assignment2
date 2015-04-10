/*****************************************************************************
 Program file name:ethan.beaver.cpp	 OS: Ubuntu 14.04		Assignment #:2
 Programmer:Ethan Beaver	Class: 	Data Struct. II			Date:4/10/15
 Compiler:GNU GCC

 Assistance/references:
 Description: Builds graphs and tells whether they're connected or not using DFS and BFS functions
 Inputs: Options and data
 Outputs: Varies

Special Comments: Menu code written by Ryan Rabello for a past assignment.

~~~~~~~~~~~~~~~~~~~~~~~~~~Grading Criteria~~~~~~~~~~~~~~~~~~~~~~~~~~

 Assignment Requirements ___/3.0

 Code Format/Cosmetics ___/3.0

 Header & Code Comments: ___/2.0

 Output Format/Cosmetics ___/2.0
 ***Does Not Compile***: ___ (-10.0)
 ***Late Work Deduction***: ___ (-0.5/day)
 Total Grade: ___/10.0

*****************************************************************************/


#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;

template <class T>
class Queue
{
    private:
    T *Q;
    int size;
    int front;
    int rear;

    public:
    Queue();
    Queue(int);
    bool emptyQ();
    bool fullQ();
    void addQ(T);
    T delQ();
    int lengthQ();
    void writeQ();
    void clearQ();
};

template <class T>
Queue<T>::Queue()
{
    size = 20;
    Q = new T[size];
    front = rear = 0;
}

template <class T>
Queue<T>::Queue(int setsize)
{
    size = setsize;
    Q = new T[size];
    front = rear = 0;
}

template <class T>
bool Queue<T>::emptyQ()
{
    if(front == rear)
        return true;
    else
        return false;
}

template <class T>
bool Queue<T>::fullQ()
{
    if(front == (rear+1)%size)
        return true;
    else
        return false;
}

template <class T>
void Queue<T>::addQ(T data)
{
    if(fullQ())
        cout << "\aError: Queue Overflow.\n";
    else
    {
        rear = (rear+1)%size;
        Q[rear] = data;
    }
}

template <class T>
T Queue<T>::delQ()
{
    static T nothing;
    if(emptyQ())
    {
        cout << "\aError: Queue Underflow\n";
        return nothing;
    }
    else
    {
        front = (front+1)%size;
        return Q[front];
    }
}

template <class T>
int Queue<T>::lengthQ()
{
    if(rear>=front)
        return (rear-front);
    else
        return(size+rear-front);
}

template <class T>
void Queue<T>::writeQ()
{
    if(front==rear)
        cout << "Queue is Empty." << endl;
    else
    {
        cout << "Queue: ";
        for(int i=1; i<=lengthQ(); i++)
            cout << Q[(front+i)%size] << "  ";
        cout << endl;
    }
}

template <class T>
void Queue<T>::clearQ()
{
    front = rear = 0;
}

template<typename U>
class Graph //The class that makes up the graph!
{
    private:
    class Vertex //The graph is made up of vertices, which are coded as a class
    {
        public:
        U data; //Vertices contain data and an adjacency list.
        list<int> adjacencyList;
    };
    vector<Vertex> vertices; //The graph class has a list(vector) of all vertices

    public:
    void build(); //Functions for working with graphs
    void out();
    void connected();
    void DFS(int);
    void BFS(int);
    vector<bool> visited; // Declare visited vector
    int miniMenu(bool);
    int menu(Graph<string>);
};


template<typename U>
void Graph<U>::build() //Function to build the graph one vertex at a time
{
    Vertex V;
    cout << "\nPlease input data for a vertex of the graph (no spaces): "; //Input data for the vertex
    cin >> V.data;
    vertices.push_back(V); //Add the vertex to the vertex list
    if(vertices.size()>0) //If there is more than one vertex in the vertex list,
    {
        char answer = 'N';
        for(int i=1;i<vertices.size();i++) //In this for-loop is code to connect the newly added vertex to whichever existing vertices the user wants to
        {
            cout << "Connect this vertex to the vertex titled '" << (vertices.at(i-1)).data << "'?(Y-yes, N-no)" << endl;
            cin >> answer;
            answer = toupper(answer);
            if(answer == 'Y')
            {
                vertices.at(i-1).adjacencyList.push_back(vertices.size()-1); //Connects other to itself
                vertices.at(vertices.size()-1).adjacencyList.push_back(i-1); //Connects itself to other
                cout << vertices.at(vertices.size()-1).data << " and " << (vertices.at(i-1)).data << " are now connected!\n";
            }
        }
    }
}

template<typename U>
void Graph<U>::out() //Code for debugging purposes only to output the data and connections of each vertex; other functions won't work after running this.
{
    for(int i=0;i<vertices.size();i++)
    {
        cout << "Vertex number: " << i << endl;
        cout << "Data of vertex:" << vertices.at(i).data << endl;
        while(vertices.at(i).adjacencyList.size()>0)
        {
            cout << "Vertex is conncected to: " << vertices.at(i).adjacencyList.front() << endl;
            vertices.at(i).adjacencyList.pop_front();
        }
        cout << endl;
    }
}

template<typename U>
void Graph<U>::connected() //Function to tell if the graph is connected or not
{
    bool isConnected = 1;
    for(int i=0;i<25;i++)
        visited.push_back(0); //Initialize the visited array to all zeros(not visited)
    int choice = 1;
    int starting;
    cout << "This function uses DFS or BFS to determine if the graph is connected.\n";
    cout << "Enter 1 for DFS or 2 for BFS: ";
    cin >> choice; //Choose to run the DFS or BFS function to determine connectivity
    if(choice == 1)
    {
        cout << "Which vertex to start with? Enter 0-" << vertices.size()-1 << endl;
        cin >> starting;
        DFS(starting);
    }
    else if(choice == 2)
    {
        cout << "Which vertex to start with? Enter 0-" << vertices.size()-1 << endl;
        cin >> starting;
        BFS(starting);
    }
    for(int k=0;k<(vertices.size());k++) //Use the visited array populated by DFS or BFS to determine connectivity
    {
        if(visited[k])
            isConnected = 1*isConnected;
        else
            isConnected = 0*isConnected;
    }
    if(isConnected) //Output whether or not the graph is connected
        cout << "The graph is connected!" << endl;
    else
        cout << "The graph is not connected" << endl;
    for(int k=0;k<(vertices.size());k++) //Reset the visited array to not connected so as not to mess with it running again.
    {
        visited[k] = 0;
    }
}

template<typename U>
void Graph<U>::DFS(int v) //Function to perform Depth First Search
{
    int sizeV = vertices.at(v).adjacencyList.size();
    if(visited.size() <= v) //Make sure the visited array is big enough
    {
        visited.resize(v+1);
    }
    visited[v] = 1; //Set the current vertex to visited
    for(int i=0; i<sizeV; i++) //Recursively visit every unvisited vertex that can be accessed from the current vertex
    {
        if(!(visited.at(vertices.at(v).adjacencyList.front())))
        {
            DFS(vertices.at(v).adjacencyList.front());
        }
        if(vertices.at(v).adjacencyList.size()>0)
        {
            vertices.at(v).adjacencyList.pop_front();
        }
    }
}

template<typename U>
void Graph<U>::BFS(int v) //Function to perform Breadth First Search
{
    Queue<int> Q(25); //Declare Variables
    int temp;
    if(visited.size() <= v) //Make sure the visited array is big enough
    {
        visited.resize(v+1);
    }
    visited[v] = 1; //Set the current vertex to visited
    Q.addQ(v); //Add the current vertex to the queue
    while(!Q.emptyQ()) //While the queue isn't empty, add all vertices that aren't visited and are accessible from the current queue to the queue, then mark them as visited
    {
        int temp = Q.delQ();
        while(vertices.at(v).adjacencyList.size()>0)
        {
            if(!visited.at(vertices.at(v).adjacencyList.front()))
            {
                visited[vertices.at(v).adjacencyList.front()] = 1;
                Q.addQ(vertices.at(v).adjacencyList.front());
            }
            vertices.at(v).adjacencyList.pop_front();
        }
    }
    visited[temp] = 1;
}

template<typename U>
int Graph<U>::miniMenu(bool isMiniMenu)
//A menu that prints options, and takes the user's choice
{
    char choice;
    if (isMiniMenu){
    cout << endl << "What would you like to do?" << endl;
    cout << endl;
    cout << "\t1) Build an undirected graph by adding a vertex to it." << endl;
    cout << "\t2) Check if the graph is connected using either a DFS or BFS function." << endl;
    cout << "\t3) Output the vertices and their connections. Warning: Only for Debugging. Don't try to run anything after running this." << endl;
    cout << endl << "\t0) Quit." << endl;
    }
    //if they have already seen the menu it prints the mini menue. As follows.
    else
    {
        cout << "\n\n1)Add Vertex\t\t2)IsItConnected?\t3)Output\n";
    }
    cout << endl;
    cout << ">>>";
    cin >> choice;
    cout << endl;
    return choice;
}

template<typename U>
int Graph<U>::menu(Graph<string> G)
{
    //initialize some menu options
    char option = NULL;
    bool printMenu = true;
    //start the main menu loop, breaks when option== 0;
    while(option!='0')
    {
        option = miniMenu(printMenu);
        switch(option)
        {
            case '0': cout << "  _____             ____\n"
                << " / ___/__  ___  ___/ / /  __ _____ \n"
                << "/ (_ / _ \\/ _ \\/ _  / _ \\/ // / -_)\n"
                << "\\___/\\___/\\___/\\_,_/_.__/\\_, /\\__/ \n"
                << "                        /___/      \n";
            break;
            case '1':
                G.build();
                printMenu = false;
                break;
            case '2':
                G.connected();
                printMenu = false;
                break;
            case '3':
                G.out();
                printMenu = false;
                break;
            default : cerr << "\nERROR: '" << option << "' is not a valid menu option.\n\n"; printMenu = true; break;
        }
    }
    return(0);
}




int main()
{
    cout << "Welcome to the Graph Program!" << endl; //Declare a Graph of type string and run the menu
    Graph<string> G;
    G.menu(G);
}
