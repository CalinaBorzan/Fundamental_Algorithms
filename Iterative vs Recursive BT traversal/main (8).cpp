
//   Quicksort is a divide and conquer algorithm because it has the 3 steps: divide- the use of a partition function that separates
//the array in 2 parts , conquer-sorting the two subarrays by recursive calls of the quicksort function, combine- the two subarrays
// are sorted, so we have a whole sorted array. Fot he best case we will have another function of partitioning that choses the pivot
//from the middle because it makes sure we have two equal arrays to sort.In worst case we have an already sorted array and the complexity
// will be O(n^2).Quicksort has the time complexity of O(n log n).
//I did the in order traversal using Morris Traversal which has the time complexity of O(n).


#include <iostream>
#include "Profiler.h"


#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 100

using namespace std;
Profiler prof("Recursive vs Iterative");

typedef struct node {
    node *left;
    node *right;
    int value;
};


node *insert(node *root, int value) {
    if (root == nullptr) {
        root = new node;
        root->value = value;
        root->left = root->right = nullptr;
    } else {
        if (root->left == nullptr) { root->left = insert(root->left, value); }
        else if (root->right == nullptr) {
            root->right = insert(root->right, value);

        } else if (rand() % 2 == 0) {
            root->left = insert(root->left, value);
        } else {
            root->right = insert(root->right, value);
        }
    }
    return root;

}

node *build(int a[], int size) {
    srand(time(0));
    int i = rand() % size;//choose a random root
    node *root = new node;
    root->value = a[i];
    root->left = root->right = nullptr;
    node *curr = root;
    for (int j = 0; j < i; j++) {
        curr = insert(curr, a[j]);
    }

    for (int j = i + 1; j < size; j++) {
        curr = insert(curr, a[j]);
    }

    return root;
}

void printInOrder(node *root, Operation op) {
    if (root == nullptr) return;
    printInOrder(root->left, op);
    cout << root->value << " ";
    op.count();
    printInOrder(root->right, op);

}

void prettyprint3(int spaces, node *node, char c) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < spaces; i++) {
        cout << " ";
    }
    cout << node->value << c << " " << endl;


    prettyprint3(spaces + 4, node->left, 'l');
    prettyprint3(spaces + 4, node->right, 'r');
}
void printInOrderIterative(node *root, Operation op1) {
    node *current = root;

    // Continue as long as there is a node to process
    while (current != nullptr) {

        // If there is no left child, print the current node
        // and move to the right child
        if (current->left == nullptr) {
            cout << current->value << " ";
            op1.count();  // Counting the operation
            current = current->right;
        } else {
            // Find the rightmost node in the left subtree or
            // the inorder predecessor of current
            node *pre = current->left;
            while (pre->right != nullptr && pre->right != current) {
                pre = pre->right;
            }

            // Making current as the right child of its inorder predecessor
            if (pre->right == nullptr) {
                pre->right = current;
                current = current->left;
            } else {
                // Reverting the changes made in the 'if' part to restore the original
                // tree, i.e., fixing the right child of predecessor
                pre->right = nullptr;
                cout << current->value << " ";
                op1.count();  // Counting the operation
                current = current->right;
            }
        }
    }
}


int PARTITION(int a[], int minim, int maxim, Operation comp_quick, Operation assign_quick) {
    assign_quick.count();
    int p = a[maxim];
    int i = minim - 1;
    for (int j = minim; j <= maxim - 1; j++) {
        comp_quick.count();
        if (a[j] <= p) {
            i++;
            swap(a[i], a[j]);
            assign_quick.count(3);

        }
    }
    assign_quick.count(3);
    swap(a[i + 1], a[maxim]);

    return i + 1;
}

void QUICKSORT(int a[], int minim, int maxim, Operation comp_quick, Operation assign_quick) {

    if (minim < maxim) {
        int q = PARTITION(a, minim, maxim, comp_quick, assign_quick);
        QUICKSORT(a, minim, q - 1, comp_quick, assign_quick);
        QUICKSORT(a, q + 1, maxim, comp_quick, assign_quick);

    }
}

void Insertion_sort(int a[], int n, int minim, Operation comp_quick_hybrid, Operation assign_quick_hybrid) {


    for (int i = minim; i <= n; i++)  //we start from 2 because the first number is already ordered
    {
        int buff = a[i];
        assign_quick_hybrid.count();
        int j = i - 1;
        comp_quick_hybrid.count();
        while (j >= 0 && buff < a[j]) {
            a[j + 1] = a[j];
            assign_quick_hybrid.count();
            j--;
        }
        a[j + 1] = buff;
        assign_quick_hybrid.count();

    }
}

void HYBRID_QUICKSORT(int a[], int minim, int maxim, int threshold, Operation comp_quick_hybrid,
                      Operation assign_quick_hybrid) {
    if (minim < maxim) {
        if (maxim - minim + 1 < threshold) {
            Insertion_sort(a, maxim, minim, comp_quick_hybrid, assign_quick_hybrid);

        } else {
            int q = PARTITION(a, minim, maxim, comp_quick_hybrid, assign_quick_hybrid);
            HYBRID_QUICKSORT(a, minim, q - 1, threshold, comp_quick_hybrid, assign_quick_hybrid);
            HYBRID_QUICKSORT(a, q + 1, maxim, threshold, comp_quick_hybrid, assign_quick_hybrid);
        }

    }
}

void Insertion_sort_count(int a[], int n, int minim) {


    for (int i = minim; i <= n; i++)  //we start from 2 because the first number is already ordered
    {
        int buff = a[i];

        int j = i - 1;

        while (j >= 0 && buff < a[j]) {
            a[j + 1] = a[j];

            j--;
        }
        a[j + 1] = buff;


    }
}

int PARTITION_COUNT(int a[], int minim, int maxim) {

    int p = a[maxim];
    int i = minim - 1;
    for (int j = minim; j <= maxim - 1; j++) {

        if (a[j] <= p) {
            i++;
            swap(a[i], a[j]);


        }
    }
    swap(a[i + 1], a[maxim]);
    return i + 1;
}

void HYBRID_QUICKSORT_COUNT(int a[], int minim, int maxim) {
    if (minim < maxim) {
        if (maxim - minim + 1 < 30) {
            Insertion_sort_count(a, maxim, minim);

        } else {
            int q = PARTITION_COUNT(a, minim, maxim);
            HYBRID_QUICKSORT_COUNT(a, minim, q - 1);
            HYBRID_QUICKSORT_COUNT(a, q + 1, maxim);
        }

    }
}

void HYBRID_QUICKSORT_Thresh(int a[], int minim, int maxim, int threshold) {
    if (minim < maxim) {
        if (maxim - minim + 1 < threshold) {
            Insertion_sort_count(a, maxim, minim);

        } else {
            int q = PARTITION_COUNT(a, minim, maxim);
            HYBRID_QUICKSORT_COUNT(a, minim, q - 1);
            HYBRID_QUICKSORT_COUNT(a, q + 1, maxim);
        }

    }
}

void QUICKSORT_COUNT(int a[], int minim, int maxim) {

    if (minim < maxim) {
        int q = PARTITION_COUNT(a, minim, maxim);
        QUICKSORT_COUNT(a, minim, q - 1);
        QUICKSORT_COUNT(a, q + 1, maxim);

    }
}

void perf_quick() {
    int a[MAX_SIZE] = {0};
    int aux[MAX_SIZE] = {0};
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation comp_quick = prof.createOperation("Quicksort Comparation", n);
        Operation assign_quick = prof.createOperation("Quicksort Assignments", n);

        Operation comp_quick_hybrid = prof.createOperation("Quicksort Hybrid Comparation", n);
        Operation assign_quick_hybrid = prof.createOperation("Quicksort Hybrid Assignments", n);
        for (int expe = 0; expe < NR_TESTS; expe++) {
            FillRandomArray(a, n, 100, 50000, false, UNSORTED);
            CopyArray(aux, a, n);
            QUICKSORT(a, 0, n - 1, comp_quick, assign_quick);
            HYBRID_QUICKSORT(aux, 0, n - 1, 30, comp_quick_hybrid, assign_quick_hybrid);
        }

    }
    prof.divideValues("Quicksort Comparation", NR_TESTS);
    prof.divideValues("Quicksort Assignments", NR_TESTS);
    prof.divideValues("Quicksort Hybrid Comparation", NR_TESTS);
    prof.divideValues("Quicksort Hybrid Assignments", NR_TESTS);

    prof.addSeries("Quicksort", "Quicksort Comparation", "Quicksort Assignments");
    prof.addSeries("Quicksort Hybrid", "Quicksort Hybrid Comparation", "Quicksort Hybrid Assignments");
    prof.createGroup("All", "Quicksort", "Quicksort Hybrid");
    prof.showReport();

}

void perf() {
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation op = prof.createOperation("In Order Recursive", n);
        Operation op1 = prof.createOperation("In Order Iterative", n);
        int a[MAX_SIZE];
        FillRandomArray(a, n, 100, 50000, false, UNSORTED);
        int i = rand() % n + 1;
        node *root = new node;
        root->value = a[i];
        root = build(a, n);
        for (int expe = 0; expe < NR_TESTS; expe++) {
            printInOrder(root, op);
            printInOrderIterative(root, op1);
        }
    }
    prof.divideValues("In Order Recursive", NR_TESTS);
    prof.divideValues("In Order Iterative", NR_TESTS);
    prof.showReport();
}

void perf_threshold() {
    int a[MAX_SIZE] = {0};
    int aux[MAX_SIZE] = {0};
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int expe = 0; expe < NR_TESTS; expe++) {
            FillRandomArray(a, n, 100, 50000, false, UNSORTED);
            CopyArray(aux, a, n);  // Copy the original array to aux
            for (int j = 1; j <= 50; ++j) {
                Operation comp_quick_hybrid = prof.createOperation("Quicksort Hybrid Comparation Treshold", j);
                Operation assign_quick_hybrid = prof.createOperation("Quicksort Hybrid Assignments Treshold", j);
                HYBRID_QUICKSORT(aux, 0, n - 1, j, comp_quick_hybrid, assign_quick_hybrid);
                CopyArray(aux, a, n);  // Restore aux to the original array
            }
        }
    }

    prof.divideValues("Quicksort Hybrid Comparation Treshold", NR_TESTS);
    prof.divideValues("Quicksort Hybrid Assignments Treshold", NR_TESTS);
    prof.addSeries("Quicksort with threshold", "Quicksort Hybrid Comparation Treshold",
                   "Quicksort Hybrid Assignments Treshold");
    prof.showReport();
}

void tempoThresh() {
    int a[MAX_SIZE] = {0};
    int aux[MAX_SIZE] = {0};
    for (int i = STEP_SIZE; i <= MAX_SIZE; i += STEP_SIZE) {
        FillRandomArray(a, i, 10, 50000, false, UNSORTED);
        CopyArray(aux, a, i);  // Copy the original array to aux
        for (int j = 1; j <= 50; j++) {
            prof.startTimer("Hybridization Quick Tempo", j);
            for (int test = 0; test < NR_TESTS; ++test) {
                HYBRID_QUICKSORT_Thresh(aux, 0, i - 1, j);
                CopyArray(aux, a, i);  // Restore aux to the original array
            }
            prof.stopTimer("Hybridization Quick Tempo", j);
        }
    }
    prof.showReport();
}

void tempoHybrid() {
    int a[MAX_SIZE] = {0};
    for (int i = STEP_SIZE; i <= MAX_SIZE; i += STEP_SIZE) {

        prof.startTimer("Hybridization Quick Tempo", i);
        for (int test = 0; test < NR_TESTS; ++test) {
            FillRandomArray(a, i, 10, 50000, false, UNSORTED);
            HYBRID_QUICKSORT_COUNT(a, 0, i - 1);
        }
        prof.stopTimer("Hybridization Quick Tempo", i);
    }

    int a2[MAX_SIZE] = {0};

    for (int i = STEP_SIZE; i <= MAX_SIZE; i += STEP_SIZE) {

        prof.startTimer("Quick Tempo", i);
        for (int test = 0; test < NR_TESTS; ++test) {
            FillRandomArray(a2, i, 10, 50000, false, UNSORTED);
            QUICKSORT_COUNT(a2, 0, i - 1);
        }
        prof.stopTimer("Quick Tempo", i);
    }
    prof.createGroup("All Time", "Hybridization Quick Tempo", "Quick Tempo");
    prof.showReport();
}

void demo() {
    int a[] = {1, 30, 60, 24, 5, 8, 6, 12};
    int size = sizeof(a) / sizeof(a[0]);
    Operation op = prof.createOperation("In Order Recursive", size);
    Operation op1 = prof.createOperation("In Order Iterative", size);
    srand(time(0));
    node *root = build(a, size);
    printInOrder(root, op);
    cout << "\n";
    printInOrderIterative(root, op1);
    cout << "\n";
    char c = 'c';
    prettyprint3(0, root, c);

}

void demo_quick() {
    int a[] = {24, 97, 40, 67, 88, 85, 15, 66, 53, 44, 26, 48, 16, 52, 45, 23, 90, 18, 49, 80};
    int size = sizeof(a) / sizeof(a[0]);
    Operation op = prof.createOperation("In Order Recursive", size);
    Operation op1 = prof.createOperation("In Order Iterative", size);
    HYBRID_QUICKSORT(a, 0, size - 1, 10, op, op);

    for (int i = 0; i < size; i++) { cout << a[i] << " "; }

}

int main() {
//     demo();
// perf();
//    demo_quick();
//    prof.reset("Quick");
//    perf_quick();
//    prof.reset("Tempo Hybrid vs Normal");
//    tempoHybrid();
//    prof.reset("Threshold Optimization");
//    perf_threshold();
//    prof.reset("Threshold Time");
//    tempoThresh();

}