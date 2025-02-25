// When we want to have a sorted list we need to think about a data structure in which we have mutiple numbers
//sorted in a descending array that are connected one by another by pointers.If we talk about k sorted lists we refer
//to a collection of sorted lists that are merged.This algorithm is done by having as input a random array that is
//put in an random way in k lists.After that we start the merging process which is done by creating a min heap of
//lists.We put the first element of the first list in the final list, then we pop it, delete it,then, if the list isn't
//empty, we heapify the tree, as to make sure that the root of this tree will have as the first element of list the minimum.
//By doing this we make sure that in the final solution we always put the minimum of all the lists.
// When we use the profiler for the 3 values of k and when n varies we see that the complexity is O(n) because
//we have constant k's and the time complexity depend primarily on the total numbers of elements.
//But when we have n constant and k varies we have the complexity of O(n log k) because we insert k lists in the heap.
//This will take O(log k).We also need to insert each minimum element from a list into the final list.Because we have
//n element this will mean that the complexity becomes O(n log k).





#include <iostream>
#include <list>
#include "Profiler.h"

#define MAX_SIZE 10000
#define MAX_K 500
#define STEP_SIZE 100
#define NR_TESTS 5

using namespace std;
Profiler prof("AVERAGE");

void MIN_HEAPIFY(list<int> heap[], int i, int N, Operation op) {
    int minim = i;
    int left = 2 * i + 1;//usually left=2*i and right=2*i+1 but because our array is indexed from 0 we must add 1
    int right = 2 * i + 2;

    if (left < N) {
        op.count();
        if (heap[left].front() < heap[i].front()) {
            minim = left;
        }
    }

    if (right < N) {
        op.count();
        if (heap[right].front() < heap[minim].front()) {
            minim = right;
        }
    }
    if (minim != i) {
        swap(heap[i], heap[minim]);
        op.count(3);
        MIN_HEAPIFY(heap, minim, N, op);
    }

}

void BUILD_BOTTOM_UP(list<int> heap[], int N, Operation op) {
    for (int i = N / 2 - 1; i >=
                            0; i--)//start from size-2+1 because we must compare starting from the last non-leaf node,more efficient this way
    {
        MIN_HEAPIFY(heap, i, N, op);
    }
}


void MERGE(list<int> heap[], int k, list<int> &sol, Operation op) {


    BUILD_BOTTOM_UP(heap, k, op);

    while (k > 0) {
        int el = heap[0].front();//first element form the first list which is the minimum
        sol.push_back(el);//push it into the final list
        heap[0].pop_front();//delete it
        if (heap[0].empty()) {//if the list is empty we swap it with the last and then minimize the size as to not work with it
            swap(heap[0], heap[k - 1]);
            op.count(3);
            k--;
        }
        MIN_HEAPIFY(heap, 0, k, op);//remake min heap to make sure first element of root is minimum
    }
}


void createlist(int a[], int N, int k, list<int> heap[]) {
    for (int i = 0; i < k; i++) {
        for (int j = i; j < N; j += k) {
            heap[i].push_back(a[j]);
        }
    }
}

void PRINTLIST(list<int> heap[], int k) {
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << " has these elements: ";
        list<int> aux = heap[i];
        while (!aux.empty()) {
            cout << aux.front() << " ";
            aux.pop_front();
        }
        cout << endl;
    }
}

void PRINTLIST2(const list<int> &sol) {
    list<int> aux = sol;
    cout << "Sorted list is:";
    while (!aux.empty()) {
        cout << aux.front() << " ";
        aux.pop_front();
    }
}

enum CASE {
    AVERAGE
};

void perf1(CASE cases) {
    int a[MAX_SIZE];

    if (cases == AVERAGE) {

        for (int k = STEP_SIZE; k <= MAX_K; k += STEP_SIZE) {
            Operation op = prof.createOperation("Opp", k);
            for (int expe = 0; expe < NR_TESTS; expe++) {

                FillRandomArray(a, MAX_SIZE, 1, 50000, false, ASCENDING);
                list<int> heap[k];
                list<int> sol;
                createlist(a, MAX_SIZE, k, heap);
                MERGE(heap, k, sol, op);
            }
        }
        prof.divideValues("Opp", NR_TESTS);
        prof.showReport();
    }

}

void perf2(CASE cases) {
    int k1 = 5;
    int k2 = 10;
    int k3 = 100;
    int a[MAX_SIZE];
    if (cases == AVERAGE) {
        for (int N = STEP_SIZE; N <= MAX_SIZE; N += STEP_SIZE) {

            Operation op = prof.createOperation("Opp1", N);
            for (int expe = 0; expe < NR_TESTS; expe++) {

                FillRandomArray(a, N, 1, 50000, false, ASCENDING);
                list<int> heap[k1];
                list<int> sol;
                createlist(a, N, k1, heap);
                MERGE(heap, k1, sol, op);
            }
        }
        for (int N = STEP_SIZE; N <= MAX_SIZE; N += STEP_SIZE) {

            Operation op = prof.createOperation("Opp2", N);
            for (int expe = 0; expe < NR_TESTS; expe++) {

                FillRandomArray(a, N, 1, 50000, false, ASCENDING);
                list<int> heap[k2];
                list<int> sol;
                createlist(a, N, k2, heap);
                MERGE(heap, k2, sol, op);
            }
        }
        for (int N = STEP_SIZE; N <= MAX_SIZE; N += STEP_SIZE) {

            Operation op = prof.createOperation("Opp3", N);
            for (int expe = 0; expe < NR_TESTS; expe++) {

                FillRandomArray(a, N, 1, 50000, false, ASCENDING);
                list<int> heap[k3];
                list<int> sol;
                createlist(a, N, k3, heap);
                MERGE(heap, k3, sol, op);
            }
        }
        prof.divideValues("Opp1", NR_TESTS);
        prof.divideValues("Opp2", NR_TESTS);
        prof.divideValues("Opp3", NR_TESTS);
        prof.createGroup("ALL", "Opp1", "Opp2", "Opp3");

    }

}

int demo() {

    int N = 20;
    int a[N];
    FillRandomArray(a, N, 1, 50, false, ASCENDING);
    int k = 6;
    Operation op = prof.createOperation("OPP", k);
    list<int> heap[k];
    list<int> sol;
    createlist(a, N, k, heap);
    PRINTLIST(heap, k);
    MERGE(heap, k, sol, op);

    PRINTLIST2(sol);

}


int main() {

    demo();

    //perf1(AVERAGE);
    //prof.showReport();
    //perf2(AVERAGE);
    //prof.showReport();

}
