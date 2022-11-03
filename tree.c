#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "tree.h"
#include "data.h"

/* Structure definition */
struct qtreeNode {
    qtreeNode_t *quad[NUMQUAD];
    data_t **data;
    int num_data;
    point2D_t pt;
    rectangle2D_t area;
    int isInternal;
};

/* Function definition*/
/*==========================================================================*/

/* Create a quadtree root with the region specified */
qtreeNode_t *createTree (long double xBL, long double yBL, long double xUR, long double yUR) {
    qtreeNode_t *root = (qtreeNode_t*) malloc(sizeof(qtreeNode_t));
    root->area = buildRect(xBL, yBL, xUR, yUR);

    for (int i = 0; i < NUMQUAD; i++) {
        root->quad[i] = NULL;
    }
    root->data = NULL;
    root->pt.xCo = -1;
    root->isInternal = 0;
    root->num_data = 0;
    return root;
}

/* Checks if a node of quadtree is an internal node */
int isInternal (qtreeNode_t *node) {
    for (int i = 0; i < NUMQUAD; i++) {
        if (node->quad[i] != NULL) {
            return 1;
        }
    }
    return 0;
}

/* Checks if a node of quadtree is a leaf node without data stored */
int isEmptyLeaf (qtreeNode_t *node) {
    if (!isInternal(node) && node->num_data == 0) return 1;
    return 0;
}

/* Checks if a point is inside a rectangle region */
int inRectangle (point2D_t *point, rectangle2D_t *rect) {
    if (point->xCo <= rect->UpperRightX && point->xCo > rect->BottomLeftX &&
        point->yCo >= rect->BottomLeftY && point->yCo < rect->UpperRightY) {
            return 1;
        }
    return 0;
}

/* Returns the start latitude and longitude from a data record */
point2D_t getStartPoint(data_t *data) {
    point2D_t pt;
    pt.xCo = data->start_lon;
    pt.yCo = data->start_lat;
    return pt;
}

/* Returns the end latitude and longitude from a data record */
point2D_t getEndPoint(data_t *data) {
    point2D_t pt;
    pt.xCo = data->end_lon;
    pt.yCo = data->end_lat;
    return pt;
}

/* Insert a new point into the quadtree */
qtreeNode_t *insertPoint (point2D_t pt, qtreeNode_t *node, data_t *data) {
    
    if (!inRectangle(&pt, &node->area)) {
        return NULL;
    }
    if ((node->isInternal == 0) && (node->num_data == 0)) {
        /* If it is an empty leaf node, insert data directly */
        node->pt = pt;
        node->data = malloc(INITIAL_DATA * sizeof(data_t*));
        assert(node->data);
        node->data[node->num_data] = data;
        node->num_data += 1;
        return node;

    } else {
        int newQuar = determineQuar(pt, &node->area);
        if ((node->isInternal == 0) && (node->num_data != 0)) {
            /* If the leaf node already contains data */
            if (checkSamePoint(node, &pt)) {
                /* If the two points are identical, attach the data structure to the
                same node. */
                int num = node->num_data;
                node->data = realloc(node->data, (num+1)*sizeof(data_t*));
                assert(node->data);
                /* Sort the records by footpathID. */
                while (getID(node->data[num-1]) > getID(data)) {
                    node->data[num] = node->data[num-1];
                    num -= 1;
                    if (num == 0) {
                        break;
                    }
                }
                node->data[num] = data;
                node->num_data += 1;

            } else {
                /* Otherwise, attach 4 new nodes under it and move the old data
                into the proper quadrant, then recurse to the new node */
                node = newNode(node);
                int oldQuar = determineQuar(node->pt, &node->area);
                node->quad[oldQuar]->data = malloc(node->num_data * sizeof(node->data));
                assert(node->quad[oldQuar]->data);
                for (int i = 0; i < node->num_data; i++) {
                    node->quad[oldQuar]->data[i] = node->data[i];
                    node->quad[oldQuar]->num_data += 1;
                }
                node->quad[oldQuar]->pt = node->pt;
                node->pt.xCo = -1;
                node = insertPoint (pt, node->quad[newQuar], data);
            }

        } else {
            /* If it is an internal node, recursively find the leaf node */

            node->num_data += 1;
            node = insertPoint(pt, node->quad[newQuar], data);
        }
    }
    return NULL;
}

/* Determines which quardrant in a rectangle should the point be allocated to */
int determineQuar (point2D_t point, rectangle2D_t *rect) {
    long double midX = (long double) (rect->BottomLeftX + rect->UpperRightX) / 2;
    long double midY = (long double) (rect->BottomLeftY + rect->UpperRightY) / 2;

    if (point.xCo <= midX && point.yCo < midY) return S_W;
    else if (point.xCo <= midX && point.yCo >= midY) return N_W;
    else if (point.xCo > midX && point.yCo >= midY) return N_E;
    else if (point.xCo > midX && point.yCo < midY) return S_E;
    return -1;
}

/* Create a single node of the quadtree */
qtreeNode_t *newOneNode(long double xBL, long double yBL, long double xUR, long double yUR) {
    qtreeNode_t *new = malloc(sizeof(qtreeNode_t));
    assert(new);
    rectangle2D_t rect = {.BottomLeftX=xBL, .BottomLeftY=yBL, .UpperRightX=xUR, .UpperRightY=yUR};
    new->area = rect;
    
    for (int i = 0; i < NUMQUAD; i++) {
        new->quad[i] = NULL;
    }
    new->data = NULL;
    new->num_data = 0;
    new->pt.xCo = -1;
    new->isInternal = 0;
    return new;
}


/* Attach four new nodes under a leaf node */
qtreeNode_t *newNode(qtreeNode_t *old) {
    old->isInternal = 1;
    long double midX = (long double) (old->area.BottomLeftX + old->area.UpperRightX) / (long double) 2;
    long double midY = (long double) (old->area.BottomLeftY + old->area.UpperRightY) / (long double) 2;

    old->quad[S_W] = newOneNode(old->area.BottomLeftX, old->area.BottomLeftY, midX, midY);
    old->quad[N_W] = newOneNode(old->area.BottomLeftX, midY, midX, old->area.UpperRightY);
    old->quad[N_E] = newOneNode(midX, midY, old->area.UpperRightX, old->area.UpperRightY);
    old->quad[S_E] = newOneNode(midX, old->area.BottomLeftY, old->area.UpperRightX, midY);

    return old;
}

/* Search if a given point is inside the quadtree */
data_t **searchPoint(qtreeNode_t *root, double x, double y, int *num) {
    point2D_t pt;
    pt.xCo = x;
    pt.yCo = y;
    qtreeNode_t *node = root;
    
    /* Traverse through to the leaf node, print the direction which 
    have travelled through */
    while (node->isInternal) {
        int quad = determineQuar(pt, &node->area);
        switch (quad) {
            case 0:
                printf(" SW");
                break;
            case 1:
                printf(" NW");
                break;
            case 2:
                printf(" NE");
                break;
            case 3:
                printf(" SE");
        }
        node = node->quad[quad];
    }
    printf("\n");

    /* If the leaf node contains data and the data is indeed what we are looking for,
    return it */
    if (node->data && node->pt.xCo != -1) {
        if (checkSamePoint(node, &pt)) {
            *num = node->num_data;
            return node->data;
        }
    }
    return NULL;
}

/* Check if two points are the same, with a tiny allowence */
int checkSamePoint(qtreeNode_t *node, point2D_t *pt) {
    double diffX = node->pt.xCo - pt->xCo;
    double diffY = node->pt.yCo - pt->yCo;
    if (fabs(diffX) < EPSILON && fabs(diffY) < EPSILON) return 1;
    return 0;
}

/* Free the memory of the quadtree */
void freeQTree(qtreeNode_t *node) {
    if (node->data) {
        free(node->data);
        node->data = NULL;
    } 
    for (int i = 0; i < NUMQUAD; i++) {
        if (node->quad[i]) {
            freeQTree(node->quad[i]);
        }
    }
    free(node);    
}

/* Checks whether two rectangles overlap */
int rectOverlap(rectangle2D_t *areaRect, rectangle2D_t *testRect) {
    if (testRect->BottomLeftX > areaRect->UpperRightX ||
        testRect->UpperRightX < areaRect->BottomLeftX ||
        testRect->BottomLeftY > areaRect->UpperRightY ||
        testRect->UpperRightY < areaRect->BottomLeftY) {
        return 0;
    }
    return 1;
}

/* Build a rectangle structure from for coordinates specifying its region */
rectangle2D_t buildRect(long double xBL, long double yBL, long double xUR, long double yUR) {
    rectangle2D_t rect;
    rect.BottomLeftX = xBL;
    rect.BottomLeftY = yBL;
    rect.UpperRightX = xUR;
    rect.UpperRightY = yUR;
    return rect;
}


/* Given a region, print out all the points in the quadtree within this region */
data_t **rangeQuery(qtreeNode_t *node, rectangle2D_t *inputArea, FILE* out, data_t **ids, int *size, int *num) {
    /* Only proceed if the region does overlap */
    if (rectOverlap(inputArea, &node->area)) {
        
        /* If a leaf node is reached and the data it contain is inside the region */
        if (!node->isInternal && node->num_data && inRectangle(&node->pt, inputArea)) {
            for (int i = 0; i < node->num_data; i++) {
                int printed = 0;
                /* Checks if the data has already been printed */
                for (int j = 0; j < *num; j++) {
                    if (getID(ids[j]) == getID(node->data[i])) {
                        printed = 1;
                        break;
                    }
                }
                /* If it hasn't, append the data into the array; otherwise do nothing */
                if (printed == 0) {
                    ids[*num] = node->data[i];
                    *num += 1;
                }
            }

        } else if (node->isInternal) {
            /* If it isi an internal node, recurse all its child nodes if they 
            overlaps with the given region, while printing out the directions 
            that have been traversed */
            for (int j = 0; j < NUMQUAD; j++) {
                if ((isInternal(node->quad[j]) || node->quad[j]->data) 
                    && rectOverlap(inputArea, &node->quad[j]->area)) {
                    switch (j) {
                        case S_W:
                            printf(" SW");
                            break;
                        case N_W:
                            printf(" NW");
                            break;
                        case N_E:
                            printf(" NE");
                            break;
                        case S_E:
                            printf(" SE");
                    }
                    rangeQuery(node->quad[j], inputArea, out, ids, size, num);
                }
            }
        }
    }
    return ids;
}
