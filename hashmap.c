#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//implement a hashtable
//collision avoidance
//linear chained

//int -> int
//hashmap cannot hold negative values or 0

//creation
// #define HASHTABLE_SIZE 100UL
// helper functions



//standard functionalilty

typedef struct Node {
    int key;
    int value;
    struct Node * next;
} Node;

typedef struct LinkedList {
    Node * sentinel;
} LinkedList;

typedef struct HashTable {
    LinkedList ** array;
    int size;
} HashTable;


int hashingFunction(struct HashTable * hashTable , int value, int * hashedValue) {
    int hashTableSize = hashTable -> size;
    int hashedValueInteger = value % hashTableSize; 
    *hashedValue = hashedValueInteger;
    return 0;
}


//add it to the back of a linked list
int pushOrChangeNode(Node * sentinel, int key, int value) {
    
    Node * temp = sentinel;
    while(temp -> next != NULL)  {
        if(temp != sentinel){
            if(temp -> key == key) {
                temp -> value = value; 
                return 0;
            }
        }
        temp = temp -> next;
    }
    //stops the node before the end of the list so stop
    if(temp -> key == key) {
        temp -> value = value; 
        return 0;
    }
    
    
    Node * newNode = malloc(sizeof(Node));
    newNode -> key = key;
    newNode -> value = value;
    newNode -> next = NULL;
    
    
    // temp -> next = newNode;
    temp -> next = newNode;
    return 0;
}

//pop from the list
int popNode(Node * sentinel, int key) {
    Node * temp = sentinel -> next;
    Node * prev = sentinel;
    while(temp != NULL) {
        if(temp -> key == key) {
            //found 
            prev -> next = temp -> next;
            temp -> next = NULL;
            
            free(temp);
            return 0;
        }
        prev = temp;
        temp = temp -> next;
    }
    return -1;
}

//-1 not found
//0 found
int searchLinkedList(Node * sentinel, int key, int * value) {
    Node * temp = sentinel;
    while(temp != NULL) {
        if(temp -> key == key && temp != sentinel) {
            
            *value = temp -> value;
            return 0;
        }
        temp = temp -> next;
    }
    return -1;
}

// //if 0 then success
int put(struct HashTable * hashTable, int key, int value){
    
    //take my key and hash it 
    int * hashedKey = malloc(sizeof(int));
    // printf("hashtable %x \n", hashTable);
    //TODO:IMPLEMENT HASH FUNCTION
    int status = hashingFunction(hashTable ,key, hashedKey);
    
    //use the hashed key to find where in the hashTable to insert it
    
    LinkedList * list = hashTable -> array[*hashedKey];
    Node * sentinel = list -> sentinel;

    
    pushOrChangeNode(sentinel, key, value);
    
    //free memory
    
    free(hashedKey);
    return 0;
}
//if -1 return then empty entry
//if 0 then check the value at the address of *value
int get(struct HashTable * hashTable, int key, int * value) {
    //take my key  and hash it
    int * hashedKey = malloc(sizeof(int));
    //TODO: implement HASH FUNCTION
    int status = hashingFunction(hashTable, key, hashedKey);
    //use the hashed key to get the sentinel at that location and then find the value with the non hashed key
   
    LinkedList * list = hashTable -> array[*hashedKey];
    Node * sentinel = list -> sentinel;
    
    if(searchLinkedList(sentinel, key, value) == -1) {
        //free before returning
        free(hashedKey);
        return -1;
    }


    //free memory
    free(hashedKey);
    return 0;
}

int delete (struct HashTable * hashTable, int key) {
    //delete 
    //get the hashed key
    int * hashedKey = malloc(sizeof(int));
    int status = hashingFunction(hashTable, key, hashedKey);
    LinkedList * list = hashTable -> array[*hashedKey];
    Node * sentinel = list -> sentinel;
    popNode(sentinel, key);
    return 0;
}

//STUB
int clearLinkedList(LinkedList* linkedList) {
    Node * sentinel = linkedList -> sentinel;
    Node * temp = sentinel;
    Node * nextNode = temp -> next;
    while(temp != NULL){
        free(temp);
        temp = nextNode;
        if(nextNode != NULL) {
            nextNode = nextNode -> next;
        }
    }
    return 0;
}

//STUB
int clearHashTable(HashTable * hashTable, int size) {
    //clear all nodes
     //clear all sentinels
    for(int i = 0; i < size; i++) {
        LinkedList * linkedList = hashTable -> array[i];
        clearLinkedList(linkedList);
        free(linkedList);
    }
    //clear all linked List
    //clear table
    free(hashTable);
    return 0;
}

int allocateHashTable(struct HashMap ** obj, int size) {
    struct HashTable * hashTable = malloc(sizeof(HashTable));
    hashTable -> array = malloc(sizeof(LinkedList *) * 100UL);
    hashTable -> size = size;

    
    for(int i = 0; i < size; i++) {
        LinkedList * list = malloc(sizeof(LinkedList));
        
        hashTable -> array[i] = list;
        Node * sentinel = malloc(sizeof(Node));
        list -> sentinel = sentinel;
        


        sentinel -> value = -100000;
        sentinel -> next = NULL;
        sentinel -> key = -100000;
    }
    *obj = hashTable;
}

int resizeHashTable(HashTable * hashTable, HashTable * newHashTable, int oldSize, int newSize) {
    
    
    
    for(int i = 0; i < oldSize; i++) {
        LinkedList * linkedList = hashTable -> array[i];
        Node * sentinel = linkedList -> sentinel;
        
        Node * temp = sentinel;
        while(temp != NULL){
            if(temp == sentinel){
                temp = temp -> next;
                continue;
            }
            int oldKey = temp -> key;
            int oldValue = temp -> value;
            //resize the hashmap
            put(newHashTable, oldKey, oldValue);
            
            
            temp = temp -> next;
        }
    }
    
    clearHashTable(hashTable, oldSize);
    
    
    return 0;
}

struct populateHashMapThreadedArguments{
    FILE * fptr;
    struct HashTable ** hashTable;
};

pthread_mutex_t m;

void* populateHashMapThreaded(void* arg){
    struct populateHashMapThreadedArguments* args = (struct populateHashMapThreadedARguments *) arg;
    int fileLine[1];

    while(1) {
        pthread_mutex_lock(&m);
        if(fscanf(args -> fptr, "%d", fileLine) == 1){

            int value = 0;
            int * hashValuePtr = malloc(sizeof(int));
            if(get(*(args -> hashTable), fileLine[0], hashValuePtr) == -1){
                value = 1;
            }
            else{
                value = *hashValuePtr + 1;
                delete(*(args -> hashTable), fileLine[0]);
            }
            put(*(args -> hashTable), fileLine[0], value);

            pthread_mutex_unlock(&m);
            
            free(hashValuePtr);
            

        }
        else{
            pthread_mutex_unlock(&m);
            break;
        }
        
        
    }
    
}


// //take in a file path and a reference to a pointer to return a populated hashmap
int fileToHashMap(char * filePath, struct HashTable ** hashTable, int size, int thread_size) {
    //open the file
    FILE* fptr;
    fptr = fopen(filePath, "r");
    if(fptr == NULL) {
        printf("File is not opened. Exiting.");
        return -1;
    }

    //create an empty hash map with the size passed over
    allocateHashTable(hashTable, size);

    //make this portion multithreaded

    pthread_t * pthread_array = malloc(thread_size * sizeof(pthread_t));
    pthread_mutex_init(&m, NULL);

    for(int i = 0; i < thread_size; i++) {
        struct populateHashMapThreadedArguments * pHMTA = malloc(sizeof(struct populateHashMapThreadedArguments));
        pHMTA -> fptr = fptr;
        pHMTA -> hashTable = hashTable;
        pthread_create(&pthread_array[i], NULL, populateHashMapThreaded, (void *) pHMTA);
    }

    for(int i = 0; i < thread_size; i++) {
        pthread_join(pthread_array[i], NULL);
    }
    pthread_mutex_destroy(&m);

    fclose(fptr);
    
    return 0;
}







int main(int argc, char ** argv)
{

    
    // hashing function non modulus 
     // hashing function modulus
    int * p = malloc(sizeof(int));
    // int n = 100;
    // hashingFunction(n, p);

    struct HashTable * hashTable = malloc(sizeof(HashTable));

    fileToHashMap("./numbers.txt", &hashTable, 100, 2);


    int status = get(hashTable, 1, p);

    printf("%d \n", *p);

    status = get(hashTable, 5, p);

    printf("%d \n", *p);

    status = get(hashTable, 10, p);

    printf("%d \n", *p);

    status = get(hashTable, 50, p);

    printf("%d \n", *p);

    // allocateHashTable(&hashTable, 100);
    // // memset(hashTable -> array, 0, HASHTABLE_SIZE);
    // // struct HashTable * newHashTable = malloc(sizeof(HashTable));

    // int * p = malloc(sizeof(int));
    // //get an empty value returns -1 
    // //put into an empty spot 
    // //get from a non empty spot returns the value stored
   
    // //put into a valid spot
    // int status = put(hashTable, 0, 1);
    // status = put(hashTable, 0, 20);
   
    // status = get(hashTable, 0, p);
    
    // status = delete(hashTable, 0);
    // status = get(hashTable, 0, p);
    // LinkedList * linkedList = hashTable -> array[0];
    // Node * sentinel = linkedList -> sentinel;
    
    // struct HashTable * newHashTable = malloc(sizeof(HashTable));

    // allocateHashTable(&newHashTable, 200);

    // resizeHashTable(hashTable, newHashTable, 100, 200);
   
    // status = put(newHashTable, 199, 23);

    return 0;
}
