/*
 * In this experiment we are expected to build a tree structure. Also there should be two
 * command to manipulate the tree whose are 'deletion' and 'list'.
 * This program takes directories of two input file from CONSOLE. First input file consists two columns.
 * First column specify the values of nodes of tree. And second one stands for how many
 * node will be added at each iteration. Second input file consists the commands; deletion and
 * list.
 * This program creates the tree according to first input file. Then, it can operate the list
 * operation. But it does not have the ability for deletion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Definitions of types.
 */
typedef struct Node{
    int data;
    int numberOfChildren;
    struct Node **leaf;
}node;

/*
 * Definitions of global variable.
 */
node **leafNodes;
int numberOfLeafNodes;

/*
 * Declerations of the functions.
 */
void findLeafNodes(node **leaf);
void insert(node **leaf, int data);
void list(node *nod, FILE **outputfile);
node* search(node *nod, int data);

/*
 * The main function takes the directories of input files and reads them. According to
 * those files it creates the tree and runs the operations on that tree.
 */
int main(int argc, char **argv){

    char *arg1 = (char*) malloc(100 * sizeof(char));            /* arg1 stands for the directory of first input file */
    char *arg2 = (char*) malloc(100 * sizeof(char));            /* arg2 stands for the directory of second input file */
    printf("Enter directories of input1 and input2 files respectively:\n");
    scanf("%s %s", arg1, arg2);

    FILE *file;
    file = fopen(arg1, "r");                        /* opens the first file */
    static char line[10];
    char *word;
    int element[256], count[256];
    int i = 0;
    while(fgets(line, sizeof(line), file)){         /* element array consists the values of nodes of tree those will be added */
        word = strtok(line, " ");                   /* count array consists the count that how many node will be added at each iteration */
        element[i] = atoi(word);
        word = strtok(NULL, "\n");
        count[i] = atoi(word);
        i++;
    }
    fclose(file);
    int numberOfElements = i;
    i = 1;

    node *root;                                     /* Decleration of root of the tree */
    root = NULL;

    insert(&root, element[0]);

    int j = 0;
    int k = 0;
    int condition = 1;
    while( ( j < numberOfElements ) ){
        leafNodes = NULL;                   /* At each iteration the leaf nodes are reseted */
        numberOfLeafNodes = 0;

        findLeafNodes(&root);               /* Finds the up-to-date leaf nodes */

        while( k < count[j]){
            insert( (leafNodes + ( k % numberOfLeafNodes ) ), element[i] );     /* Calling insert function to add new nodes */
            i++;
            if( i >= numberOfElements ){            /* If the elements are done then it stops the creation of the tree */
                condition = 0;
                break;
            }
            k++;
        }
        if( condition == 0 ){
            break;
        }
        k = 0;
        j++;
    }

    file = fopen(arg2, "r");
    FILE *outputfile = fopen("output.txt", "w");
    while(fgets(line, sizeof(line), file)){                     /* Reads second input file which consists operations */
        word = strtok(line, " ");
        if( strcmp(word, "l") == 0 ){                           /* If the command is list then it finds the node which */
            word = strtok(NULL, "\n");                          /* will be the start node of listing */
            int n = atoi(word);
            node *temp = NULL;
            temp = search(root, n);                             /* Calling search function */
            if( temp != NULL ){                                 /* If the node is exist then program prints that node and */
                list(temp, &outputfile);                        /* its children to the output file */
                fprintf(outputfile, "\n");
            }
        }
        else if( strcmp(word, "d") == 0){

        }
    }
    fclose(file);

    return 0;
}

/*
 * This function finds leaf nodes in the tree and keeps them in 'leafNodes' array.
 */
void findLeafNodes(node **leaf){
    if( ( *(leaf) )->leaf == NULL ){                        /* If node does not have any children node then it is a leaf node */
        if(numberOfLeafNodes == 0){
            numberOfLeafNodes++;
            leafNodes = (node**) malloc( numberOfLeafNodes * sizeof(node*) );
        }
        else{
            numberOfLeafNodes++;
            leafNodes = (node**) realloc( leafNodes, numberOfLeafNodes * sizeof(node*) );
        }
        *(leafNodes + numberOfLeafNodes - 1) = *leaf;
        return;
    }
    else{                                                   /* If node has children then program looks for if its children are leaf node */
        int i = 0;

        while( i < ( *(leaf) )->numberOfChildren ){
            findLeafNodes( ( ( *(leaf) )->leaf + i ) );
            i++;
        }
        return;
    }
}

/*
 * This function creates the node with given data and add it to tree.
 */
void insert(node **leaf, int data){

    node *temp = NULL;

    if( *leaf == NULL ){                                        /* If node is NULL then new node will be added here */
        temp = (node*) malloc( sizeof( node ) );
        temp->leaf = NULL;
        temp->numberOfChildren = 0;
        temp->data = data;
        *leaf = temp;
        return;
    }

    if( ( *(leaf) )->numberOfChildren == 0 ){                   /* If leaf node has no children then it creates a list for keeping the children for future usage */
        ( *(leaf) )->numberOfChildren = ( *(leaf) )->numberOfChildren + 1;
        ( *(leaf) )->leaf = (node**) malloc( ( *(leaf) )->numberOfChildren * sizeof(node*) );               /* malloc */
        *( ( *(leaf) )->leaf + ( ( *(leaf) )->numberOfChildren - 1 ) ) = NULL;
        insert( ( ( *(leaf) )->leaf + ( ( *(leaf) )->numberOfChildren - 1 ) ), data );
    }
    else{                                                       /* If leaf node has children then it reallocates the list of children for more adding */
        ( *(leaf) )->numberOfChildren = ( *(leaf) )->numberOfChildren + 1;
        ( *(leaf) )->leaf = (node**) realloc( ( *(leaf) )->leaf, ( *(leaf) )->numberOfChildren * sizeof(node*) );       /* realloc */
        *( ( *(leaf) )->leaf + ( ( *(leaf) )->numberOfChildren - 1 ) ) = NULL;
        insert( ( ( *(leaf) )->leaf + ( ( *(leaf) )->numberOfChildren - 1 ) ), data );
    }
}

/*
 * This function prints the tree begining from the given node.
 */
void list(node *nod, FILE **outputfile){
    if(nod){
        int i = 0;
        fprintf(*outputfile, "%d ", nod->data);
        while( i < nod->numberOfChildren ){
            list( *( nod->leaf + i) , outputfile );
            i++;
        }
    }
}

/*
 *  This function searches is there any node that have the same value with the given data.
 *  If there is then it returns the node otherwise it returns NULL.
 */
node* search(node *nod, int data){
    if( nod == NULL ){                          /* If node is NULL return NULL */
        return NULL;
    }
    else{
        if( nod->data == data ){                /* If it is matched return node */
            return nod;
        }
        else{                                   /* If no match then it looks for children of this node recursively */
            int i = 0;
            node *temp = NULL;
            while( i < nod->numberOfChildren ){
                temp = search( *( nod->leaf + i ), data);
                if( temp != NULL ){
                    break;
                }
                i++;
            }
            return temp;
        }
    }
}