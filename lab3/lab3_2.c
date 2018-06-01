#include <stdio.h>

int main() {
	FILE *infs_1, *infs_2, *outfs;
	char ch;
	char str1[1024], str2[1024];

	infs_1 = fopen("source1.txt", "r");
	infs_2 = fopen("source2.txt", "r");
	outfs = fopen("target_1.txt", "w");
	
	// fgetc & fputc
	ch = fgetc(infs_1);
	while(ch != EOF) {
		fputc(ch, outfs);
		ch = fgetc(infs_1);
	}
	fclose(outfs);

	// fgets & fputs
	outfs = fopen("target_2.txt", "w");
	fgets(str1, 1024, infs_2);
	printf("%s\n", str1);
	fputs(str1, outfs);
	fclose(outfs);

	// fscanf & fprintf
	fscanf(infs_2, "%s", str2);
	outfs = fopen("target_3.txt", "w");
	fprintf(outfs, "%s", str2);

}
