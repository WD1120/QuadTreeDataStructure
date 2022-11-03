#ifndef _TREE_H_
#define _TREE_H_

#include "data.h"

/* Constant definition */
#define S_W 0
#define N_W 1
#define N_E 2
#define S_E 3
#define NUMQUAD 4
#define NUMPOINT 2
#define NUM_PINNACAL 4 
#define EPSILON 1e-12
#define LEN_QUAD 2
#define INITIAL_DATA 1

/* Structure declearation and definition */
typedef struct qtreeNode qtreeNode_t;

typedef struct {
    long double UpperRightX;
    long double UpperRightY;
    long double BottomLeftX;
    long double BottomLeftY;
} rectangle2D_t;

typedef struct {
    double xCo;
    double yCo;
} point2D_t;



/* Function declearation */
/*==========================================================================*/

/* Create a quadtree root with the region specified */
qtreeNode_t *createTree (long double xBL, long double yBL, long double xUR, long double yUR);

/* Checks if a node of quadtree is a leaf node without data stored */
int isEmptyLeaf(qtreeNode_t *node);

/* Checks if a node of quadtree is an internal node */
int isInternal(qtreeNode_t *node);

/* Checks if a point is inside a rectangle region */
int inRectangle(point2D_t *point, rectangle2D_t *rect);

/* Returns the start latitude and longitude from a data record */
point2D_t getStartPoint(data_t *data);

/* Returns the end latitude and longitude from a data record */
point2D_t getEndPoint(data_t *data);

/* Insert a new point into the quadtree */
qtreeNode_t *insertPoint(point2D_t pt, qtreeNode_t *root, data_t *data);

/* Determines which quardrant in a rectangle should the point be allocated to */
int determineQuar(point2D_t point, rectangle2D_t *rect);

/* Create a single node of the quadtree */
qtreeNode_t *newOneNode(long double xBL, long double yBL, long double xUR, long double yUR);

/* Attach four new nodes under a leaf node */
qtreeNode_t *newNode(qtreeNode_t *old);

/* Search if a given point is inside the quadtree */
data_t **searchPoint(qtreeNode_t *root, double x, double y, int *num);

/* Check if two points are the same, with a tiny allowence */
int checkSamePoint(qtreeNode_t *node, point2D_t *pt);

/* Free the memory of the quadtree */
void freeQTree(qtreeNode_t *root);

/* Checks whether two rectangles overlap */
int rectOverlap(rectangle2D_t *areaRect, rectangle2D_t *testRect);

/* Build a rectangle structure from for coordinates specifying its region */
rectangle2D_t buildRect(long double xBL, long double yBL, long double xUR, long double yUR);

/* Given a region, print out all the points in the quadtree within this region */
data_t **rangeQuery(qtreeNode_t *node, rectangle2D_t *area, FILE* out, data_t **ids, int *size, int *num);

#endif