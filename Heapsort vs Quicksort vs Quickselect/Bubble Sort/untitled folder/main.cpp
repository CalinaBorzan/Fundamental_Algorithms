//  The bubble sort iterative works by comparing adjacent elements and sorting them if they are in wrong order.The pass
//is repeated until there are no more swaps to be performed. In the best case scenario we work with a sorted array
//and time complexity is O(n) and in worst case we have a descending array and time complexity will be O(n^2).

// The bubble sort recursive works by comparing two adjacent elements, swapping them, then making the size smaller
//with one to make sure we don't usr the last number already the biggest and the calling the function again.

// In the average case we see that the Bubble Iterative is with very little more efficient than the Recursive one.




#include <iostream>
#include "Profiler.h"


#define MAX_SIZE 10000
#define STEP_SIZE  100
#define NR_TESTS 100

Profiler prof("Average");

using namespace std;

void Iterative_Bubble_sort(int a[], int n) {

    //Operation comp_bubble_it = prof.createOperation("Comparation Bubbleit", n);
    //Operation assign_bubble_it = prof.createOperation("Assign Bubbleit", n);
    int k;//not;
    int i;
    do {
        k = 1;//yes
        for (i = 0; i < n - 1; i++) {
            //comp_bubble_it.count();
            if (a[i + 1] < a[i]) {
                k = 0;//no
                //assign_bubble_it.count(3);
                swap(a[i], a[i + 1]);
            }
        }
        n--;//punem maximul la final,deci nu mai umblam la finalul vectorului

    } while (!k);

}

void Recursive_Bubble_sort(int a[], int n,Operation comp_bubble_re, Operation assign_bubble_re)
{
    if (n == 1)
       return;

    for (int i = 0; i < n - 1; i++) {
        //comp_bubble_re.count();
        if (a[i] > a[i + 1]) {
            swap(a[i], a[i + 1]);
            //assign_bubble_re.count(3);
        }
    }
    n--;
    Recursive_Bubble_sort(a,n,comp_bubble_re,assign_bubble_re);


}

void Recursive_Bubble_sort_time(int a[], int n)
{
    if (n == 1)
        return;

    for (int i = 0; i < n - 1; i++) {
        //comp_bubble_re.count();
        if (a[i] > a[i + 1]) {
            swap(a[i], a[i + 1]);
            //assign_bubble_re.count(3);
        }
    }
    n--;
    Recursive_Bubble_sort_time(a,n);


}
void COUNTOP_Recursive_Bubble_sort(int a[], int n) {

    Operation comp_bubble_re = prof.createOperation("Comparation BubbleRe", n);
    Operation assign_bubble_re = prof.createOperation("Assign BubbleRe", n);
    Recursive_Bubble_sort(a,n,comp_bubble_re,assign_bubble_re);


}


void PRINT(int a[], int size) {

    for (int i = 0; i < size; i++)
        cout << a[i] << " ";
}


enum CASE {
    AVERAGE
};

void perf(CASE cases) {

    int a[MAX_SIZE];
    if (cases == AVERAGE) {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            for (int expe = 0; expe <NR_TESTS; expe++) {
                int copy1[MAX_SIZE];

                //insertion
                FillRandomArray(a, n, 10, 50000, false, UNSORTED);
                CopyArray(copy1, a, n);
                //bubble sort iterative
                Iterative_Bubble_sort(a, n);
                //bubble sort recursive
                COUNTOP_Recursive_Bubble_sort(copy1, n);
            }
        }
        prof.divideValues("Comparation Bubbleit", NR_TESTS);
        prof.divideValues("Assign Bubbleit", NR_TESTS);
        prof.divideValues("Comparation BubbleRe", NR_TESTS);
        prof.divideValues("Assign BubbleRe", NR_TESTS);
    }


   prof.addSeries("Bubble Recursive", "Comparation BubbleRe", "Assign BubbleRe");
    prof.addSeries("Bubble Iterative", "Comparation Bubbleit", "Assign Bubbleit");


   prof.createGroup("Assignments", "Assign BubbleRe", "Assign Bubbleit");
    prof.createGroup("Comparations", "Comparation BubbleRe", "Comparation Bubbleit");
    prof.createGroup("All", "Bubble Recursive", "Bubble Iterative");

    prof.showReport();
}


void tempoRecBubble()
{int size=10000;
    int a[MAX_SIZE];
    FillRandomArray(a,size,10,50000,false,UNSORTED);
    for(int i=100;i<=10000;i+=STEP_SIZE) {

        prof.startTimer("Recursive_Bubble_sort_time", i);
        for (int test = 0; test < NR_TESTS; ++test) {
            Recursive_Bubble_sort_time(a, i);
        }
        prof.stopTimer("Recursive_Bubble_sort_time", i);
    }
    prof.showReport();
}

void tempoItBubble()
{int size=10000;
    int a[MAX_SIZE];
    FillRandomArray(a,size,10,50000,false,UNSORTED);
    for(int i=100;i<=10000;i+=STEP_SIZE) {

        prof.startTimer("Iterative_Bubble_sort", i);
        for (int test = 0; test < NR_TESTS; ++test) {
            Iterative_Bubble_sort(a,i);
        }
        prof.stopTimer("Iterative_Bubble_sort", i);
    }
    prof.showReport();
}

void demo() {
    int a[] = {2, 5, 1, 11, 23, 45, 23, 17, 90};
    int size = 9;
    int copy[MAX_SIZE];
    CopyArray(copy, a, size);
    cout << "This is the array at the start:";
    PRINT(a, size);
    cout << "\n This is the array after iterative bubble sort:";
    Iterative_Bubble_sort(a, size);
    PRINT(a, size);
    cout << "\n This is the array after recursive bubble sort:";
    Recursive_Bubble_sort_time(copy, size);
    PRINT(copy, size);

}



int main() {
   demo();
    //perf(AVERAGE);


  // prof.reset("Timp Iterativ");
    // tempoItBubble();
    //prof.reset("Timp Recursiv");
    //tempoRecBubble();
}
