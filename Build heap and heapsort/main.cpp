
// Bottom up is an algorithm that creates a heap from an array of numbers. This is done by starting at the last non-leaf
//node in array and for each node we compare it with its children and swapping it with the largest. After that we move to
//the previous non-leaf node.We do this until we reach the top of the heap. The time complexity of this algorithm
//is O(n).

// Top down is an algorithm that creates a heap from an array of numbers. This is done by starting from the first
//child of the heap and comparing it with its parent. We see which one is greater and the one that is the biggest
//will become the parent. The time complexity is O(n log n) because for almost every node we might need
//to traverse the path from the node to the root of the heap.For this I used the function SWIM which is an
//iterative one, because this is the most efficient.

//Heapsort  is a sorting algorithm the sorts a heap in ascending order. We use this by making a max heap with the
//help of the bottom up method. And the we start the sorting which is realised by swapping the root element with the
//last of the heap.With this method we repeatedly move the biggest element to the end of the heap.


//From the graphs we notice that the most efficient algorithm for constructing a heap is the bottom up one, even for the
//worst and average case.

#include <iostream>
#include "Profiler.h"

Profiler prof("Worst");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
using namespace std;

void MAX_HEAPIFY(int a[], int i, int size, Operation comp_heapify, Operation assign_heapify) {


    int maxim = i;
    int left = 2 * i + 1;//usually left=2*i and right=2*i+1 but because our array is indexed from 0 we must add 1
    int right = 2 * i + 2;

    if (left < size) {

        comp_heapify.count();
        if (a[left] > a[i]) {
            maxim = left;
        }
    }


    if (right < size) {
        comp_heapify.count();
        if (a[right] > a[maxim]) {
            maxim = right;

        }
    }
    if (maxim != i) {
        swap(a[i], a[maxim]);
        assign_heapify.count(3);
        MAX_HEAPIFY(a, maxim, size, comp_heapify, assign_heapify);
    }

}


void BUILD_BOTTOM_UP(int a[], int size) {
    Operation comp_heapify = prof.createOperation("Bottom Up Comparation", size);
    Operation assign_heapify = prof.createOperation("Bottom Up Assignments", size);
    for (int i = size / 2 - 1; i >=
                               0; i--)//start from size-2+1 because we must compare starting from the last non-leaf node,more efficient this way
    {

        MAX_HEAPIFY(a, i, size, comp_heapify, assign_heapify);
    }

}

void SWIM(int a[], int i, int size, Operation comp_swim, Operation assign_swim) {

    int parent = (i - 1) / 2;
    bool ok = false;//i use this ok because when counting the comparation i need to to count it even if it doesn't get in while

    while (i > 0 && a[i] > a[parent]) {
        ok = true;
        comp_swim.count();
        assign_swim.count(3);
        swap(a[i], a[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
    if (ok == false) {
        comp_swim.count();

    }
}

void BUILD_TOP_DOWN(int a[], int size) {
    Operation comp_swim = prof.createOperation("Top Down Comparation", size);
    Operation assign_swim = prof.createOperation("Top Down Assignments", size);
    for (int i = 1; i < size; i++) { SWIM(a, i, size, comp_swim, assign_swim); }

}

void HEAPSORT(int a[], int size) {
  Operation comp_heapify = prof.createOperation("Bottom Up Comparation", size);
Operation assign_heapify = prof.createOperation("Bottom Up Assignments", size);
 BUILD_BOTTOM_UP(a, size);
 for (int i = size - 1; i >= 0; i--) //reduce the size because we already sorted the last element
 {swap(a[0], a[i]);
MAX_HEAPIFY(a, 0, i ,comp_heapify,assign_heapify);
}

}

void PRINT(int a[], int size) {
    for (int i = 0; i < size; i++)

        cout << a[i] << " ";

}

void demo() {
    int a[] = {1,5,2,1,2,7,2,4,3};
    int size = 9;
int copy[MAX_SIZE],copy2[MAX_SIZE];
    CopyArray(copy,a,size);
    CopyArray(copy2,a,size);
    cout<<"The array at start is:\n";
    PRINT(a,size);

    cout<<"\nThe array after bottom up is:\n";
    BUILD_BOTTOM_UP(a,size);
    PRINT(a,size);
    cout<<"\nThe array after top down is:\n";
    BUILD_TOP_DOWN(copy,size);
    PRINT(copy,size);

    cout<<"\nThe array after heapsort is:\n";
    HEAPSORT(copy2,size);
    PRINT(copy2,size);



}

enum CASE {
    AVERAGE, WORST
};

void perf(CASE cases) {
    int a[MAX_SIZE];
    if (cases == WORST) {
        //for Bottom up
        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            FillRandomArray(a, size, 10, 50000, false, ASCENDING);
            BUILD_BOTTOM_UP(a, size);

            //for Top Down
            FillRandomArray(a, size, 10, 50000, false, ASCENDING);
            BUILD_TOP_DOWN(a, size);


        }


    }
    else if (cases == AVERAGE) {
        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            for (int expe = 0; expe < NR_TESTS; expe++) {
                int copy1[MAX_SIZE];
                FillRandomArray(a, size, 10, 50000, false, UNSORTED);
                CopyArray(copy1, a, size);
                BUILD_BOTTOM_UP(a, size);
                BUILD_TOP_DOWN(copy1, size);
            }
        }

        prof.divideValues("Bottom Up Comparation", NR_TESTS);
        prof.divideValues("Bottom Up Assignments", NR_TESTS);
        prof.divideValues("Top Down Comparation", NR_TESTS);
        prof.divideValues("Top Down Assignments", NR_TESTS);
    }



    prof.addSeries("Bottom Up", "Bottom Up Comparation", "Bottom Up Assignments");
    prof.addSeries("Top Down", "Top Down Comparation", "Top Down Assignments");

    prof.createGroup("Assignments", "Bottom Up Assignments", "Top Down Assignments");
    prof.createGroup("Comparing", "Bottom Up Comparation", "Top Down Comparation");
    prof.createGroup("All", "Bottom Up", "Top Down");
}


void perf_all() {
    perf(WORST);
    prof.reset("Average");
    perf(AVERAGE);
    prof.showReport();

}

int main() {
    demo();
   // perf_all();
}
