#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define smallSize 100
#define Edge_L 500
#define SIZE 10000
#define QueSize 2000000
#define Length 7000000

struct queue
{
  int items[QueSize];
  int front;
  int rear;
};

struct node
{
  unsigned int vertexNumber;        //  for saving every nodes index, relating Node ---> Graph
  unsigned int kevin_NO;            //  every node's Kevin bacon number, they are -1 when initialization
  unsigned int type;                //  differentiating between movie and actors, 0 indicating movie and 1 indicating actor
  char name[smallSize];             //  name of actor or movie
  struct node* edges[Edge_L];       //  neighbors of a vertex
  struct node* parent;              //  will be use to know previous node(the node that we came from) while breadth first searching
};

struct table   // used for storing actors that have already been searched
{
    struct node* nodes[20];
};


struct Graph    // our main graph
{
  struct node* nodes[Length];  // actors node   //  movie nodes are not connected to the main graph, they are connected to actor nodes
  int visited[Length];         // for storing actor nodes that are visited, array indexes correspond to every nodes vertexNumber
  int movieVisited[Length];    // for storing visited movie nodes, movie nodes are not connected to the graph, but they have VertexNumbers
};                              // so it is possible to relate array indexes to the movie nodes vertexNumbers

struct queue* createQueue();
void enqueue(struct queue* q, int value);
int dequeue(struct queue* q);
int isEmpty(struct queue* q);
int* bfs_byNumbers(struct Graph* head, int start_vertex);//First Module of the Assignment, returns an array with number of actors with [1,2,..N] Kevin Bacon Number
int bfs(struct Graph* head, int start_vertex, char name[], struct table* table1);//Second Module of the Assignment, breadth first search for a specific actor
void search_by_Name(struct Graph* head, struct table* table1); // Third Module is included in this function, doesn't repeat BFS for an actor
void print_relations(struct table* head, int index); // for printing Kevin bacon number of an actor and relations
int readFROMfile(struct Graph* head, char fileName[]); // function that reads from file
void nullify_graph(struct Graph* head);   //  for initializing graph's nodes as null
void nullify_edges(struct node* head);    //  for initializing every nodes edges as null
int does_exist(struct Graph* head, char name[]); // for getting the index of any actor's node from the graph
void addMovie(struct Graph* head, int index, struct node* movie); // connecting movie node to actor node and vice versa
int getEmptyIndex(struct table* table1);  // getting an empty index from the table for adding new searched actor
int check_if_saved(struct table* table1, char name[]);// used for checking if an actor has already been searched, printing its relation without BFS
struct node* createNode(char name[], int type); // for creating node for every actor and movie when reading from the file
void nullify_table(struct table* table1); // for making sure our table is empty at first initialization
void nullify(struct Graph* head); // for zeroing out visited nodes, it is needed when doing breadth first search again, resetting graph after operations
void selection1(struct Graph* head); // If Option one is selected, i wanted to have clean operations inside Switch case, so i created this function
void menu(struct Graph* head);      // main Menu

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == QueSize - 1)
    printf("\nQueue is Full!!\n");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    printf("Queue is empty\n");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      q->front = q->rear = -1;
    }
  }
  return item;
}

// Create a queue
struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

// Creating new node, returns new node
struct node* createNode(char name[], int type)
{
  struct node* newNode =(struct node*)malloc(sizeof(struct node));
  strcpy((newNode->name), name); // assigning the name
  newNode->parent = NULL; // making it's parent null, it will change in BFS function
  newNode->type = type;   // type Movie = 0, Actor = 1;
  newNode->vertexNumber = -1; // it should be -1 at first, because index zero is taken by the graph visited array.
  newNode->kevin_NO = -1;  // every nodes kevin no should be -1 at first
  nullify_edges(newNode);  // make all the edges null
  return newNode;
}

// for making sure every node of the graph is null at first
void nullify_graph(struct Graph* head)
{
    for(int i=0; i<Length; i++)
    {
        head->nodes[i] = NULL;
        head->visited[i] = 0;
        head->movieVisited[i] = 0;
    }
}

// for making table's edges null at first
void nullify_table(struct table* table1)
{
    for(int i = 0; i<20; i++)
    {
        table1->nodes[i] = NULL;
    }
}

//  for making sure every edge is null at first
void nullify_edges(struct node* head)
{
    for(int i = 0; i<Edge_L; i++)
    {
        head->edges[i] = NULL;
    }
}

// for resetting the graph after operations
void nullify(struct Graph* head)
{
    for( int i = 0; i<Length; i++)
    {
        head->visited[i] = 0;
        head->movieVisited[i] = 0;

    }
}

// checks if an actor is already in the graph or not, return the index of the actor node, or return -1 if doesn't exist
int does_exist(struct Graph* head, char name[])
{
    unsigned int index = 0; // used for finding the index of an specific actor
    while( head->nodes[index] != NULL )
    {
        if(!strcmp(name, (head->nodes[index]->name)))
        {
            return index;  //  returns the index of the actor node
        }

        index++;
    }
    return -1;   // returns -1 if the actor is not in the graph
}

// Breadth first search, returns the index of the actor node which is connected to the table, not to the graph, -1 if the actor doesn't exist
int bfs(struct Graph* head, int start_vertex, char actorName[], struct table* table1)// Seond Module of the Assignment
{
    struct queue* q = createQueue();
    struct node* tmp; // used for iterating through the graph

    head->visited[start_vertex] = 1;      // mark the first node as visited
    enqueue(q, start_vertex);             //  put the value of that nodes vertex number to the queue
    head->nodes[start_vertex]->kevin_NO = 0; // Kevin Bacon's , Kevin Bacon number is 0;

    while( !isEmpty(q) )
    {
        int currentVertex = dequeue(q);  // dequeue an actor node's vertexNumber, used for visiting it's edges
        tmp = head->nodes[currentVertex];  //  tmp points to the actor node,  tmp = Actor
        if(currentVertex == does_exist(head, actorName)) // if the dequeued vertex number is same as that actor's vertex number, it means we have
        {                                           // reached the actor by bfs so return the actor's vertex number
            int empty = getEmptyIndex(table1);
            table1->nodes[empty] = tmp; // add it to the table for not searching for this actor again
            return empty; // retun vertex number if the actor has a relation with Kevin Bacon
        }
        int i = 0; // used for iterating through graph->node[] array
        while(tmp->edges[i] != NULL)       //  if Actor(tmp) ---> Movie is not null
        {
            if(head->movieVisited[tmp->edges[i]->vertexNumber] == 0)  // if Actor(tmp) ---> Movie is not visited // no need to check the movie vertex
            {                                                    // again, because if a movie is visited, it means its all nodes are already visited
                // Movie ----> Actor , making parent of the movie point at the actor to keep track of the node we came from
                tmp->edges[i]->parent = tmp;
                head->movieVisited[tmp->edges[i]->vertexNumber] = 1;  // mark the movie as visited

                int j = 0; // used for iterating through actors of a movie
                while(tmp->edges[i]->edges[j] != NULL)  //  Actor(tmp)  ---> Movie ----> Actor ,  if it is not null
                {
                    int adjVertex = tmp->edges[i]->edges[j]->vertexNumber;  //   Actor(tmp) --->Movie--->Actor, get vertex number
                    if(head->visited[adjVertex] == 0)
                    {

                        head->nodes[adjVertex]->parent = tmp->edges[i];  // parent of the actor point at the previous node, will be needed
                        head->visited[adjVertex] = 1; // mark the node as visited
                        head->nodes[adjVertex]->kevin_NO = ((tmp->kevin_NO)+1); // write this nodes Kevin bacon number as previous actor node's plus one
                        enqueue(q, adjVertex);
                    }
                    j++;
                }
            }
            i++;
        }
    }
    return -1;  // return -1 if the actor doesn't have any relation with Kevin Bacon
}

// this function is used for computing the number of actors with Kevin Bacon number, returns an array with kevin bacon numbers
int* bfs_byNumbers(struct Graph* head, int start_vertex) // difference between this BFS function and the previous BFS function is that the
{                                                   // previous one stops after the actor is found and returns its index, this function returns
    struct queue* q = createQueue();                // an array of numbers
    struct node* tmp;  // for iterating through the graph
    static int arr[10] = {0}; // for saving the number of actors with kevin bacon numbers

    head->visited[start_vertex] = 1;      // mark the first node as visited
    enqueue(q, start_vertex);    //  put the value of that nodes vertex number to the queue
    head->nodes[start_vertex]->kevin_NO = 0; // Kevin Bacon's , Kevin Bacon number is 0;

    while( !isEmpty(q) )
    {
        int currentVertex = dequeue(q);
        tmp = head->nodes[currentVertex];  //  tmp points to the actor  tmp = Actor
        int i = 0;
        while(tmp->edges[i] != NULL)       //  if Actor(tmp) ---> Movie is not null
        {

            if(head->movieVisited[tmp->edges[i]->vertexNumber] == 0)  // if Actor(tmp) ---> Movie is not visited
            {
                // Movie ----> Actor , making parent of the movie point at the actor to keep track of the node we came from
                tmp->edges[i]->parent = tmp;
                head->movieVisited[tmp->edges[i]->vertexNumber] = 1;  // movie visited

                int j = 0;
                while(tmp->edges[i]->edges[j] != NULL)  //  Actor(tmp)  ---> Movie ----> Actor ,  if it is not null
                {

                    int adjVertex = tmp->edges[i]->edges[j]->vertexNumber;  //   Actor(tmp) --->Movie--->Actor get vertex number

                    if(head->visited[adjVertex] == 0)
                    {
                        head->nodes[adjVertex]->parent = tmp->edges[i];  // parent of the actor point at the previous node
                        head->visited[adjVertex] = 1;
                        head->nodes[adjVertex]->kevin_NO = ((tmp->kevin_NO)+1);
                        arr[(tmp->kevin_NO)+1]++; // incrementing array accordingly
                        enqueue(q, adjVertex);
                    }
                    j++;
                }
            }
            i++;
        }
    }
    return arr;  // return the array of actors with [1,2,3,4,...N] kevin bacon numbers
}

// for reading from file, returns 0 if file didn't open, 1 if file completely read
int readFROMfile(struct Graph* head, char fileName[])
{
    FILE *file;
    char line[SIZE];  // for storing every line of the text file
    char actorName[SIZE]; // for storing actors names
    char movieName[SIZE]; // for storing movie names
    struct node* tmpActor; // for creating new actor node and storing it
    struct node* tmpMovie; // for creating new movie node and storing it

    file = fopen(fileName , "r");
    if ( file == NULL)
    {
        printf("Could no read the file\n");
        return 0;
    }
    int movie_Count = 0;  //  for counting number of movies
    int graph_vertics_counter = 0;  // for counting number of actors

    fgets(line, SIZE, file);    //   buffering first line of text file then separating the line as words inside while loop

    while( !feof(file) )  // until end of file is reached
    {
        int i = 0;  // for keeping track of every character of line[] array we got from text file
        int len;    // getting line[] array length
        len = strlen(line);

        for(int k = 0; k< SIZE; k++)  // making sure the array elements are null
        {
            movieName[k] = 0;
        }

        while( i < len)        // separating line as words
        {
            for(int k=0; k< SIZE; k++) // making sure the array elements are null
            {
                actorName[k] = 0;
            }

            if( i == 0 )  //  the first part is movie name so it should be added separately as having type 0, type zero indicates movie
            {
                int j = 0;
                while( line[i] != '/' && line[i] != NULL)
                {
                    movieName[j] = line[i];
                    i++;
                    j++;
                }

                tmpMovie = createNode(movieName,0);
                tmpMovie->vertexNumber = movie_Count;
                movie_Count++;
            }
            else
            {
                int j =0; // used for reading line character by character
                while( line[i] != '/' && line[i] != NULL && line[i] != '\n')
                {
                    actorName[j] = line[i];  // get character by character
                    i++;
                    j++;
                }
                //printf("Actor Name is : %s\n", actorName);
                int index = does_exist(head, actorName); // returns -1 if the actor is not in the list

                if( index != -1) // if the actor is in the list
                {
                    addMovie(head, index, tmpMovie); //  connect the actor to it's movie
                }
                else // else create a new node for the actor then connect that actor to it's movie
                {
                    tmpActor = createNode(actorName, 1); // create a new node for the actor, type = 1 indicating actor node, type = 0 indicating movie
                    tmpActor->vertexNumber = graph_vertics_counter; // assign vertexNumber
                    head->nodes[graph_vertics_counter] = tmpActor;  // add the actor to the graph
                    head->visited[graph_vertics_counter] = 0;    // mark it not visited
                    addMovie(head, graph_vertics_counter, tmpMovie); // connected the movie with the actor and vice versa
                    graph_vertics_counter++;
                }
            }
            i++;
        }
        fgets(line, SIZE, file);
    }
    printf("Total Number of Movies: %d\n", movie_Count);
    fclose(file);
    return 1;
}

// for connecting movie with actors and actors with movie
void addMovie(struct Graph* head, int index, struct node* movie)
{
    int i = 0, j = 0; // i is used for getting an empty edge from
    while( head->nodes[index]->edges[i] != NULL) // increment i until an empty place is found for adding the movie
    {
        i++;
    }
    while( movie->edges[j] != NULL) // increment j until an empty slot is reached for adding the actor
    {
        j++;
    }
    head->nodes[index]->edges[i] = movie; // connecting actor to movie
    movie->edges[j] = head->nodes[index]; //  connecting movie to actor
}

// for printing an actors Kevin bacon number and the actors relation with Kevin bacon
void print_relations(struct table* table1, int index)
{
    struct node* tmp = table1->nodes[index]; // for iterating through parent nodes like this,  KevinBacon <---Movie <--- Actor <---Movie <---Actor
    char movieName[smallSize]; // name of the movie
    char actorName1[smallSize];// first actor's name
    char actorName2[smallSize];// second actors name
    int kevin_no = 0; // to keep track of the keving bacon number of the actor

    if(index != -1) // true if the actor has a relation with Kevin Bacon
    {
        printf("%s Kevin Bacon Number is %d: \n", table1->nodes[index]->name, table1->nodes[index]->kevin_NO);

        tmp = table1->nodes[index];
        while(tmp != NULL && strcmp("Bacon, Kevin", tmp->name) )  // break if Kevin bacon is reached
        {

            strcpy(actorName1, tmp->name);
            tmp = tmp->parent;

            strcpy(movieName, tmp->name);
            tmp = tmp->parent;

            strcpy(actorName2, tmp->name);

            printf("%s - %s : %s\n", actorName1, actorName2, movieName);

        }
    }
    else // if the actor doesn't have any relation with Kevin bacon
    {
        printf("Kevin Bacon Number is : Sonsuz\n");
    }
    printf("\n");
}

void search_by_Name(struct Graph* head, struct table* table1)  // if option 2 is selected from main function, search by an actors Kevin bacon number
{
    char kevin[] = "Bacon, Kevin";
    char *p; // for making sure fgets doesn't include \n character
    int kevinIndex = does_exist(head, kevin);  // storing location of Kevin Bacon Node, because it's not always in the first node of the graph
    char actorName[smallSize]; //  for getting actors name from user

    printf("Enter the Name of The Actor as 'Surname, Name' \n");

    fgets(actorName, smallSize, stdin );

    if((p = strchr(actorName, '\n')) != NULL)
    {
        *p = '\0';
    }

    printf("Please Wait!!\n");
    int index = check_if_saved(table1, actorName); // getting index of the actor if it is already in the table
    if(index != -1) // print from table if it already exists in the table
    {
        printf("Printing From Already Existing Tables:\n");
        print_relations(table1, index );
        return;
    }

    int bfsIndex = bfs(head, kevinIndex, actorName, table1);
    print_relations(table1, bfsIndex);

}

int getEmptyIndex(struct table* table1)
{
    int i = 0;
    while(table1->nodes[i] != NULL)
    {
        i++;
    }
    return i;
}

int check_if_saved(struct table* table1, char name[])
{
    unsigned int index = 0;
    while( table1->nodes[index] != NULL )
    {
        if(!strcmp(name, (table1->nodes[index]->name)))
        {
            return index;  //  returns the index of the actor node
        }
        index++;
    }
    return -1;   // returns -1 if the actor is not in the saved list
}

void selection1(struct Graph* head)
{
    int *p; // for storing returned array from bfs_byNumber function
    p = bfs_byNumbers(head, does_exist(head, "Bacon, Kevin"));
    printf("Number of Actor with 0 Bacon Number : 1\n");
    int i = 1; // for reading through array
    while(*(p+i) != 0)
    {
        printf( "Number of Actors with %d Bacon Number is: %d\n", i, *(p+i) );
        i++;
    }
    i = 0;
    int j = 0;
    while( head->nodes[i] != NULL)
    {
        if(head->nodes[i]->kevin_NO == -1)
        {
            j++;
        }
        i++;
    }
    printf("Number of Actors with infinite Bacon Number is: %d\n",j);
    nullify(head);
    for(i=0; i<20; i++)
    {
        *(p+i) = 0;
    }
}

void menu(struct Graph* head)
{
    int cikis = 1;
    struct table *table1 = (struct table*)malloc(sizeof(struct table));
    nullify_table(table1);
    while(cikis)
    {
        char choice;
        printf(" 1 . For Listing Every Actors Kevin Bacon Number\n");
        printf(" 2 . For Searching for an Actors Kevin Bacon Number\n");
        printf(" 3 . For Exit\n");
        printf("Select One of The Options: \n");

        choice = getchar();
        getchar();

        switch(choice)
        {
            case '1':
            {
                selection1(head);
                break;
            }
            case '2':
            {

                search_by_Name(head, table1);
                nullify(head);
                break;
            }
            case '3':
            {
                cikis = 0;
                break;
            }
            default:
            {
                printf("Wrong Input\n");
                continue;
                break;
            }
        }
    }
}

int main() {

    struct Graph *head = (struct Graph*) malloc(sizeof(struct Graph));
    char *p;
    nullify_graph(head);

    char fileName[smallSize];
    printf("Enter the Name of the File with .txt extension\n");

    fgets(fileName, smallSize, stdin);

    if((p = strchr(fileName, '\n')) != NULL)
    {
        *p = '\0';
    }

    printf("Please Wait!!\n");
    if(!(readFROMfile(head, fileName))) // end the program if file didn't open
    {
        return 0;
    }
    menu(head);

    return 0;
}
