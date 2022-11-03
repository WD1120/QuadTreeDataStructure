/*----------------------------------------------------
..Project: dict3
  dict3.c :
         = the implementation of dict 3

----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "list.h"
#include "tree.h"
#include "data.h"

/* Constant definitions */
#define STAGE_3 3
#define INITIAL_SIZE 1
#define DOUBLE_LEN 18

/* Function definitions */
int constructQTree (qtreeNode_t *root, list_t *cities);
int create_list (FILE *infile, list_t *lst);
void searchData (FILE *out, qtreeNode_t *root);


/*==========================================================================
Main Function
==========================================================================*/
int main(int argc, char *argv[]) {

    int stage = atoi(argv[1]);
    FILE *infile;
    infile = fopen(argv[2], "r");
    assert(infile);
    FILE *outfile;
    outfile = fopen(argv[3], "w");
    assert(outfile);

    list_t *cities = make_empty_list();
    create_list(infile, cities);
    
    if (stage == STAGE_3) {
        char *ptr = NULL;
        qtreeNode_t *root = createTree(strtold(argv[4], &ptr), strtold(argv[5], &ptr),
            strtold(argv[6], &ptr), strtold(argv[7], &ptr));
        constructQTree(root, cities); 
        searchData(outfile, root);
        freeQTree(root);
    }
    
    free_list(cities, string_free);
    fclose(infile);
    fclose(outfile);
    return 0;
}

/*==========================================================================*/

int create_list (FILE *infile, list_t *lst) {
    int n = 0;
    int flag = 1;
    char header[MAX_LINE_LEN];
    fscanf(infile, " %[^\n]", header);

    data_t *d = malloc(sizeof(*d));
    assert(d);
    while (flag) {
        flag = read_record(infile, d);
        if (flag) {
            insert_at_foot(lst, *d);
            n++;
        } else {
            break;
        }
    }
    free(d);
    return n;
}


/* Construct the Quadtree */
int constructQTree (qtreeNode_t *root, list_t *cities) {
    node_t *record = get_head(cities);
    int n = 1;
    while (record) {
        insertPoint(getStartPoint(&record->data), root, &record->data);
        insertPoint(getEndPoint(&record->data), root, &record->data);
        record = record->next;
        n++;
    }
    return n;
}


/* Search if the input data exist in the quadtree */
void searchData (FILE *out, qtreeNode_t *root) {
    /* Store the input data from stdin */
    int size = INITIAL_SIZE;
    int n = 0;
    char pointX[DOUBLE_LEN];
    char pointY[DOUBLE_LEN];
    char **pointXS = malloc(size * sizeof(char*));
    char **pointYS = malloc(size * sizeof(char*));
    assert(pointXS);
    assert(pointYS);
    while (scanf("%[^ ]%s\n", pointX, pointY) != EOF) {
        if (n >= size - 1) {
            pointXS = (char**)realloc(pointXS, (size*2)*sizeof(char*));
            pointYS = (char**)realloc(pointYS, (size*2)*sizeof(char*));
            assert(pointXS);
            assert(pointYS);
            size *= 2;
        }
        pointXS[n] = strdup(pointX);
        pointYS[n] = strdup(pointY);
        n++;
    }

    /* Search for the point */
    for (int i = 0; i < n; i++) {
        char *buffer;
        printf("%s %s -->", pointXS[i], pointYS[i]);
        int num = 0;
        data_t **data = searchPoint(root, strtod(pointXS[i], &buffer), 
            strtod(pointYS[i], &buffer), &num);
        if (data) {
            fprintf(out, "%s %s\n", pointXS[i], pointYS[i]);
            for (int j = 0; j < num; j++) {
                print_record(out, data[j]);
            }
        }
    }

    for (int k = 0; k < n; k++) {
        free(pointXS[k]);
        free(pointYS[k]);
    }
    free(pointXS);
    free(pointYS);
}