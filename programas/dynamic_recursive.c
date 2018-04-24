#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

int** L;
char * A;
char * B;
int m;
int n;

int lcs_length(char * AA, char * BB)
{
	A = AA; B = BB;
	int i,j;

    m = strlen(A);
    n = strlen(B);
    printf("%d %d\n",m,n);

    //allocate storage for L;
    L = (int**) malloc((m+1)*sizeof(int*));
    for(i=0 ;i <= m; i++)
        L[i] = (int*) malloc((n+1)*sizeof(int));

	for (i = 0; i <= m; i++)
	    for (j = 0; j <= n; j++)
		    L[i][j] = -1;

	return subproblem(0, 0);
}

int max(int tam, int tam2){
	if(tam > tam2)
		return tam;
	else
		return tam2;
}

int subproblem(int i, int j)
{
	if (L[i][j] < 0) {
	    if (A[i] == '\0' || B[j] == '\0')
            L[i][j] = 0;
	    else if (A[i] == B[j])
            L[i][j] = 1 + subproblem(i+1, j+1);
	    else
            L[i][j] = max(subproblem(i+1, j), subproblem(i, j+1));
	}
	return L[i][j];
}

void printMatrix()
{
    int i, j;

    printf("  ");
    for (j = 0; j <= n; j++){
        printf(" %c ",B[j]);
    }
    printf("\n");

    for (i = 0; i <= m; i++){
        if(A[i] != '\0')
            printf("%c ",A[i]);
        else
            printf("  ");
        for (j = 0; j <= n; j++){
            printf("%2d ",L[i][j]);
        }
        printf("\n");
    }
}

void printSubsequence()
{
    int i, j;
    i = 0;
    j = 0;
    printf("Longest Common Subsequence:\n");
    while (i < m && j < n)
    {
	    if (A[i]==B[j])
	    {
	        printf("'%c',",A[i]);
	        i++; j++;
	    }
	    else if (L[i+1][j] >= L[i][j+1]) i++;
	    else j++;
    }
    printf("\n");
}

char* read_file(char* file_name) {
	FILE* fp = fopen(file_name, "r");
	if (fp == NULL)
		return NULL;
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = (char*)malloc(length*sizeof(char));
	fread(buffer, 1, length, fp);
	fclose(fp);
	return buffer;
}

void createDir(char* path) {
	struct stat st = {0};
	char *tmp = (char*)malloc(strlen(path)*sizeof(char));
	strcpy(tmp, path);
	dirname(tmp);
	if (stat(tmp, &st) == -1) {
		int err = mkdir(tmp, 0700);
		if (err == -1)
			printf("couldn't create dir %s", tmp);
	} 
}

int main(int argc, char* argv[]){
	if (argc < 4) {
		printf("args: FILE1 FILE2 OUTPUT_FILE");
		return 0;
	}
	A = read_file(argv[1]);
	B =	read_file(argv[2]);
	int lcs_len = lcs_length(A,B);
	createDir(argv[3]);
	FILE* out_file = fopen(argv[3], "w");
	int res = lcs_len;
	fprintf(out_file, "%d", res);
    //printMatrix();
	fclose(out_file);
	return 0; 
	
}
