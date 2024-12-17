/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//implement a hashtable
//collision avoidance
//linear chained

//int -> int
//hashmap cannot hold negative values or 0

//creation
#define HASHTABLE_SIZE 100UL
// helper functions

int hashingFunction(int value, int * hashedValue) {
    int hashedValueInteger = value % HASHTABLE_SIZE; 
    *hashedValue = hashedValueInteger;
    return 0;
}

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
} HashTable;



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
            
            free(prev);
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
    printf("hashtable %x \n", hashTable);
    //TODO:IMPLEMENT HASH FUNCTION
    int status = hashingFunction(key, hashedKey);
    
    //use the hashed key to find where in the hashTable to insert it
    
    LinkedList * list = hashTable -> array[*hashedKey];
    Node * sentinel = list -> sentinel;
    printf("sentinel %x \n", list -> sentinel);
    
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
    int status = hashingFunction(key, hashedKey);
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
    int status = hashingFunction(key, hashedKey);
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

int resizeHashTable(HashTable * hashTable, HashTable * newHashTable, int oldSize, int newSize) {
    
    newHashTable -> array = malloc(sizeof(LinkedList *) * newSize);
    
    for(int i = 0; i < newSize; i++) {
        LinkedList * list = malloc(sizeof(LinkedList));
        newHashTable -> array[i] = list;
        Node * sentinel = malloc(sizeof(Node));
        list -> sentinel = sentinel;
       
        // printf("%x\n", list.sentinel);
        sentinel -> value = -100000;
        sentinel -> next = NULL;
        sentinel -> key = -100000;
    }
    
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
    
    
    // clearHashTable(hashTable, oldSize);
    
    
    return 0;
}







int main(int argc, char ** argv)
{
    // printf("Hello World");
    // int array[3];
    // printf("%d\n", array[0]);
    
    // hashing function non modulus 
     // hashing function modulus
    // int * p = malloc(sizeof(int));
    // int n = 100;
    // hashingFunction(n, p);
    // printf("%d", *p);
    
    struct HashTable * hashTable = malloc(sizeof(HashTable));
    hashTable -> array = malloc(sizeof(LinkedList *) * HASHTABLE_SIZE);
    
    for(int i = 0; i < HASHTABLE_SIZE; i++) {
        LinkedList * list = malloc(sizeof(LinkedList));
        
        hashTable -> array[i] = list;
        Node * sentinel = malloc(sizeof(Node));
        list -> sentinel = sentinel;
        
        printf("sentinel %d  address %x\n", i, sentinel);
       
        // printf("%x\n", list.sentinel);
        sentinel -> value = -100000;
        sentinel -> next = NULL;
        sentinel -> key = -100000;
    }
    // memset(hashTable -> array, 0, HASHTABLE_SIZE);
    
    int * p = malloc(sizeof(int));
    //get an empty value returns -1 
    //put into an empty spot 
    //get from a non empty spot returns the value stored
   
    //put into a valid spot
    int status = put(hashTable, 0, 1);
    status = put(hashTable, 0, 20);
   
    status = get(hashTable, 0, p);
    
    status = delete(hashTable, 0);
    status = get(hashTable, 0, p);
    LinkedList * linkedList = hashTable -> array[0];
    Node * sentinel = linkedList -> sentinel;
    
    HashTable * newHashTable = malloc(sizeof(HashTable));
    HashTable * newHashTable2 = malloc(sizeof(HashTable));
    HashTable * newHashTable3 = malloc(sizeof(HashTable));
    printf("old hashTable %x \n", hashTable);
    printf("old sentinel %x \n", sentinel);
    printf("old Linked List %x \n", linkedList);
    printf("hashTable %x %x %x\n", newHashTable, newHashTable2, newHashTable3);
    resizeHashTable(hashTable, newHashTable, 100, 200);
   
    
    status = put(newHashTable, 199, 23);

    return 0;
}
