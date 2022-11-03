/*----------------------------------------------------
..Project: dict1 dict2
  data.c :
         = the implementation of module data of the project

----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"

/*==========================================================================*/


/* Reads one line of record in from a csv file */
int read_record (FILE *f, data_t *d) {
    double footpath_id = 0, mcc_id, mccid_int, statusid, streetid, street_group;
    char address[MAX_STR_LEN + 1];
    char clue_sa[MAX_STR_LEN + 1];
    char asset_type[MAX_STR_LEN + 1];
    char segside[MAX_STR_LEN + 1];

    if (fscanf(f, "%lf,", &footpath_id) == 0 || footpath_id == 0) {
        return 0;
    }
    read_string(f, address);
    read_string(f, clue_sa);
    read_string(f, asset_type);
    fscanf(f, "%lf,", &d->deltaz);
    fscanf(f, "%lf,", &d->distance);
    fscanf(f, "%lf,", &d->grade1in);
    fscanf(f, "%lf,", &mcc_id);
    fscanf(f, "%lf,", &mccid_int);
    fscanf(f, "%lf,", &d->rlmax);
    fscanf(f, "%lf,", &d->rlmin);
    read_string(f, segside);
    fscanf(f, "%lf,", &statusid);
    fscanf(f, "%lf,", &streetid);
    fscanf(f, "%lf,", &street_group);
    fscanf(f, "%lf,", &d->start_lat);
    fscanf(f, "%lf,", &d->start_lon);
    fscanf(f, "%lf,", &d->end_lat);
    fscanf(f, "%lf\n", &d->end_lon);
    
    /* Copy elements into struct data_t */
    d->footpath_id = (int) footpath_id;
    if (strlen(address) <= MAX_STR_LEN + 1) {
        d->address = strdup(address);
        assert(d->address);
    } 
    if (strlen(clue_sa) <= MAX_STR_LEN + 1) {
        d->clue_sa = strdup(clue_sa);
        assert(d->clue_sa);
    } 
    if (strlen(asset_type) <= MAX_STR_LEN + 1) {
        d->asset_type = strdup(asset_type);
        assert(d->asset_type);
    } 
    d->mcc_id = (int) mcc_id;
    d->mccid_int = (int) mccid_int;
    if (strlen(segside) <= MAX_STR_LEN + 1) {
        d->segside = strdup(segside);
        assert(d->segside);
    } 
    d->statusid = (int) statusid;
    d->streetid = (int) streetid;
    d->street_group = (int) street_group;

    return 1;
}


/* Reads in a string from the csv file, with the special formatting */
void *read_string(FILE *f, char* str) {
    int i = 0;
    char temp[1];
    
    if ((temp[0] = fgetc(f)) == '\"') {
        while ((str[i] = fgetc(f)) != '\"') {
            i++;
        }
        fgetc(f);
    } else if (temp[0] == ',') {
        str[i] = '\0';
        return 0;
    } else {
        str[i] = temp[0];
        i++;
        while ((str[i] = fgetc(f)) != ',') {
            i++;
        }
    }
    str[i] = '\0';
    return 0;
}


/* Prints out one record in one line */
void print_record(FILE *f, data_t *d) {

    fprintf(f, "--> footpath_id: %d ", d->footpath_id);
    fprintf(f, "|| address: %s ", d->address);
    fprintf(f, "|| clue_sa: %s ", d->clue_sa);
    fprintf(f, "|| asset_type: %s ", d->asset_type);
    fprintf(f, "|| deltaz: %.2lf ", d->deltaz);
    fprintf(f, "|| distance: %.2lf ", d->distance);
    fprintf(f, "|| grade1in: %.1lf ", d->grade1in);
    fprintf(f, "|| mcc_id: %d ", d->mcc_id);
    fprintf(f, "|| mccid_int: %d ", d->mccid_int);
    fprintf(f, "|| rlmax: %.2lf ", d->rlmax);
    fprintf(f, "|| rlmin: %.2lf ", d->rlmin);
    fprintf(f, "|| segside: %s ", d->segside);
    fprintf(f, "|| statusid: %d ", d->statusid);
    fprintf(f, "|| streetid: %d ", d->streetid);
    fprintf(f, "|| street_group: %d ", d->street_group);
    fprintf(f, "|| start_lat: %lf ", d->start_lat);
    fprintf(f, "|| start_lon: %lf ", d->start_lon);
    fprintf(f, "|| end_lat: %lf ", d->end_lat);
    fprintf(f, "|| end_lon: %lf || \n", d->end_lon);

}


/* Compares the grade1in value of two records */
int city_cmp_grade1in(data_t *A, data_t *B) {
    if (A->grade1in < B->grade1in) return -1;
    if (A->grade1in > B->grade1in) return 1;
    return 0;
}


/* Frees the strings in the data structure */
void *string_free(void *s) {
    free(((data_t *)s)->address);
    free(((data_t *)s)->asset_type);
    free(((data_t *)s)->clue_sa);
    free(((data_t *)s)->segside);

    return NULL;
}


/* Returns the footpath id of the data record */
int getID(data_t *data) {
    return data->footpath_id;
}


/* A comparison function for sorting in ascending order of footpath id */
int ascendingOrder(const void *A, const void *B) {
    if (getID(*(data_t**)A) < getID(*(data_t**)B)) {
        return -1;
    }
    return 1;
}
