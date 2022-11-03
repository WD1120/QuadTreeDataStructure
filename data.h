/*----------------------------------------------------
..Project: dict1 dict2
  data.h :
         = the interface of module data of the project
----------------------------------------------------*/

#ifndef _DATA_H_
#define _DATA_H_

/* constant definitions */
#define MAX_STR_LEN 128
#define MAX_LINE_LEN 512

/* list definitions */
typedef struct data {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
} data_t;


/* Function prototypes */
/*==========================================================================*/

/* Reads one line of record in from a csv file */
int read_record (FILE *f, data_t *d);

/* Reads in a string from the csv file, with the special formatting */
void* read_string(FILE *f, char* str);

/* Prints out one record in one line */
void print_record(FILE *f, data_t *d);

/* Compares the grade1in value of two records */
int city_cmp_grade1in(data_t *A, data_t *B);

/* Frees the strings in the data structure */
void *string_free(void *s);

/* Returns the footpath id of the data record */
int getID(data_t *data);

/* A comparison function for sorting in ascending order of footpath id */
int ascendingOrder(const void *A, const void *B);

#endif