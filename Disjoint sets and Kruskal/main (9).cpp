/*/Disjoint sets are sets that are never over-lapping.We do this by having an array of nodes which
 * we use Make_Set to make 10 sets with this 10 nodes.The Union of these sets is done by Find_Set which
 * verifies where each number is located.This function uses path compression which shortens the path of the
 * node to its representative, making it more efficient.Then we do Union which works with the rank of each set.
 * The node will be linked to the node that has a bigger rank as to make sure we keep the ranks as low as possible.
 *The time complexity for Find Set is O( E log V) because it involves comparing ranks.Time complexity for Make Set is O(1).
 * Time complexity for Union is O(log n).
 *Then we apply Kruskal with the help of the disjoint sets function.This is done by sorting the edges of the graph
 * in function of weights and then we see if they are in the same set.If they are different it means the edge can be
 * added into MST cause thwy will not create a cycle.Union merges them.The time complexity will be O(size*log(size)).
 */

#include <iostream>
#include <random>
#include <cstdlib>
#include "Profiler.h"

#define MAX_SIZE 1000
#define STEP_SIZE 10
#define NR_TESTS 5

using namespace std;
Profiler prof("Kruskal");
typedef struct node {
    node *parent;
    int value;
    int rank;
};

typedef struct edge {
    int x;
    int y;
    int weight;
};

node *Make_Set(int value, Operation make) {
    node *x = (node *) malloc(sizeof(node));
    x->parent = nullptr;
    x->rank = 0;
    x->value = value;
    make.count(3);
    return x;
}

node *Make_Set_Demo(int value) {
    node *x = (node *) malloc(sizeof(node));
    x->parent = nullptr;
    x->rank = 0;
    x->value = value;
    return x;
}

node *FS(node *x, Operation find) {
//if the node is a root it becomes its representative
    find.count();
    if (x->parent == nullptr) {
        return x;
    }
    node *repr = FS(x->parent, find);
//we set the parent as the representative to do path compression, to make subsequent searches more efficient
    x->parent = repr;
    find.count(2);
    return repr;
}

node *FS_Demo(node *x) {
//if the node is a root it becomes its representative

    if (x->parent == nullptr) {
        return x;
    }
    node *repr = FS_Demo(x->parent);
//we set the parent as the representative to do path compression, to make subsequent searches more efficient
    x->parent = repr;
    return repr;
}


node *Link(node *x, node *y, Operation link) {
    link.count(3);
    if (x->rank > y->rank) {
        y->parent = x;
        link.count();

    } else if (x->rank < y->rank) {
        x->parent = y;
        link.count();
    } else {
        x->parent = y;
        link.count(2);
        y->rank++;
    }

    return y;
}

node *Link_Demo(node *x, node *y) {

    if (x->rank > y->rank) {
        y->parent = x;
    } else if (x->rank < y->rank) {
        x->parent = y;
    } else {
        x->parent = y;
        y->rank++;
    }

    return y;
}

node *Union(node *x, node *y, Operation unions) {
    return Link(FS(x, unions), FS(y, unions), unions);
}

node *Union_Demo(node *x, node *y) {
    return Link_Demo(FS_Demo(x), FS_Demo(y));
}

void sets() {
    node *sets[10]; // Array of pointers
    for (int i = 1; i <= 10; i++) {
        sets[i] = Make_Set_Demo(i);
    }

    for (int i = 1; i <= 10; i++) {
        if (sets[i] != nullptr) {
            node *repr = FS_Demo(sets[i]);
            cout << repr->value << " ";
        }
    }
    cout << endl;

    Union_Demo(sets[2], sets[1]);
    Union_Demo(sets[4], sets[3]);
    Union_Demo(sets[8], sets[4]);
    for (int i = 1; i <= 10; i++) {
        if (sets[i] != nullptr) {
            node *repr = FS_Demo(sets[i]);
            cout << repr->value << " ";
        }
    }
    cout << endl;
    Union_Demo(sets[9], sets[3]);
    for (int i = 1; i <= 10; i++) {
        if (sets[i] != nullptr) {
            node *repr = FS_Demo(sets[i]);
            cout << repr->value << " ";
        }
    }
}

void sort(edge *edges, int nrvert) {

    for (int i = 0; i <nrvert; i++) {
        for (int j = i + 1; j <nrvert; j++) {
            if (edges[i].weight > edges[j].weight) {
                auto aux = edges[i];
                edges[i]=edges[j];
                edges[j]= aux;
            }
        }
    }
}

void Kruskal(edge *edges, int nrvert, int size) {

    Operation make = prof.createOperation("Make_Set", nrvert);
    Operation find = prof.createOperation("Find_Set", nrvert);
    Operation unions = prof.createOperation("Union_Set", nrvert);
    node *sets[nrvert];
    for (int i = 0; i < nrvert; i++) {
        sets[i] = Make_Set(i, make);
    }

    sort(edges, size);

    for (int i = 0; i < size; i++) {
        node *x = sets[edges[i].x];
        node *y = sets[edges[i].y];

        if (FS(x, find) != FS(y, find)) {
            Union(x, y, unions);
        }
    }
}

void Kruskal_Demo(edge *edges, int nrvert, int size) {

    node *sets[nrvert];
    for (int i = 0; i < nrvert; i++) {
        sets[i] = Make_Set_Demo(i);

    }

    sort(edges, size);
    for (int i = 0; i < size; i++) {
        node *x = sets[edges[i].x];
        node *y = sets[edges[i].y];


        if (FS_Demo(x) != FS_Demo(y)) {
            cout << edges[i].x << " - " << edges[i].y << " : " << edges[i].weight << endl;
            Union_Demo(x, y);
        }
    }
}


void generated(int nredge, int nrvertices, edge *edges) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, nrvertices - 1);

    bool matrix[nrvertices][nrvertices];
    for (int count = 0; count < nredge; count++) {
        int i = 0, j = 0, weightr = 0;
        do {
            i = distribution(gen);
            j = distribution(gen);
            weightr = distribution(gen);

        } while (matrix[i][j] == true || i == j);
        matrix[i][j] = true;
        matrix[j][i] = true;
        edges[count].x = i;
        edges[count].y = j;
        edges[count].weight = weightr;
        // cout << edges[count].x << " " << edges[count].y << "\n";
    }

}

void demokruskal() {
    int n = 5;  // number of nodes
    int const nEdges = 9;
    edge edges[nEdges] = {
            {1, 2, 2},
            {1, 3, 4},
            {2, 3, 1},
            {2, 4, 1},
            {3, 4, 7},
            {3, 0, 5},
            {4, 0, 6},
            {1, 0, 8},
            {2, 0, 9},

    };

    Kruskal_Demo(edges, n, nEdges);
}

void perf() {
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int expe = 0; expe < NR_TESTS; expe++) {
            auto edges = new edge[n * 4];
            generated(n * 4, n, edges);
            Kruskal(edges, n, n * 4);
        }
    }
    prof.divideValues("Make_Set", NR_TESTS);
    prof.divideValues("Uni_Set", NR_TESTS);
    prof.divideValues("Find_Set", NR_TESTS);
    prof.showReport();
}

int main() {
   // sets();
    //cout << endl;
     //demokruskal();
    perf();

}
