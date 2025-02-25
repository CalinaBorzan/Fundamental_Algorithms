#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <random>
#include <cstdlib>
#include "bfs.h"

using namespace std;
//
//int get_neighbors(const Grid* grid, Point p, Point neighb[]) {
////10 10
////1 1 1 1 1 1 1 1 1 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 0 0 0 0 0 0 0 0 1
////1 1 1 1 1 1 1 1 1 1
//    // Possible moves for a knight
//    int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
//    int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};
//
//    int nr = 0;
//
//    for (int i = 0; i < 8; ++i) {
//        Point newNeighb;
//        newNeighb.row = p.row + dx[i];
//        newNeighb.col = p.col + dy[i];
//
//        // Check if the new position is within the grid limits
//        if (newNeighb.row >= 0 && newNeighb.row < grid->rows &&
//            newNeighb.col >= 0 && newNeighb.col < grid->cols) {
//            neighb[nr] = newNeighb;
//            ++nr;
//        }
//    }
//
//    return nr;
//}

int get_neighbors(const Grid *grid, Point p, Point neighb[]) {
//    10 18
//    1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//    1 0 0 0 1 0 1 0 0 1 0 0 0 1 0 1 0 1
//    1 0 0 0 1 0 0 0 0 1 0 0 0 1 0 0 0 1
//    1 0 0 0 1 1 0 0 1 1 0 0 0 1 1 0 0 1
//    1 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 0 1
//    1 0 1 0 1 0 0 0 1 1 0 1 0 1 0 0 0 1
//    1 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1
//    1 1 0 1 0 1 1 0 1 1 1 0 1 0 1 1 0 1
//    1 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1
//    1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int nr = 0;
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; i++) {
        Point newneighb;
        newneighb.row = p.row + dx[i];
        newneighb.col = p.col + dy[i];
        if (newneighb.row >= 0 && newneighb.row < grid->rows && newneighb.col >= 0 && newneighb.col < grid->cols) {
            neighb[nr] = newneighb;
            nr++;
        }
    }
    return nr;
}

void grid_to_graph(const Grid *grid, Graph *graph) {
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node *) malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            } else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node **) malloc(graph->nrNodes * sizeof(Node *));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node **) malloc(graph->v[i]->adjSize * sizeof(Node *));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node **) realloc(graph->v[i]->adj, k * sizeof(Node *));
            }
        }
    }
}

void free_graph(Graph *graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op) {

    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int i = 0; i < graph->nrNodes; ++i) {
        Node *node = graph->v[i];
        if(op != NULL) op->count(3);
        node->color = COLOR_WHITE;
        node->dist = 0;
        node->parent = nullptr;
    }
    queue<Node *> q;
    if(op != NULL) op->count(4);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = nullptr;
    q.push(s);
    while (!q.empty()) {
        Node *node = q.front();
        for (int index = 0; index < node->adjSize; ++index) {
            Node *neigbh = node->adj[index];
            if(op != NULL) op->count();
            if (neigbh->color == COLOR_WHITE) {
                if(op != NULL) op->count(4);
                neigbh->parent = node;
                neigbh->color = COLOR_GRAY;
                neigbh->dist = node->dist + 1;
                q.push(neigbh);
            }

        }
        q.pop();
        node->color = COLOR_BLACK;
        if(op != NULL) op->count();

    }

}

void prettyprint3(int level, int index, int p[], Point *repr, int n) {
    if (index == -1) {
        return;
    }

    for (int i = 0; i < level; i++) {
        cout << " ";
    }
    cout << "(" << repr[index].row << ", " << repr[index].col << ")" << endl;
    for (int i = 0; i < n; i++) {
        if (p[i] == index) { prettyprint3(level + 4, i, p, repr, n); }
    }
}

void print_bfs_tree(Graph *graph) {
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int *) malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int *) malloc(n * sizeof(int));
    repr = (Point *) malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int rootindex;
        for (int i = 0; i < n; i++) {
            if (p[i] == -1) {
                rootindex = i;
                break;
            }

        }
        prettyprint3(0, rootindex, p, repr, n);

    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) {
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

      bfs(graph, start, nullptr);
        // Check if the end node is reachable
        if (end->color == COLOR_BLACK && end->parent != nullptr) {
            Node *current = end;
            int sizepath = current->dist + 1;
            // Construct the path by backtracking from end to start
            for (int i = sizepath - 1; i >= 0; i--) {
                path[i] = current;
                current = current->parent;
            }

            return sizepath;
        }

        return -1; //can't reach
    }


bool edge_exists(Graph *graph,int node1,int node2)
{
    for (int i = 0; i < graph->v[node1]->adjSize; ++i) {
        if (graph->v[node1]->adj[i]->position.row == node2) {
            return true;
        }
    }
    return false;
}
Node* create_or_get_node(Graph *graph, int position) {
    if (graph->v[position] == NULL) {
        Node *newnode = (Node *)malloc(sizeof(Node));
        memset(newnode, 0, sizeof(Node));
        newnode->position.row = position;
        graph->v[position] = newnode;
    }
    return graph->v[position];
}

void add_edge_to_adj_list(Node *node, Node *neighbor) {
    node->adjSize++;
    node->adj = (Node **)realloc(node->adj, node->adjSize * sizeof(Node *));
    node->adj[node->adjSize - 1] = neighbor;
}
void add_edge(Graph *graph, int node1, int node2) {
    // Ensure that nodes exist or create them
    create_or_get_node(graph, node1);
    create_or_get_node(graph, node2);
    // Add the edge to the adjacency lists
    add_edge_to_adj_list(graph->v[node1], graph->v[node2]);
    add_edge_to_adj_list(graph->v[node2], graph->v[node1]);
}

void generaterandom(Graph *graph, int num_edges) {
    // Connect all nodes initially to ensure connectivity
    for (int i = 0; i < graph->nrNodes; i++) {
        int j=i+1;
            if (!edge_exists(graph, i, j)) {
                add_edge(graph, i, j);
                num_edges--;
            }
        }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, graph->nrNodes - 1);

    while (num_edges > 0) {
        int node1 = distribution(gen);
        int node2 = distribution(gen);

        if (node1 != node2 && !edge_exists(graph, node1, node2)) {

            add_edge(graph, node1, node2);
            num_edges--;
        }
    }
}


void performance() {
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generaterandom(&graph,n);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generaterandom(&graph,4500);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
