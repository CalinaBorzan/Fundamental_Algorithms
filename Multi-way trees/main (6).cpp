/*/ In this homework we had to make 3 representations:parent one, multi way,
 * and binary. For the parent one. I simply worked with the parent array,
 * searched for the root which had the index -1 and then recursively printing
 * the nodes that had as index the root and so on.For pretty print I used a
 * for for spaces and one for the recursive function. Then for the multi-way
 * I use an array of pointers to the children for each node.I used an add child
 * function that has as parameters pointers to the node parent and node child.
 * With this we will add to each parent a pointer to its child and pretty print
 * is simply done by recursively calling the function for each child. For the
 * binary representation I used a struct that has pointers to the left and right
 * child. Firstly we create the left child an the a linked list with the right
 * ones, its siblings. This program has the complexity of O(n).
 */


#include <iostream>


using namespace std;


typedef struct R2 {
    int value;
    R2 *children[100];//array of pointers to children of a node
    int size;
};

typedef struct R3 {
    int value;
    R3 *left;//pointer to left child
    R3 *right;//pointer to right child

};

void addchild(R2 *child, R2 *p) {
    if (p->size < 100) {
        p->children[p->size] = child;//child  node is added to children array at position p->size
        p->size++;//new child was added

    }

}

void prettyprint1(int spaces, int a[], int size, int root) {
    if (root == -1) { return; }
    for (int i = 0; i < spaces; i++) { cout << " "; }
    cout << root << endl;

    for (int i = 0; i < size; i++) {
        if (a[i] == root) {
            prettyprint1(spaces + 4, a, size, i + 1);
        }

    }

}

void prettyprint2(int spaces, R2 *node) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < spaces; i++) { cout << " "; }
    cout << node->value << endl;

    for (int i = 0; i < node->size; i++) {
        prettyprint2(spaces + 4, node->children[i]);
    }

}

void prettyprint3(int spaces, R3 *node) {
    if (node == nullptr) {
        return;
    }


    for (int i = 0; i < spaces; i++) { cout << " "; }
    cout << node->value << endl;

    prettyprint3(spaces + 4, node->left);

    prettyprint3(spaces, node->right);
}


void R1() {
    int parent[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int root = -1;
    int size = 9;
    for (int i = 0; i < size; i++) {
        if (parent[i] == -1) {
            root = i + 1;
            break;
        }
    }
    prettyprint1(0, parent, size, root);

}

R2 *root2 = nullptr;

void TR12() {
    int parent[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int size = 9;
    //pointers to the nodes
    R2 *node[size];

    //create nodes for each element in the array
    for (int i = 0; i < size; i++) {
        node[i] = new R2;
        node[i]->value = i + 1;//we add one because we need nodes from 1 to 9
        node[i]->size = 0;

    }
    //find the root node
    for (int i = 0; i < size; i++) {
        if (parent[i] == -1) {
            root2 = node[i];
            break;
        }
    }
    //add children to their parents nodes
    for (int i = 0; i < size; i++) {
        if (parent[i] != -1) {
            addchild(node[i], node[parent[i] - 1]);
        }

    }
    //print the tree starting from root
    prettyprint2(0, root2);
}

R3 *T3(R2 *current) {
    R3 *node = (R3 *) malloc(sizeof(R3));
    node->value = current->value;
    node->left = nullptr;
    node->right = nullptr;
//Recursively we create the left child and a linked list of right children
    if (current->size > 0) {
        node->left = T3(current->children[0]);
        R3 *ptr = node->left;
        for (int i = 1; i < current->size; i++) {
            ptr->right = T3(current->children[i]);
            ptr = ptr->right;
        }
    }
    return node;


}

void TR23() {

    R3 *root3 = T3(root2);
    prettyprint3(0, root3);

}

int main() {
    cout << "Pretty print for R1:\n";
    R1();
    cout << endl;
    cout << "Pretty print for R2:\n";
    TR12();
    cout << endl;
    cout << "Pretty print for R3:\n";
    TR23();
}
