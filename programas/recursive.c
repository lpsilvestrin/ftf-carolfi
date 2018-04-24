#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * A;
char * B;
int m;
int n;

int lcs_length(char * AA, char * BB)
{
	A = AA; B = BB;

    m = strlen(A);
    n = strlen(B);
    printf("%d %d\n",m,n);

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
    if (A[i] == '\0' || B[j] == '\0') 
        return 0;
    else if (A[i] == B[j]) 
        return 1 + subproblem(i+1, j+1);
    else 
        return max(subproblem(i+1, j), subproblem(i, j+1));
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

int main(int argc, char* argv[]){
	if (argc < 4) {
		printf("args: FILE1 FILE2 OUTPUT_FILE");
		return 0;
	}
	A = read_file(argv[1]);
	B =	read_file(argv[2]);
	int lcs_len = lcs_length(A,B);
	FILE* out_file = fopen(argv[3], "w");
	fprintf(out_file, "%d", lcs_len);
    //printMatrix();
	fclose(out_file);
	return 0; 
	
}
