/*
 * During the DFS, for each node, Tarjan's algorithm assigns two values: index and lowlink.
index is an incremental counter, starting from 0, assigned to each node as it's visited.
lowlink is initially set to the same value as index, and it represents the smallest index of any node that can be reached from the current node.
onstack is used to track whether a node is currently on the stack.
 */



#include <iostream>
#include <stdlib.h>
#include <stack>
#include "Profiler.h"
#include <random>
#include <cstdlib>

using namespace std;
Profiler prof("DFS");
using namespace std;

int timedfs = 0;

typedef struct Node {
    int adjSize;
    int discoverytime;
    int finishtime;
    struct Node** adj;
    int value;
    int color;
    int index;
    int lowlink;
    bool onstack;
    struct Node* parent;
} Node;

typedef struct {
    int nrNodes;
    Node** v;
} Graph;

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

Node* DFS_VISIT(Graph* graph, Node* s, stack<Node*>& stacktop,Operation *op) {
    timedfs = timedfs + 1;
    if(op != NULL) op->count(2);
    s->discoverytime = timedfs;
    s->color = COLOR_GRAY;//it was visited
    for (int index = 0; index < s->adjSize; ++index) {
        Node* neighb = s->adj[index];//if adjacent node is not visited, visit it
        if(op != NULL) op->count();
        if (neighb->color == COLOR_WHITE) {
            if(op != NULL) op->count();
            neighb->parent = s;//set its parent
            DFS_VISIT(graph, neighb, stacktop,op);

        }
    }
    if(op != NULL) op->count(2);
    s->color = COLOR_BLACK;//mark the current node as black
    timedfs = timedfs + 1;
    s->finishtime = timedfs;
    stacktop.push(s);//put it on stack for topological sort
    return s;
}

void dfs(Graph* graph, stack<Node*>& stacktop,Operation *op) {
    for (int i = 0; i < graph->nrNodes; ++i) {
        Node* node = graph->v[i];
        if(op != NULL) op->count(2);
        node->color = COLOR_WHITE;//mark all nodes with white
        node->parent = nullptr;
    }
    timedfs = 0;//reset the global time

    for (int i = 0; i < graph->nrNodes; ++i) {
        Node* node = graph->v[i];
        if(op != NULL) op->count();
        if (node->color == COLOR_WHITE) {
            DFS_VISIT(graph, node, stacktop, op);
        }
    }
}
void topologicalSort(Graph* graph) {
    stack<Node*> stacktop;
    dfs(graph, stacktop, nullptr);

    cout << "Topological Sort:" << endl;
    while (!stacktop.empty()) {
        cout << stacktop.top()->value << " ";
        stacktop.pop();
    }
    cout << endl;
}

void printG(Graph* graph) {
    for (int i = 0; i < graph->nrNodes; ++i) {
        cout << "Node " << graph->v[i]->value << " is connected to:";
        for (int j = 0; j < graph->v[i]->adjSize; ++j) {
            cout << graph->v[i]->adj[j]->value << " ";
        }
        cout << endl;
    }
}
Node* createnode(int index) {
    Node* newnode = new Node;
    newnode->value = index;
    newnode->adjSize = 0;
    newnode->adj = nullptr;
    newnode->color = COLOR_WHITE;
    newnode->index = -1;
    newnode->lowlink = 0;
    newnode->onstack = false;
    newnode->discoverytime = 0;
    newnode->finishtime = 0;
    return newnode;
}


void add_edge(Graph* graph, Node* node1, Node* node2) {
    Node** newAdj = new Node*[node1->adjSize + 1];
    for (int i = 0; i < node1->adjSize; ++i) {
        newAdj[i] = node1->adj[i];
    }
    newAdj[node1->adjSize] = node2;
    node1->adj = newAdj;
    node1->adjSize++;
}

void print_tree(Node* node, int level) {
    if (node == nullptr) {
        return;
    }
    // Print the current node
    for (int i = 0; i < level; ++i) {
        cout << " ";
    }
    cout << node->value << endl;

    // Print child nodes
    for (int i = 0; i < node->adjSize; i++) {
        Node* child = node->adj[i];
        if (child->discoverytime > node->discoverytime && child->finishtime < node->finishtime) {
            print_tree(child, level + 4);
        }
    }
}
void tarjanDFS(Graph* graph, Node* s, stack<Node*>& stacktop, int& index, int& nr) {
    s->index = index;
    s->lowlink = index;
    s->onstack = true;
    stacktop.push(s);
    index++;

    for (int i = 0; i < s->adjSize; ++i) {
        Node* neighb = s->adj[i];

        if (neighb->index == -1) {
            // Node has not been visited yet
            tarjanDFS(graph, neighb, stacktop, index, nr);
            s->lowlink = min(s->lowlink, neighb->lowlink);
        } else if (neighb->onstack) {
            // Back edge to a node in the strongly connected
            s->lowlink = min(s->lowlink, neighb->index);
        }
    }

    // Check if the node is a root of a strongly connected
    if (s->lowlink == s->index) {
        cout << "Strongly Connected Component:" << ++nr << ":";
        Node* nodescc;
        do {
            nodescc = stacktop.top();
            stacktop.pop();
            nodescc->onstack = false;
            cout << " " << nodescc->value;
        } while (nodescc != s);
        cout << endl;
    }
}

void tarjan(Graph* graph) {
    stack<Node*> stacktop;
    int index = 0;
    int nr = 0;

    for (int i = 0; i < graph->nrNodes; ++i) {
        Node* node = graph->v[i];
        if (node->index == -1) {//if the node has not been visited,index=-1, iterate through  it
            tarjanDFS(graph, node, stacktop, index, nr);
        }
    }
}

bool edge_exists(Graph* graph, int node1Index, int node2Index) {
    Node* node1 = graph->v[node1Index];
    for (int i = 0; i < node1->adjSize; ++i) {
        if (node1->adj[i]->value == node2Index) {
            return true;
        }
    }
    return false;
}

void generaterandom(Graph *graph, int num_edges) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, graph->nrNodes - 1);

    // Step 1: Connect all nodes to ensure connectivity
    for (int i = 0; i < graph->nrNodes - 1; ++i) {
        if (!edge_exists(graph, i, i + 1)) {
            add_edge(graph, graph->v[i], graph->v[i + 1]);
            --num_edges;
        }
    }

    // Step 2: Add remaining edges randomly
    while (num_edges > 0) {
        int node1 = distribution(gen);
        int node2 = distribution(gen);

        // Ensure no self-loop or duplicate edge
        if (node1 != node2 && !edge_exists(graph, node1, node2)) {
            add_edge(graph, graph->v[node1], graph->v[node2]);
            --num_edges;
        }
    }
}


void demodfs() {
    Node *nodes[7];
    for (int i = 0; i < 7; i++) {
        nodes[i] = createnode(i);
    }
    Graph *graph = new Graph;
    graph->nrNodes = 7;
    graph->v = nodes;
    add_edge(graph, nodes[0], nodes[1]);
    add_edge(graph, nodes[0], nodes[2]);
    add_edge(graph, nodes[1], nodes[3]);
    add_edge(graph, nodes[2], nodes[4]);
    add_edge(graph, nodes[4], nodes[6]);
    add_edge(graph, nodes[4], nodes[5]);
    add_edge(graph, nodes[3], nodes[5]);
    add_edge(graph, nodes[3], nodes[4]);

    cout << "The initial graph is:" << endl;
    printG(graph);
    cout << endl;
    stack<Node *> stacktop;
    dfs(graph, stacktop, nullptr);
    cout << "\nDFS Tree:" << endl;

    Node *node = graph->v[0];

    if (node->color == COLOR_BLACK) {
        print_tree(node, 0);
    }

    Node *nodes2[7];
    for (int i = 0; i < 7; i++) {
        nodes2[i] = createnode(i);
    }
    Graph *graph2 = new Graph;
    graph2->nrNodes = 7;
    graph2->v = nodes2;
    add_edge(graph2, nodes2[0], nodes2[1]);
    add_edge(graph2, nodes2[0], nodes2[2]);
    add_edge(graph2, nodes2[1], nodes2[3]);
    add_edge(graph2, nodes2[2], nodes2[4]);
    add_edge(graph2, nodes2[4], nodes2[6]);
    add_edge(graph2, nodes2[3], nodes2[5]);
    add_edge(graph2, nodes2[3], nodes2[4]);

    cout << "The initial graph is:" << endl;
    printG(graph2);
    cout << endl;
    topologicalSort(graph2);


    Node *nodes3[7];
    for (int i = 0; i < 7; i++) {
        nodes3[i] = createnode(i);
    }
    Graph *graph3 = new Graph;
    graph3->nrNodes = 7;
    graph3->v = nodes3;
    add_edge(graph3, nodes3[0], nodes3[1]);
    add_edge(graph3, nodes3[2], nodes3[0]);
    add_edge(graph3, nodes3[1], nodes3[2]);
    add_edge(graph3, nodes3[1], nodes3[3]);
    add_edge(graph3, nodes3[2], nodes3[4]);
    add_edge(graph3, nodes3[4], nodes3[2]);
    add_edge(graph3, nodes3[4], nodes3[6]);
    add_edge(graph3, nodes3[3], nodes3[5]);
    add_edge(graph3, nodes3[3], nodes3[4]);

    cout << "The initial graph is:" << endl;
    printG(graph3);
    cout << endl;
    tarjan(graph3);
}

void performance() {
    int n, i;
    Profiler p("dfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("dfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = new Node*[graph.nrNodes];
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = createnode(i);
        }

        ///generate n random edges
        generaterandom(&graph,n);
        stack<Node*> stacktop;
        dfs(&graph, stacktop, &op);

    }
    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = new Node*[graph.nrNodes];
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = createnode(i);
        }
        //generate 4500 random edges
        generaterandom(&graph,4500);
        stack<Node*> stacktop;
        dfs(&graph, stacktop, &op);

    }

    p.showReport();
}

int main() {
    demodfs();
//    performance();
    return 0;
}
