
//  Heapsort  is a sorting algorithm the sorts a heap in ascending order. We use this by making a max heap with the
//help of the bottom up method. And then we start the sorting which is realised by swapping the root element with the
//last of the heap.With this method we repeatedly move the biggest element to the end of the heap. It has the time complexity
//of O(n log n).We have log n because we use a recursive method and because we separate the whole tree into little subtrees.We
//to do this for each and one node so that is from where n comes.In the best case we make sure we have an array with al equal
//numbers. For worst we will work with an already sorted array.

//   Quicksort is a divide and conquer algorithm because it has the 3 steps: divide- the use of a partition function that separates
//the array in 2 parts , conquer-sorting the two subarrays by recursive calls of the quicksort function, combine- the two subarrays
// are sorted so we have a whole sorted array. Fot he best case we will have another function of partitioning that choses the pivot
//from the middle because it makes sure we have two equal arrays to sort.In worst case we have an already sorted array and the complexity
// will be O(n^2).Quicksort has the time complexity of O(n log n).
//
//   Quickselect is an algorithm used to find the k-th smallest element of the array.We do this by partitoning and using a pivot.
//Time complexity is O(n).
//
// From the graphs we see that in the average case quicksort is the best because it divides the array in subarray making it more
//easy to sort.








#include <iostream>
#include <ctime>
#include<cstdlib>
#include "Profiler.h"

Profiler prof("Best");

#define MAX_SIZE 10000
#define STEP_SIZE  100
#define NR_TESTS 5

using namespace std;


int PARTITION(int a[], int minim, int maxim, Operation comp_quick, Operation assign_quick) {
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

int PARTITIONQ(int a[], int minim, int maxim) {
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
void QUICKSORT(int a[], int size, int minim, int maxim) {
    Operation comp_quick = prof.createOperation("Quicksort Comparation", size);
    Operation assign_quick = prof.createOperation("Quicksort Assignments", size);
    if (minim < maxim) {
        int q = PARTITION(a, minim, maxim, comp_quick, assign_quick);
        QUICKSORT(a, size, minim, q - 1);
        QUICKSORT(a, size, q + 1, maxim);

    }
}


int BESTPARTITION(int a[], int minim, int maxim, Operation comp_quick, Operation assign_quick) {
    int mid = (maxim + minim) / 2;
    int p=maxim;
    swap(a[p],a[mid]);
    int i = minim - 1;
    for (int j = minim; j <= maxim - 1; j++) {
        comp_quick.count();
        if (a[j] <= a[p]) {
            i++;
            swap(a[i], a[j]);
            assign_quick.count(3);

        }
    }
    assign_quick.count(3);
    swap(a[i + 1], a[maxim]);
    return i +1;


}


void BESTQUICKSORT(int a[], int size, int minim, int maxim) {
    Operation comp_quick = prof.createOperation("Quicksort Comparation", size);
    Operation assign_quick = prof.createOperation("Quicksort Assignments", size);
    if (minim < maxim) {
        int q = BESTPARTITION(a, minim, maxim, comp_quick, assign_quick);
        BESTQUICKSORT(a, size, minim, q - 1);
        BESTQUICKSORT(a, size, q + 1, maxim);

    }
}
int random(int a,int b)
{
  return a+rand()%(b-a+1);

}

int PARTITIONQUICK(int a[], int minim, int maxim) {
    int p =random (minim,maxim);

            swap(a[maxim], a[p]);

            return PARTITIONQ(a,minim,maxim);

}

int QUICKSELECT(int a[], int left, int right, int q) {
    if (right == left) {
        return a[left];

    }
    int i = PARTITIONQUICK(a, left, right);
    int k=i-left+1;
    if (k == q)
        return a[i];
    else if (q< k) {
        return QUICKSELECT(a, left, i- 1, q);
    } else {
        return QUICKSELECT(a, i + 1, right,q- k);
    }


}


void MAX_HEAPIFY(int a[], int i, int size, Operation comp_heapsort, Operation assign_heapsort) {


    int maxim = i;
    int left = 2 * i + 1;//usually left=2*i and right=2*i+1 but because our array is indexed from 0 we must add 1
    int right = 2 * i + 2;

    if (left < size) {

        comp_heapsort.count();
        if (a[left] > a[i]) {
            maxim = left;
        }
    }


    if (right < size) {
        comp_heapsort.count();
        if (a[right] > a[maxim]) {
            maxim = right;

        }
    }
    if (maxim != i) {
        swap(a[i], a[maxim]);
        assign_heapsort.count(3);
        MAX_HEAPIFY(a, maxim, size, comp_heapsort, assign_heapsort);
    }

}


void BUILD_BOTTOM_UP(int a[], int size, Operation comp_heapsort, Operation assign_heapsort) {

    for (int i = size / 2 - 1; i >=
                               0; i--)//start from size-2+1 because we must compare starting from the last non-leaf node,more efficient this way
    {

        MAX_HEAPIFY(a, i, size, comp_heapsort, assign_heapsort);
    }

}

void HEAPSORT(int a[], int size) {
    Operation comp_heapsort = prof.createOperation("Heapsort Comparation", size);
    Operation assign_heapsort = prof.createOperation("Heapsort Assignments", size);
    BUILD_BOTTOM_UP(a, size, comp_heapsort, assign_heapsort);
    for (int i = size - 1; i >= 0; i--) //reduce the size because we already sorted the last element
    {
        assign_heapsort.count(3);
        swap(a[0], a[i]);
        MAX_HEAPIFY(a, 0, i, comp_heapsort, assign_heapsort);
    }

}

void PRINT(int a[], int size) {
    for (int i = 0; i < size; i++)

        cout << a[i] << " ";

}

void demo() {

    int a[] = {2, 1, 5, 3, 7, 1, 9, 4};
    int copy1[MAX_SIZE], copy2[MAX_SIZE];
    int size = 8;
    cout << "The array at the start is: ";
    PRINT(a, size);
    CopyArray(copy1, a, size);
    CopyArray(copy2, a, size);
    cout << "\nThe array after quicksort is: ";
   QUICKSORT(a, size, 0, size -1);
    PRINT(a, size);
    cout << "\nThe array after heapsort is: ";
    HEAPSORT(copy1, size);
    PRINT(copy1, size);


}

void demoquick() {
    int a[] = {2, 1, 5, 3, 7, 1,1, 9, 4};
    int size = 8;
    int q = 4;
    if (q <= size - 1)//make sure that the q-th number isn't bigger than the size of array

    {
        int r = QUICKSELECT(a, 0, size - 1, q );
        cout << "The " << q << " smallest element is: " << r;
    } else cout << "Impossible";


}

enum CASE {
    AVERAGE, WORST, BEST
};


void perf(CASE cases) {
    int a[MAX_SIZE];
    if (cases == BEST) {

        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {

            //for Heapsort
            FillRandomArray(a, size, 10, 5000, false, ASCENDING);
            for (int i = 0; i < size; i++) {
                a[i] = 1;

            }

            HEAPSORT(a, size);

            //for Quicksort

            FillRandomArray(a, size, 10, 50000, false, ASCENDING);
            BESTQUICKSORT(a, size, 0, size - 1);


        }


    } else if (cases == WORST) {


        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            //for Heapsort
            FillRandomArray(a, size, 10, 50000, false, ASCENDING);
            HEAPSORT(a, size);

            //for Quicksort
            FillRandomArray(a, size, 10, 50000, false, ASCENDING);
            QUICKSORT(a, size, 0, size - 1);


        }


    } else if (cases == AVERAGE) {
        for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
            for (int expe = 0; expe < NR_TESTS; expe++) {
                int copy1[MAX_SIZE];
                FillRandomArray(a, size, 10, 50000, false, UNSORTED);
                CopyArray(copy1, a, size);
                //for heapsort
                HEAPSORT(a, size);

                //for quicksort
                QUICKSORT(copy1, size, 0, size - 1);


            }


        }
        prof.divideValues("Quicksort Comparation", NR_TESTS);
        prof.divideValues("Quicksort Assignments", NR_TESTS);
        prof.divideValues("Heapsort Comparation", NR_TESTS);
        prof.divideValues("Heapsort Assignments", NR_TESTS);


    }
    prof.addSeries("Quicksort", "Quicksort Comparation", "Quicksort Assignments");
    prof.addSeries("Heapsort", "Heapsort Comparation", "Heapsort Assignments");

    prof.createGroup("Assignments", "Quicksort Assignments", "Heapsort Assignments");
    prof.createGroup("Comparing", "Quicksort Comparation", "Heapsort Comparation");
    prof.createGroup("All", "Quicksort", "Heapsort");

}


void perf_all() {
    perf(BEST);
    prof.reset("Worst");
    perf(WORST);
    prof.reset("Average");
    perf(AVERAGE);
    prof.showReport();

}

int main() {
    demo();
    //perf_all();
    demoquick();
}

