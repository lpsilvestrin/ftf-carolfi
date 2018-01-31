/*****************************************************************************/
/*IMPORTANT:  READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.         */
/*By downloading, copying, installing or using the software you agree        */
/*to this license.  If you do not agree to this license, do not download,    */
/*install, copy or use the software.                                         */
/*                                                                           */
/*                                                                           */
/*Copyright (c) 2005 Northwestern University                                 */
/*All rights reserved.                                                       */

/*Redistribution of the software in source and binary forms,                 */
/*with or without modification, is permitted provided that the               */
/*following conditions are met:                                              */
/*                                                                           */
/*1       Redistributions of source code must retain the above copyright     */
/*        notice, this list of conditions and the following disclaimer.      */
/*                                                                           */
/*2       Redistributions in binary form must reproduce the above copyright   */
/*        notice, this list of conditions and the following disclaimer in the */
/*        documentation and/or other materials provided with the distribution.*/
/*                                                                            */
/*3       Neither the name of Northwestern University nor the names of its    */
/*        contributors may be used to endorse or promote products derived     */
/*        from this software without specific prior written permission.       */
/*                                                                            */
/*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS    */
/*IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED      */
/*TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND         */
/*FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL          */
/*NORTHWESTERN UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,       */
/*INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          */
/*(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR          */
/*SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)          */
/*HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,         */
/*STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN    */
/*ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             */
/*POSSIBILITY OF SUCH DAMAGE.                                                 */
/******************************************************************************/

/*************************************************************************/
/**   File:         example.c                                           **/
/**   Description:  Takes as input a file:                              **/
/**                 ascii  file: containing 1 data point per line       **/
/**                 binary file: first int is the number of objects     **/
/**                              2nd int is the no. of features of each **/
/**                              object                                 **/
/**                 This example performs a fuzzy c-means clustering    **/
/**                 on the data. Fuzzy clustering is performed using    **/
/**                 min to max clusters and the clustering that gets    **/
/**                 the best score according to a compactness and       **/
/**                 separation criterion are returned.                  **/
/**   Author:  Wei-keng Liao                                            **/
/**            ECE Department Northwestern University                   **/
/**            email: wkliao@ece.northwestern.edu                       **/
/**                                                                     **/
/**   Edited by: Jay Pisharath                                          **/
/**              Northwestern University.                               **/
/**                                                                     **/
/**   ================================================================  **/
/**																		**/
/**   Edited by: Sang-Ha  Lee											**/
/**				 University of Virginia									**/
/**																		**/
/**   Description:	No longer supports fuzzy c-means clustering;	 	**/
/**					only regular k-means clustering.					**/
/**					Simplified for main functionality: regular k-means	**/
/**					clustering.											**/
/**                                                                     **/
/*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/types.h>
#include <fcntl.h>
#include <omp.h>
#include "getopt.h"

#include "kmeans.h"



extern double wtime(void);

int num_omp_threads = 1;

void read_gold(int nclusters, int numAttributes, char * out_filename, float **cluster_centres_gold) {
    int i, j;
    FILE *file;
    if( (file = fopen(out_filename, "rb" )) == 0 )
        printf( "The GOLD file was not opened\n" );
    for (i=0; i< nclusters; i++) {
        fread(&j, 1, sizeof(int), file);
        for (j=0; j<numAttributes; j++) {
            fread(&cluster_centres_gold[i][j], 1, sizeof(float), file);
        }

    }
    fclose(file);
}


int compare_gold(int nclusters, int numAttributes, float **cluster_centres, float **cluster_centres_gold) {
    int i, j;
    int errors=0;
    char error_detail[200];
    for (i=0; i< nclusters; i++) {
        for (j=0; j<numAttributes; j++)
            if(cluster_centres[i][j] != cluster_centres_gold[i][j]) {
                errors++;
                if  (errors < 1000) {
                    sprintf(error_detail," p: [%d,%d], r: %1.16e, e: %1.16e", i,j, cluster_centres[i][j], cluster_centres_gold[i][j]);
                    printf("#ERR %s\n",error_detail);
                }

            }
    }
    printf("#SDC Ite:1 KerTime:0.0 AccTime:0.0 KerErr:%d AccErr:%d\n#END",errors, errors);
    return errors;
}

/*---< usage() >------------------------------------------------------------*/
void usage(char *argv0) {
    char *help =
        "Usage: %s [switches] -i filename\n"
        "       -i filename     		: file containing data to be clustered\n"
        "       -o output-filename     		: file containing the output result\n"
        "       -g gold-filename     		: file containing the gold result\n"
        "       -k                 	: number of clusters (default is 5) \n"
        "       -t threshold		: threshold value\n"
        "       -n no. of threads	: number of threads";
    "       -l no. of repeatitions (default is 1))";
    fprintf(stderr, help, argv0);
    exit(-1);
}

/*---< main() >-------------------------------------------------------------*/
int main(int argc, char **argv) {
    int     opt;
    extern char   *optarg;
    extern int     optind;
    int     nclusters=5;
    char   *filename = 0;
    char   *out_filename = 0;
    char   *gold_filename = 0;
    float  *buf;
    float **attributes;
    float **cluster_centres=NULL;
    float **cluster_centres_gold=NULL;
    int     i, j;
    int loop_iterations=1;

    int     numAttributes;
    int     numObjects;
    char    line[1024];
    int     nloops = 1;
    float   threshold = 0.001;
    double  timing;

    while ( (opt=getopt(argc,argv,"i:o:g:k:t:l:n:?"))!= EOF) {
        switch (opt) {
        case 'i':
            filename=optarg;
            break;
        case 'o':
            out_filename=optarg;
            break;
        case 'g':
            gold_filename=optarg;
            break;
        case 't':
            threshold=atof(optarg);
            break;
        case 'k':
            nclusters = atoi(optarg);
            break;
        case 'n':
            num_omp_threads = atoi(optarg);
            break;
        case 'l':
            loop_iterations = atoi(optarg);
            break;
        case '?':
            usage(argv[0]);
            break;
        default:
            usage(argv[0]);
            break;
        }
    }


    if (filename == 0 || out_filename == 0 || gold_filename == 0) usage(argv[0]);

    numAttributes = numObjects = 0;

    /* from the input file, get the numAttributes and numObjects ------------*/
    FILE *infile;
    if ((infile = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error: no such file (%s)\n", filename);
        exit(1);
    }
    while (fgets(line, 1024, infile) != NULL)
        if (strtok(line, " \t\n") != 0)
            numObjects++;
    rewind(infile);
    while (fgets(line, 1024, infile) != NULL) {
        if (strtok(line, " \t\n") != 0) {
            /* ignore the id (first attribute): numAttributes = 1; */
            while (strtok(NULL, " ,\t\n") != NULL) numAttributes++;
            break;
        }
    }


    cluster_centres_gold    = (float**) malloc(nclusters *             sizeof(float*));
    cluster_centres    = (float**) malloc(nclusters *             sizeof(float*));
    for(i = 0; i < nclusters; i++) {
        cluster_centres_gold[i] = (float*)  malloc(nclusters * numAttributes * sizeof(float));
        cluster_centres[i] = (float*)  malloc(nclusters * numAttributes * sizeof(float));
    }

    printf("#HEADER filename:%s threshold:%f clusters:%d threads:%d\n",filename,threshold,nclusters,num_omp_threads);

    read_gold(nclusters, numAttributes, out_filename, cluster_centres);
    read_gold(nclusters, numAttributes, gold_filename, cluster_centres_gold);


    int errors = compare_gold(nclusters, numAttributes, cluster_centres, cluster_centres_gold);




    free(attributes);
    free(cluster_centres[0]);
    free(cluster_centres);
    free(buf);
    return(0);
}

