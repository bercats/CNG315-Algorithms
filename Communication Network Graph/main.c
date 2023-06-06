#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Berke Diler 240150
 * CNG 315 - Assignment 3
 * This program uses graph adjacency representation.
 */

//A structure for e-mails
typedef struct email {
    int email_id;
    char sender[50];
    char recipient[50];
    int day_of_the_month;
    char *content;
    int num_of_words;
}Email;

//struct for representing a vertex in the graph
typedef struct vertex {
    char name[50];
    int num_of_sent_emails;
    int num_of_received_emails;
    int num_of_words_received;
    struct Edge *edges;
}vertex;
//struct for representing an edge in the graph
typedef struct Edge {
    vertex *recipient;
    vertex *sender;
    int num_of_words; //the weight of the edge
    struct Edge *next;
}Edge;
//struct for representing a graph
typedef struct Graph {
    int num_of_vertices;
    //an array of vertices
    vertex *vertices;
}Graph;
//function prototype for creating a vertex
vertex *createVertex(char *name, Graph *graph);
//function prototype for creating an edge that takes the graph, sender's name, recipient's name and the number of words as parameters
Edge *createEdge(Graph *graph, char *sender, char *recipient, int num_of_words);
//function prototype for printing the graph
void printGraph(Graph *graph);
//readEmails function prototype
Graph readEmails(char *pathname, int num_of_files);
//function prototype for checking path
int checkPath(Graph *graph, char *sender, char *recipient);
int main() {
    //ask the user the number of files to be read
    int num_of_files;
    printf("Enter the number of files to be read: ");
    scanf("%d", &num_of_files);
    //ask the user the pathname of the files
    char pathname[50];
    printf("Enter the pathname of the files: ");
    scanf("%s", pathname);
    //read the emails
    Graph graph = readEmails(pathname, num_of_files);
    //print the graph
    printGraph(&graph);
    //ask the user for the sender and recipient
    char sender[50];
    char recipient[50];
    printf("Enter the sender: ");
    scanf("%s", sender);
    printf("Enter the recipient: ");
    scanf("%s", recipient);
    //check if there is a path between the sender and the recipient
    int result = checkPath(&graph, sender, recipient);
    if(result == 1) {
        printf("There is a path between %s and %s", sender, recipient);
    } else {
        printf("There is no path between %s and %s", sender, recipient);
    }

    return 0;

}

//function for creating a vertex
vertex *createVertex(char *name, Graph *graph) {
    //check if the person is already added to the graph
    for (int i = 0; i < graph->num_of_vertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return &graph->vertices[i];
        }
    }
    //if the person is not added to the graph, add it
    graph->vertices = (vertex *) realloc(graph->vertices, sizeof(vertex) * (graph->num_of_vertices + 1)); //increase the size of the array
    strcpy(graph->vertices[graph->num_of_vertices].name, name); //copy the name of the person to the array
    graph->vertices[graph->num_of_vertices].num_of_sent_emails = 0; //initialize the number of sent emails to 0
    graph->vertices[graph->num_of_vertices].num_of_received_emails = 0; //initialize the number of received emails to 0
    graph->vertices[graph->num_of_vertices].num_of_words_received = 0; //initialize the number of words received to 0
    graph->vertices[graph->num_of_vertices].edges = NULL; //initialize the edges to NULL
    graph->num_of_vertices++; //increase the number of vertices by 1
    return &graph->vertices[graph->num_of_vertices - 1]; //return the vertex
}

//function for creating an edge
Edge *createEdge(Graph *graph, char *sender, char *recipient, int num_of_words) {
    //create a new edge
    Edge *edge = (Edge *) malloc(sizeof(Edge));
    edge->sender = createVertex(sender, graph); //create a vertex for the sender
    edge->recipient = createVertex(recipient, graph); //create a vertex for the recipient
    edge->num_of_words = num_of_words; //set the number of words
    edge->next = NULL; //initialize the next edge to NULL
    //add the edge to the sender's edges
    if (edge->sender->edges == NULL) {
        edge->sender->edges = edge;
    } else {
        Edge *temp = edge->sender->edges;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = edge;
    }
    //increase the number of sent emails of the sender by 1
    edge->sender->num_of_sent_emails++;
    //increase the number of received emails of the recipient by 1
    edge->recipient->num_of_received_emails++;
    return edge;
}
int count_words(char *str) { // count the number of words in the email content
    int count = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            count++;
        }
        i++;
    }
    return count+1;
}
char *int_to_string(int num) { // convert an integer to a string
    char *str = malloc(10);
    sprintf(str, "%d", num);
    return str;
}
char *concatenate(char *path, int num) { // concatenate the path with the number of the email
    char *str = malloc(100);
    strcpy(str, path);
    strcat(str, "/");
    strcat(str, int_to_string(num));
    strcat(str, ".txt");
    return str;
}
    //readEmails function
    Graph readEmails(char *pathname, int num_of_files) {
        //create a graph
        Graph graph;
        graph.num_of_vertices = 0;
        graph.vertices = NULL;
        struct email *emails = malloc(sizeof(struct email) * num_of_files);
        //read the emails
        FILE *fp;
        printf("Reading the emails... \n");
        int j= 1;
        for(int i = 0;i<num_of_files;i++){
            char *filename = concatenate(pathname, j);
            fp = fopen(filename, "r");
            if (fp == NULL) {
                printf("Error opening file \n");
                exit(1);
            }
            //read the file and store the data in the emails array
            emails[i].content = malloc(1000);
            fscanf(fp, "%d \n From: %s \n To: %s \n Date: %d \n ", &emails[i].email_id, emails[i].sender, emails[i].recipient, &emails[i].day_of_the_month);
            fgets(emails[i].content, 1000, fp);
            emails[i].num_of_words = count_words(emails[i].content);
            createVertex(emails[i].sender, &graph);
            createVertex(emails[i].recipient, &graph);
            createEdge(&graph, emails[i].sender, emails[i].recipient, emails[i].num_of_words);
            //update the number of words received by the recipient
            for (int k = 0; k < graph.num_of_vertices; k++) {
                if (strcmp(graph.vertices[k].name, emails[i].recipient) == 0) {
                    graph.vertices[k].num_of_words_received += emails[i].num_of_words;
                }
            }
            fclose(fp);
            j++;
        }
        printf("Reading the emails is done. \n");


        //find the people who have recieved the maximum number of words
        printf("Creating the graph is done. \n");
        //print the vertices
        printf("Printing the vertices... \n");
        for (int i = 0; i < graph.num_of_vertices; i++) {
            printf("Name: %s \n", graph.vertices[i].name);
            printf("Number of sent emails: %d \n", graph.vertices[i].num_of_sent_emails);
            printf("Number of received emails: %d \n", graph.vertices[i].num_of_received_emails);
            printf("Number of words received: %d \n", graph.vertices[i].num_of_words_received);
            printf("Edges: \n");
            Edge *temp = graph.vertices[i].edges;
            while (temp != NULL) {
                printf("Sender: %s \n", temp->sender->name);
                printf("Recipient: %s \n", temp->recipient->name);
                printf("Number of words: %d \n", temp->num_of_words);
                temp = temp->next;
            }
            printf("-------------------------------------------------- \n");
        }
        printf("Printing the vertices is done. \n");
        return graph;
    }
    //printGraph function
    void printGraph(Graph *graph) {
       printf("The resulting graph's adjacency list is as follows: \n");
       for(int i = 0; i < graph->num_of_vertices ; i++)
       {
           printf("%s -> ", graph->vertices[i].name);
           for(Edge *edge = graph->vertices[i].edges; edge != NULL; edge = edge->next)
           {
               printf("%s | %d -> ", edge->recipient->name, edge->num_of_words);
           }
              printf("NULL\n");
       }
       int maxSent = 0;
       for(int i = 0; i < graph->num_of_vertices ; i++)
       {
           if(graph->vertices[i].num_of_sent_emails > maxSent)
           {
               maxSent = graph->vertices[i].num_of_sent_emails;
           }
       }
         printf("The people who have sent the maximum number of emails are: \n");
            for(int i = 0; i < graph->num_of_vertices ; i++)
            {
                if(graph->vertices[i].num_of_sent_emails == maxSent)
                {
                    printf("%s sent %d emails. \n", graph->vertices[i].name, maxSent);
                }
            }
            //find the maximum number of words in total that a person has received
            int maxReceived = 0;
            for(int i = 0; i < graph->num_of_vertices ; i++)
            {
                if(graph->vertices[i].num_of_words_received > maxReceived)
                {
                    maxReceived = graph->vertices[i].num_of_words_received;
                }
            }
            //find the people who have received the maximum number of words
            printf("The people who have received the maximum number of words are: \n");
            for(int i = 0; i < graph->num_of_vertices ; i++)
            {
                if(graph->vertices[i].num_of_words_received == maxReceived)
                {
                    printf("%s recieved %d words \n", graph->vertices[i].name, graph->vertices[i].num_of_words_received);
                }
            }
    }
    //checkPath checks if there is a path between two vertices in the graph and returns 1 if there is a path and 0 if there is no path between the two vertices in the graph, uses BFS
    int checkPath(Graph *graph, char *sender, char *recipient)
    {
        int senderIndex = -1;
        int recipientIndex = -1;
        for(int i = 0; i < graph->num_of_vertices; i++)
        {
            if(strcmp(graph->vertices[i].name, sender) == 0)
            {
                senderIndex = i;
            }
            if(strcmp(graph->vertices[i].name, recipient) == 0)
            {
                recipientIndex = i;
            }
        }
        if(senderIndex == -1 || recipientIndex == -1)
        {
            return 0;
        }
        int *visited = malloc(sizeof(int) * graph->num_of_vertices);
        for(int i = 0; i < graph->num_of_vertices; i++)
        {
            visited[i] = 0;
        }
        int *queue = malloc(sizeof(int) * graph->num_of_vertices);
        int front = 0;
        int rear = 0;
        queue[rear] = senderIndex;
        rear++;
        visited[senderIndex] = 1;
        while(front != rear)
        {
            int current = queue[front];
            front++;
            for(Edge *edge = graph->vertices[current].edges; edge != NULL; edge = edge->next)
            {
                int index = -1;
                for(int i = 0; i < graph->num_of_vertices; i++)
                {
                    if(strcmp(graph->vertices[i].name, edge->recipient->name) == 0)
                    {
                        index = i;
                    }
                }
                if(visited[index] == 0)
                {
                    queue[rear] = index;
                    rear++;
                    visited[index] = 1;
                }
            }
        }
        if(visited[recipientIndex] == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
