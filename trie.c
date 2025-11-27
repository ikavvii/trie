#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

#define NUM_CHAR 256


typedef struct trie{
    struct trie* children[NUM_CHAR];
    bool terminal;
} trie;


trie* createNode(){
    trie* newNode = malloc(sizeof(trie));
    //setting null
    for(int i=0;i<NUM_CHAR;i++){
        newNode->children[i] = NULL;
    }
    newNode->terminal = false;
    return newNode;
}

bool insertNode(trie** root,char* str){

    if(*root == NULL){
        *root = createNode();
    }

    unsigned char* text = (unsigned char*)str;
    trie* tmp = *root;
    int len = strlen(str);

    for(int i=0;i<len;i++){
        if( tmp->children[text[i]]==NULL ){
            tmp->children[text[i]] = createNode();
        }
        tmp = tmp->children[text[i]];
    }

    if(tmp->terminal==true){
        return false;
    }else{
        tmp->terminal = true;
        return true;
    }
}



bool hasChildren(trie* node) {
    for (int i = 0; i < NUM_CHAR; i++) {
        if (node->children[i] != NULL)
            return true;
    }
    return false;
}

bool deleteTrieUtil(trie* node, char* str, int depth, bool* wordDeleted) {
    if (node == NULL)
        return false;

    if (depth == strlen(str)) {

        if (!node->terminal)
            return false;

        node->terminal = false;
        *wordDeleted = true;

        return !hasChildren(node);
    }

    unsigned char index = (unsigned char)str[depth];

    if (deleteTrieUtil(node->children[index], str, depth + 1, wordDeleted)) {
        free(node->children[index]);
        node->children[index] = NULL;

        return (!node->terminal && !hasChildren(node));
    }

    return false;
}


void deleteWord(trie** root, char* str) {
    if (*root == NULL)
        return false;

    bool wordDeleted = false;

    deleteTrieUtil(*root, str, 0, &wordDeleted);

    if (wordDeleted)
        printf("The word is deleted\n");
    else
        printf("Word not found\n");
}




void printTrieMap(trie* root, int level, char ch) {
    if (root == NULL) return;

    for (int i = 0; i < level; i++)
        printf("   ");

    if (level == 0)
        printf("(ROOT)\n");
    else
        printf("|-- %c%s\n", ch, root->terminal ? " (END)" : "");

    for (int i = 0; i < NUM_CHAR; i++) {
        if (root->children[i] != NULL) {
            printTrieMap(root->children[i], level + 1, (char)i);
        }
    }
}

void printtrie(trie* root) {
    if (root == NULL) {
        printf("(ROOT) -> EMPTY\n");
        return;
    }
    printTrieMap(root, 0, '\0');
}

bool searchStr(trie* root, char* str){
    trie* tmp = root;
    int len = strlen(str);

    for(int i=0;i<len;i++){
        if(tmp->children[str[i]]==NULL){
            return false;
        }
        tmp = tmp->children[str[i]];
    }
    return tmp->terminal;
}


void insertWords(trie* currRoot, char* buffer, int depth,
                 char res[][100], int* ri){

    if(currRoot->terminal){
        buffer[depth] = '\0';
        strcpy(res[*ri], buffer);
        (*ri)++;
    }

    for(int i=0;i<NUM_CHAR;i++){
        if(currRoot->children[i]!=NULL){
            buffer[depth] = (char)i;
            insertWords(currRoot->children[i], buffer, depth+1, res, ri);
        }
    }
}

void intellisense(char res[][100], char* prefix, trie* root, int* ri){

    trie* tmp = root;
    int len = strlen(prefix);

    for(int i=0;i<len;i++){
        if(tmp->children[(unsigned char)prefix[i]]==NULL)
            return;
        tmp = tmp->children[(unsigned char)prefix[i]];
    }

    char buffer[100];
    strcpy(buffer, prefix);
    insertWords(tmp, buffer, len, res, ri);
}


char* processChar(char c, bool reset) {
    static char str[100];
    static int i = 0;

    if (reset) {
        i = 0;
        str[0] = '\0';
        return str;
    }

    str[i++] = c;
    str[i] = '\0';
    printf("Current input: %s\n", str);
    return str;
}



void main() {
    char c;
    char* str;
    trie* root = NULL;
    static char res[100][100];
    int ri = 0;

    // Insert test words
    insertNode(&root,"tea");
    insertNode(&root,"test");
    insertNode(&root,"teacher");
    insertNode(&root,"team");
    insertNode(&root,"teach");

    printf("Trie structure:\n");
    printtrie(root);
    printf("\n");

    printf("Type characters (press ESC to stop, BACKSPACE to delete):\n");

    // Reset the static variable in processChar
    processChar(0, true);

    while(1){
        c = getch();

        if(c == 27){ // ESC key
            return;
        }
        else if(c == 8) { // BACKSPACE key
            // Handle backspace - you can implement this if needed
            continue;
        }
        else {
            str = processChar(c, false);
            ri = 0;
            intellisense(res, str, root, &ri);

            printf("Suggestions (%d found): \n", ri);
            printf("---------------\n");
            if(ri == 0) {
                printf("No suggestions found\n");
            } else {
                for(int i = 0; i < ri; i++){
                    printf("%s\n", res[i]);
                }
            }
            printf("---------------\n\n");
        }
    }
}




