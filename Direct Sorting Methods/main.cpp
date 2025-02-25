
//   The insertion sort algorithm works by taking an element from the unsorted part of the array and comparing
//it with each number from the sorted part. After that we insert it in its correct position in that sorted part.
//In the best case scenario we go through the array only once, because the array is from the start sorted already.
//The time complexity is O(n) and it is linear as we can see from the graph. In the worst case for insertion
//we will work with a descending array, where the time complexity will be O(n^2).


//   The selection sort works by repeatedly finding the minimum element from the unsorted portion of array and
// placing it at the beginning of the sorted array. In the best case we work from the start with a sorted
//array but here we notice that il will have the same time complexity as the worst case, where the array
//is ascending and the first element is the biggest from the array, and to be exact O(n^2).

//  The bubble sort works by comparing adjacent elements and sorting them if they are in wrong order.The pass
//is repeated until there are no more swaps to be performed. In the best case scenario we work with a sorted array
//and time complexity is O(n) and in worst case we have a descending array and time complexity will be O(n^2).

// As to determine which of the tree sorting algorithms is the most efficient we study the graph in the average case
//where is pretty obvious that Insertion and Selection are pretty similar as efficiency, being better than bubble.
#include <iostream>
#include "Profiler.h"

Profiler prof("Best");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
using namespace std;


void Insertion_sort(int a[], int n) {

    Operation comp_insertion = prof.createOperation("Compare Insertion", n);
    Operation assign_insertion = prof.createOperation("Assign Insertion", n);
    bool ok = false;

    for (int i = 1; i < n; i++)  //we start from second number because the first number is already ordered
    {
        int buff = a[i];
        assign_insertion.count();
        int j = i - 1;
        ok = true;//we use ok as to make sure we count the number of comp ok
        while (j >= 0 && buff < a[j]) {
            ok = false;
            comp_insertion.count();
            a[j + 1] = a[j];
            assign_insertion.count();
            j--;
        }
        if(ok) {
            comp_insertion.count();
        }
        a[j + 1] = buff;
        assign_insertion.count();

    }
}

void Selection_sort(int a[], int n) {

    Operation comp_selection = prof.createOperation("Compare Selection", n);
    Operation assign_selection = prof.createOperation("Assign Selection", n);
    int i, j, minim;
    for (i = 0; i < n - 1; i++) {
        minim = i;//nu punem assign,lucram pe indici
        for (j = i + 1; j < n; j++) {
            comp_selection.count();
            if (a[j] < a[minim])
                minim = j;
        }
        if (i != minim) {
            assign_selection.count(3);
            swap(a[minim], a[i]);
        }
    }
}


void Bubble_sort(int a[], int n) {

    Operation comp_bubble = prof.createOperation("Compare Bubble", n);
    Operation assign_bubble = prof.createOperation("Assign Bubble", n);
    int k;//not;
    int i;
    do {
        k = 1;//yes it is sorted
        for (i = 0; i < n - 1; i++) {
            comp_bubble.count();
            if (a[i + 1] < a[i]) {
                k = 0;//no is not sorted
                assign_bubble.count(3);
                swap(a[i], a[i + 1]);
            }
        }
        n--;//punem maximul la final,deci nu mai umblam la finalul vectorului
    } while (!k);
}

void demo() {
    int a[] = {3, 1, 4, 7, 6, 9, 5};
    int n = 7;
    int copy[100];
    CopyArray(copy, a, n);
    Insertion_sort(copy, n);
    cout << "This is the new sorted array \n";
    for (int i = 0; i < n; i++)
        cout << " " << copy[i];
    CopyArray(copy, a, n);
    Selection_sort(copy, n);
    cout << "\nThis is the new sorted array\n ";
    for (int i = 0; i < n; i++)
        cout << " " << copy[i];
    CopyArray(copy, a, n);
    Bubble_sort(copy, n);
    cout << "\nThis is the new sorted array\n ";
    for (int i = 0; i < n; i++)
        cout << " " << copy[i];


}

enum CASE {
    Average, Best, Worst
};

void perf(CASE cases) {

    int a[MAX_SIZE];
    if (cases == Best) {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            //insertion
            FillRandomArray(a, n, 10, 50000, false, ASCENDING);
            Insertion_sort(a, n);

            //selection
            FillRandomArray(a, n, 10, 50000, false, ASCENDING);
            Selection_sort(a, n);

            //bubble
            FillRandomArray(a, n, 10, 50000, false, ASCENDING);
            Bubble_sort(a, n);
        }
    } else if (cases == Worst) {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            //insertion
            FillRandomArray(a, n, 10, 50000, false, DESCENDING);
            Insertion_sort(a, n);

            //selection
            FillRandomArray(a, n, 10, 49999, false, ASCENDING);
            a[0] = 500000;
            Selection_sort(a, n);

            //bubble
            FillRandomArray(a, n, 10, 50000, false, DESCENDING);
            Bubble_sort(a, n);
        }
    } else if (cases == Average) {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            for (int expe = 0; expe < NR_TESTS; expe++) {
                int copy1[MAX_SIZE], copy2[MAX_SIZE];
                //insertion
                FillRandomArray(a, n, 10, 50000, false, UNSORTED);
                CopyArray(copy1, a, n);
                CopyArray(copy2, a, n);
                Insertion_sort(a, n);
                //selection
                Selection_sort(copy1, n);
                //bubble
                Bubble_sort(copy2, n);
            }
        }
        prof.divideValues("Compare Insertion", NR_TESTS);
        prof.divideValues("Assign Insertion", NR_TESTS);
        prof.divideValues("Compare Selection", NR_TESTS);
        prof.divideValues("Assign Selection", NR_TESTS);
        prof.divideValues("Compare Bubble", NR_TESTS);
        prof.divideValues("Assign Bubble", NR_TESTS);
    }

    prof.addSeries("Insertion", "Compare Insertion", "Assign Insertion");
    prof.addSeries("Selection", "Compare Selection", "Assign Selection");
    prof.addSeries("Bubble", "Compare Bubble", "Assign Bubble");

   // prof.createGroup("Assignments", "Assign Insertion", "Assign Selection", "Assign Bubble");
   // prof.createGroup("Comparing", "Compare Insertion", "Compare Selection", "Compare Bubble");
   // prof.createGroup("All", "Insertion", "Selection", "Bubble");
}

void perf_all() {
    perf(Best);
    prof.reset("Worst");
    perf(Worst);
    prof.reset("Average");
    perf(Average);
    prof.showReport();
}

int main() {
    //demo();
    //perf();
    perf_all();
}

