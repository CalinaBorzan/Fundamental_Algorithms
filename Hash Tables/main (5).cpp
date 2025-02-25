/* A hash table is a data structure in which you can store data which can be found easily by a key.We usa a hash function
 * that makes sure to give each element a special place in the table based by its index. In this code we use open addressing
 * which is a collision technique where , when a collision occurs, the algorithm searches for the next available slot in
 * the hash table. For this I use quadratic probing in which the interval for the indexes is determined by successive quadratic
 * increments.Here if collision occurs at index h(k,i) the next attempt will be at h(k,i+1), with an increased quadratic
 * increment.For time complexity it usually tends to be 0(1) but it really depends on the load factor.If the load factor becomes
 * too high, as we see in perf, where we make the search algorithm in more than 1 cases, we observe that once the factor is
 * higher, the likelihood of collision increases, leading to more quadratic probes and,a degradation in performance.
 * Is important to use prime numbers in the quadratic hash function because they can help reduce the likelihood
 * of clustering.They tend to produce more random and irregular patterns.In this code we prove that when the fill factor gets bigger
 * the effort to find and even not to find numbers is bigger than when we have a fill factor of 0.8. We also can observe that
 * for searching deleted elements if we decrease the fill factor form 0.99 to 0.8  we get pretty much the same values as for when
 * we search number at 0.99 fil factor.
 *
 */




#include <iostream>
#include "Profiler.h"
#include <cstring>
#include <random>
#include <iomanip>
#include <set>

#define N 10007
#define NOT_FOUND -1
#define DELETED -2
using namespace std;


int hash_quadratic(int id, int i,int size) {
    int c1 = 3, c2 = 5;
    int index = (id + c1 * i + c2 * i * i) % size;

    return index;
}

//here we have the struct for the elements of tha hash table
struct Entry {
    int id;
    char name[30];
};
//here is the struct of the hashtable,in which we can be in control of its size and the number of elements
struct HashTable {
    int size;
    Entry T[N] = {NULL};//we work with an array of entries
    int count;

};

//in insert we have a pointer T that will point to the
int insert(HashTable &T, Entry e,int size) {
    int i = 0;//number of attempts made to insert the index in the table
    do {
        int index = hash_quadratic(e.id, i,size);
        if (T.T[index].id == 0) {//if the place in the hashtable is 0, empty slot
            T.T[index] = e;

            return index;
        }
        i++;
    } while (i < size);

    return NOT_FOUND;
}


int search(HashTable &T, int id,int size) {
    int i = 0;
    do {
        int index = hash_quadratic(id, i,size);
        if (T.T[index].id == 0) {
            return NOT_FOUND;
        }
        if (T.T[index].id == id) {

            return index;
        }
        i++;
    } while (i < size);

    return NOT_FOUND;  // Element not found
}

//we make a search 2 because for the averages we need to see how many times we need the number of how many times we accessed this function
//as to search the element which is given by the i
int search2(HashTable &T, int id,int size) {
    int i = 0;
    do {
        int index = hash_quadratic(id, i,size);
        if (T.T[index].id == 0) {
            return i;
        }
        if (T.T[index].id == id) {
            return i;
        }
        i++;
    } while (i < size);

    return i;

}

int deleted(HashTable &T, int id,int size) {
    int i = 0;
    int index;
    do {
        index = hash_quadratic(id, i,size);
        if (T.T[index].id == id) {//if the element has the same id as the one we want to delete
            T.T[index].id = DELETED;//we put deleted as in place of key
            memset(T.T[index].name, 0, sizeof(T.T[index].name));//we erase the name
            T.count--;//count of the elements in the hash table must be decremented
            return true;
        }
        i++;
    } while (T.T[index].id != 0 && i < size && T.T[index].id!=DELETED);//we do this until we reach the maximum and the elements isn't found anymore
    return NOT_FOUND;
}

void print_table(HashTable &T) {
    printf("\nHash Table\n-------------------\n");

    for (int i = 0; i < T.size; i++) {
        if (T.T[i].id >0) {
            printf("Index:%d, Key:%d, Name:%s\n", i, T.T[i].id, T.T[i].name);
        }
        else  if (T.T[i].id ==0) printf("Index:%d NULL\n", i);
        else if (T.T[i].id ==-2)printf("Index:%d DELETED\n", i);
    }

    printf("-------------------\n\n");
}


void demo() {
    HashTable hashTable;
    hashTable.size = 30;
    hashTable.count = 0;

    Entry e1 = {5, "ANDREI"};
    int index1 = insert(hashTable, e1,hashTable.size);

    Entry e2 = {6, "LALA"};
    int index2 = insert(hashTable, e2,hashTable.size);

    Entry e3 = {10, "MARIA"};
    int index3 = insert(hashTable, e3,hashTable.size);
    Entry e4 = {25, "CALINA"};
    int index4 = insert(hashTable, e4,hashTable.size);
    Entry e5 = {100, "GIGEL"};
    int index5 = insert(hashTable, e5,hashTable.size);
    Entry e6 = {88, "MARCEL"};
    int index6 = insert(hashTable, e6,hashTable.size);
    Entry e7 = {18, "MARICICA"};
    int index7 = insert(hashTable, e7,hashTable.size);
    Entry e8 = {75, "IONEL"};
    int index8 = insert(hashTable, e8,hashTable.size);

    print_table(hashTable);

    // Search for elements
    int idToSearch = 100;
    int index = search(hashTable, idToSearch,hashTable.size);

    if (index != NOT_FOUND) {
        cout << "Key " << idToSearch << " found at index " << index << endl;
    } else {
        cout << "Key " << idToSearch << " not found" << endl;
    }
    //deleted id
    int idtodelete = 18;
    deleted(hashTable, idtodelete,hashTable.size);
    print_table(hashTable);
    //search after delete

    idToSearch = 18;
  index = search(hashTable, idToSearch,hashTable.size);
    if (index != NOT_FOUND) {
        cout << "Key " << idToSearch << " found at index " << index << endl;
    } else {
        cout << "Key " << idToSearch << " not found" << endl;
    }


}

int randompartition(int partitionsize, int index, set<int>& chosenElements) {
    int start = index * partitionsize;
    int end = (index + 1) * partitionsize;
    while(true) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(start, end);
        int chosenElement = distribution(gen);
        if (chosenElements.insert(chosenElement).second && chosenElement!=0) {//check if the element is already chosen in this partition and
            //is not null
            return chosenElement;
        }

    }
    return 0;
}

float avgfound[7] = {0};
float avgnotfound[7] = {0};
float maxeffound[7] = {0};
float maxeffnot[7] = {0};

void fillingsearch(float fill, int index) {
    int nr = fill * N;//the number of elements to be inserted tinand cont de fill factor
    int partitions = 1500;//how many partitions i will have
    float partitionsize = N / partitions;//how many numbers are in a partition
    int a[10007];
    HashTable T;
    set<int> chosenElements;

    FillRandomArray(a, nr, 20000000, 499999999, true, UNSORTED);
    for (int i = 0; i < nr; i++) {
        Entry e;
        e.id = a[i];
        insert(T, e, N);//insert elements in the hash table
    }

    int v1[1501] = {0};
    int maxfound = -1;
    int k1 = -1;
    int sum2 = 0;
    //found
    for (int i = 0; i < partitions; i++) {

        int x = randompartition(partitionsize, i,chosenElements);
        if(T.T[x].id!=0)
        { k1++;
            int x2 = T.T[x].id;
            int n = search2(T, x2, N);
            v1[k1] = n + 1;//vector de frecventa, numaram accesarile ca sa cautam x2 in hash
             }
    }

    for (int j = 0; j < 1500; j++) {
        sum2 = sum2 + v1[j];
        if (v1[j] > maxfound) {
            maxfound = v1[j];
        }
    }
    avgfound[index] = sum2 + avgfound[index];
    maxeffound[index] = maxfound;


    //not found
    FillRandomArray(a, nr, 50000000, 100000000, true, UNSORTED);
    int sum = 0;
    int v[1501] = {0};
    int k = -1;
    int maxnot = -1;
    for (int i = 0; i < 1499; i++) {
        k++;

        int m = search2(T, a[i],N);
        v[k] = m + 1;//vector de frecventa

    }
    for (int j = 0; j < 1500; j++) {
        sum = sum + v[j];
        if (v[j] > maxnot) {
            maxnot = v[j];
        }
    }
    avgnotfound[index] = sum + avgnotfound[index];
    maxeffnot[index] = maxnot;


}


void fillingdelete(float fill, int index) {
    int nr = fill * N;
    int partitions = 1500;
    float numberpartitions= N / partitions;//see the number of elements that are in one partition
    int a[10007];
    HashTable T;
    T.size=N;
    T.count=1500;
    std::set<int> chosenElements;
    //inserting elements in the hash table
    FillRandomArray(a, nr, 2000000, 49999999, true, UNSORTED);
    for (int i = 0; i < nr; i++) {
        Entry e;
        e.id = a[i];
        insert(T, e, N);
    }
//with this we delete until we get at 0.8 fill factor
    while (static_cast<float>(T.count) / N > 0.8) {
        int idToDelete = randompartition(N, 0, chosenElements);  // Choose an ID to delete from the partitions
       deleted(T, idToDelete, N);
    }

    int v1[1501] = {0};
    int maxfound = -1;
    int k1 = -1;
    int sum2 = 0;

    //found
    for (int i = 0; i < partitions; i++) {

        int x = randompartition(numberpartitions, i,chosenElements);

        if(T.T[x].id!=DELETED)
        {  k1++;
            int x2 = T.T[x].id;
            int n = search2(T, x2,N);
            v1[k1] = n + 1;}
    }

    for (int j = 0; j < 1500; j++) {
        sum2 = sum2 + v1[j];
        if (v1[j] > maxfound) {
            maxfound = v1[j];
        }
    }
    avgfound[index] = sum2 + avgfound[index];
    maxeffound[index] = maxfound;


    //not found
    FillRandomArray(a, nr, 50000000, 100000000, true, UNSORTED);
    int sum = 0;
    int v[1501] = {0};
    int k = -1;
    int maxnot = -1;
    for (int i = 0; i < 1500; i++) {
        k++;

        int m = search2(T, a[i], N);
        v[k] = m + 1;//vector de frecventa

    }
    for (int j = 0; j < 1500; j++) {
        sum = sum + v[j];
        if (v[j] > maxnot) {
            maxnot = v[j];
        }
    }
    avgnotfound[index] = sum + avgnotfound[index];
    maxeffnot[index] = maxnot;

}


void perf_all() {
    const int numRows = 6;
    const int numCols = 4;

    int nrexp =10;
    float table[numRows][numCols] = {0};  // Initialize the table with zeros

    for (int i = 0; i < nrexp; i++) {
        int index = 0;
        float fill = 0.8;
        fillingsearch(fill, index);

        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];

        index++;
        fill = 0.85;
        fillingsearch(fill, index);

        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];

        index++;
        fill = 0.90;
        fillingsearch(fill, index);
        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];

        index++;
        fill = 0.95;
        fillingsearch(fill, index);
        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];

        index++;
        fill = 0.99;
        fillingsearch(fill, index);
        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];
        index++;

        //deletion
        float filldel = 0.99;
        fillingdelete(filldel, index);

        table[index][1] = avgnotfound[index] / (nrexp * 1500);
        table[index][0] = avgfound[index] / (nrexp * 1500);
        table[index][3] = maxeffnot[index];
        table[index][2] = maxeffound[index];
    }

    // Output the table
    cout << setw(15) << "Avg Found" << setw(15) << "Avg Not Found" << setw(20)
         << "Max Effort Found " << setw(20) << "Max Effort Not Found\n";

    // Output the table data
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            cout << setw(15) << table[i][j];
        }
        cout << "\n";
    }
}

int main() {
    //demo();
    perf_all();
}
