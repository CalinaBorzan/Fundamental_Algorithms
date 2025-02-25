/*/ In this lab we had to construct a balanced Binary Search Tree in which we used a divide and conquer algorithm
 * for constructing the tree and os_select and os_delete to find and delete nodes. A binary search tree is a tree
 * in which every nodes on left branch is smaller than its parent, and every node on every right branch is bigger than
 * its parent. With os_select we were supposed to find the i-th smallest element in the tree based on its rank and
 * with the help of the size. To construct the tree I used a divide and conquer algorithm in which I worked with the middle
 * of the numbers from 1 to n-number of nodes,and after that the element on the left subtree will be represented by
 * the ones that are in the left part of the array, and the right subtree by the right elements of the remained array.
 * The os_select works by searching on the left subtree for children, if there are the node's rank receives size of the left
 * subtree plus one.We recursively search in the left subtree  for it if the rank is smaller than the k-th smallest element
 * we want to search we go in the left subtree,else if is bigger we go in the right. For deletion we have 3 cases, when we
 * have no child, when we have only one child and when we have 2 children. When we have no child we simply free the memory
 * of that node, when we have only when we replace that current node with its child,wether is a left one or right one
 * and free the memory, and when we have 2 children we work with successor parent and successor.This means that we will
 * start searching by going to the right child of the node and then on its left subtree until we have no more left nodes.
 * We do this as to find the smallest element that will be greater than the left part of the node that needs to be deleted and
 * smaller than all of the right side.Then we copy its value in place of the node that needs to be deleted and remake the
 * links between the child and parents.
 * After that we make the profiler and the graph.The graph shows us that both the deletion and the selection when use n times
 * will become linear even though if used only once for n nodes they have the time complexity of O(h) where h is the height.
 *
 *
 *
 */

#include <iostream>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

using namespace std;
Profiler prof("BST");

typedef struct node {
    struct node *left;
    struct node *right;
    struct node*parent;
    int value;
    int size;//nodes present in the subtree
};


node *divide(int left, int right, node *parent) {//we use divide and conquer method
    if (left > right) { return nullptr; }

    node *c = (node *) malloc(sizeof(node));
    c->value = (left + right) / 2;
    int mid = (left + right) / 2;
    c->parent=parent;
    c->size = right - left + 1;
    c->left = divide(left, mid - 1, parent);
    c->right = divide(mid + 1, right, parent);
    return c;
}

node *Build_Tree(int n) {
    node *root = nullptr;
    return divide(1, n, root);

}


void updatesize(node*curr,Operation op1) {
    int leftSize, rightSize;
    while (curr != nullptr) {
        if (curr->left != nullptr) {
            leftSize = curr->left->size;
            //we go on the left subtree and update the size
            op1.count();
        } else {
            leftSize = 0;
        }

        if (curr->right != nullptr) {
            rightSize = curr->right->size;
            //we go on the right subtree and update the size
        } else {
            rightSize = 0;
        }

        curr->size = leftSize + rightSize + 1;
        op1.count();
        curr = curr->parent;
    }
}
node *Os_Select(node *node, int i, Operation op) {
    if (node == nullptr) { return nullptr; }//we didn't find the node
    int rank;//the i-th smallest
    //check if there is a left child
    if (node->left != nullptr) { //if there is we set the rank to the size of the left subtree plus 1
        {
            rank = node->left->size + 1;//everything that is in its left is smaller than the node that is why we have size+1
            op.count();
        }
    } else { rank = 1; }

    if (i == rank) { return node; }

    else if (i < rank) { return Os_Select(node->left, i, op); }
    else { return Os_Select(node->right, i - rank, op); }


}

node *Os_Delete(node *nodet, int i, Operation op1) {
    if (nodet == nullptr) { return nullptr; }//element not found
    int rank;
    //check if there is a left child
    if (nodet->left != nullptr) { //if there is we set the rank to the size of the left subtree plus 1
        rank = nodet->left->size + 1;
        op1.count();
    } else { rank = 1; }

    if (i < rank) { nodet->left = Os_Delete(nodet->left, i, op1); }
    else if (i > rank) { nodet->right = Os_Delete(nodet->right, i - rank, op1); }
    else {
        //case 1: no child
        if (nodet->left == nullptr && nodet->right == nullptr) {
            free(nodet);
            return nullptr;
        }
        //case 2: one child
        if (nodet->left == nullptr) {
            node *aux = nodet->right;//if we don't have a left child, we replace the current  node with its right child
            free(nodet);//free memory
            return aux;
        } else if (nodet->right == nullptr) {
            node *aux = nodet->left;//if we don't have a right child, we replace the current node with its left child
            free(nodet);
            return aux;
        }
        //case 3: two children
        node *successorParent = nodet;
        node *successor = nodet->right;
        while (successor->left != nullptr) {
            successorParent = successor;
            successor = successor->left;
        }

        if(successorParent!=nodet)
        {successorParent->left =successor->right;
         }
        else
            {successorParent->right = successor->right;//in case that the node we want to delete is also the successor p
            }
        nodet->value = successor->value;
        updatesize(successorParent,op1);
    }
    updatesize(nodet,op1);
    return nodet;
}


void prettyprint3(int spaces, node *node) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < spaces; i++) {
        cout << " ";
    }
    cout << node->value <<" "<<node->size<< endl;


    prettyprint3(spaces + 4, node->left);
    prettyprint3(spaces + 4, node->right);
}

void perf() {

    srand(time(0));
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation op = prof.createOperation("Select", n);
        Operation op1 = prof.createOperation("Delete", n);

        for (int expe = 0; expe < NR_TESTS; expe++) {
            node *root = Build_Tree(n);
            for(int j=0;j<n;j++)
            {int i = rand() % n + 1;
            Os_Select(root, i, op);
            Os_Delete(root, i, op1);}
        }
    }
    prof.divideValues("Select", NR_TESTS);
    prof.divideValues("Delete", NR_TESTS);
    prof.createGroup("All","Select","Delete");

    prof.showReport();
}

void demo() {
    int n = 11;
    Operation op = prof.createOperation("Select", n);
    Operation op1 = prof.createOperation("Delete", n);
    node *root = Build_Tree(n);
    prettyprint3(0, root);

    //select
    srand(time(0));
    int i = rand() % n + 1;
    cout<<"Search for:"<<i<<"th smallest element\n";
    node *res = Os_Select(root, i, op);
    if (res != nullptr) { cout << "The " << i << "th smalest element is " << res->value; }
    else { cout << "not found"; }
    cout << "\n";
    //delete 1 number
    Os_Delete(root, i, op);
    int j = rand() % n + 1;
    cout<<"Search for:"<<j<<"th smallest element\n";
    node *res2 = Os_Select(root, j, op);
    if (res2 != nullptr) { cout << "The " << j << "th smalest element is " << res2->value; }
    else { cout << "not found"; }
    cout << "\n";
    //delete second number
    Os_Delete(root, j, op);

    int m = rand() % n + 1;
    cout<<"Search for "<<m<<"th smallest element\n";
    node *res3 = Os_Select(root, m, op);
    if (res3 != nullptr) { cout << "The " << m << "th smalest element is " << res3->value; }
    else { cout << "not found"; }
    cout << "\n";
    //delete third
    Os_Delete(root, m, op1);
    prettyprint3(0, root);
}


int main() {
    demo();
    //perf();
}
