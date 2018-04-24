#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

int* X;
int* Y;
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

    //allocate storage for one-dimensional arrays X and Y
    X = (int*) malloc((n+1)*sizeof(int));
    Y = (int*) malloc((n+1)*sizeof(int));

	for (i = m; i >= 0; i--)
	{
	    for (j = n; j >= 0; j--)
	    {
		    if (A[i] == '\0' || B[j] == '\0')
                X[j] = 0;
		    else if (A[i] == B[j])
                X[j] = 1 + Y[j+1];
		    else
                X[j] = max(Y[j], X[j+1]);
	    }
        // copy X to Y
        for (j = n; j >= 0; j--)
	        Y[j] = X[j];
	}

    return X[0];
}

int max(int tam, int tam2){
    if(tam > tam2)
        return tam;
    else
        return tam2;
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
