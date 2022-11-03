/*----------------------------------------------------
..Project: dict4
  dict4.c :
         = the implementation of dict4

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
#define STAGE_4 4
#define INITIAL_SIZE 1
#define NUM_POINTS 4
#define DOUBLE_LEN 18

/* Function definitions */
int constructQTree (qtreeNode_t *root, list_t *cities);
int create_list (FILE *infile, list_t *lst);
void searchQuery (FILE *out, qtreeNode_t *node, int n);

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
    int n = create_list(infile, cities);
    
    if (stage == STAGE_4) {
        char *ptr = NULL;
        qtreeNode_t *root = createTree(strtold(argv[4], &ptr), strtold(argv[5], &ptr),
            strtold(argv[6], &ptr), strtold(argv[7], &ptr));
        constructQTree(root, cities); 
        searchQuery(outfile, root, n);
        freeQTree(root);
    }
    
    free_list(cities, string_free);
    fclose(infile);
    fclose(outfile);
    return 0;
}


/*==========================================================================*/

/* Scan through the csv file and store the records in linked list */
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
        //printf("Data %d\n", n);
        record = record->next;
        n++;
    }
    return n;
}


/* Search for the points inside the region given by input */
void searchQuery (FILE *out, qtreeNode_t *root, int nrecord) {
    /* Store the input data */
    int size = INITIAL_SIZE;
    int n = 0;
    char point[NUM_POINTS][DOUBLE_LEN];
    char **pointXBL = malloc(size * sizeof(char*));
    char **pointYBL = malloc(size * sizeof(char*));
    char **pointXUR = malloc(size * sizeof(char*));
    char **pointYUR = malloc(size * sizeof(char*));
    assert(pointXBL);
    assert(pointYBL);
    assert(pointXUR);
    assert(pointYUR);
    while (scanf("%[^ ] %[^ ] %[^ ] %s\n", point[0], point[1], point[2], point[3]) != EOF) {
        if (n >= size - 1) {
            pointXBL = realloc(pointXBL, (size*2)*sizeof(char*));
            pointYBL = realloc(pointYBL, (size*2)*sizeof(char*));
            pointXUR = realloc(pointXUR, (size*2)*sizeof(char*));
            pointYUR = realloc(pointYUR, (size*2)*sizeof(char*));
            assert(pointXBL);
            assert(pointYBL);
            assert(pointXUR);
            assert(pointYUR);
            size *= 2;
        }
        pointXBL[n] = strdup(point[0]);
        assert(pointXBL[n]);
        pointYBL[n] = strdup(point[1]);
        assert(pointYBL[n]);
        pointXUR[n] = strdup(point[2]);
        assert(pointXUR[n]);
        pointYUR[n] = strdup(point[3]);
        assert(pointYUR[n]);
        n++;
    }

    for (int i = 0; i < n; i++) {
        char *buffer;
        fprintf(out, "%s %s %s %s\n", pointXBL[i], pointYBL[i], pointXUR[i], pointYUR[i]);
        printf("%s %s %s %s -->", pointXBL[i], pointYBL[i], pointXUR[i], pointYUR[i]);
        rectangle2D_t inputArea = buildRect(strtod(pointXBL[i], &buffer), strtod(pointYBL[i], &buffer), 
            strtod(pointXUR[i], &buffer), strtod(pointYUR[i], &buffer));
        data_t *ids[nrecord];
        int num = 0;

        /* Search for the data points in the query, print directions to stdout */
        rangeQuery(root, &inputArea, out, ids, &size, &num);
        printf("\n");

        /* Sort and print out the records in the file */
        qsort(ids, num, sizeof(data_t*), ascendingOrder);
        for (int i = 0; i < num; i++) {
            print_record(out, ids[i]);
        }
    }

    for (int k = 0; k < n; k++) {
        free(pointXBL[k]);
        free(pointYBL[k]);
        free(pointXUR[k]);
        free(pointYUR[k]);        
    }
    free(pointXBL);
    free(pointXUR);
    free(pointYBL);
    free(pointYUR);
}
