#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define SUB_FILENAME  "subjects.csv"
#define EXM_FILENAME  "exams.csv"

#define FAKE_LAG_DEV  100000
#define FAKE_LAG_ADD  20000
#define FAKE_LAG_SPI  3
#define FAKE_LAG      1

typedef struct {
	char name[32];
	char code_name[8];
	char professor[32];
	int  ESPB;
}Subject;

typedef struct {
	char type[16];
	char date[16];
	Subject *subject;
	char topic[64];
	int  max_points;
	int  points;
	bool passed;
}Exam;



//Reads in the subjects from the file in the working dir, if the file is not
//present, creates the file in the working dir.
//Read file 0
//Created file 1
//Error creating file 2
int readSubjects(Subject *subjects){
	
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	//Try to open the file
	printf("Trying to open file...\n");

	FILE *file = fopen(SUB_FILENAME, "r");

	if (file == NULL) {

	//Create the file if it isn't in the dir

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	usleep((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI));
		printf("File not found!\nTrying to create a file...\n");
		file = fopen(SUB_FILENAME, "w");

		if (file == NULL){
			printf("Error creating the file!\n");
			fclose(file);
			return 2;
		}

		printf("File created success!\n");
		return 1;
		
	}
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("File opened success!\n");

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	//Trying to read the file
	printf("Trying to read file...\n");
	
	int read = 0;
	int records = 0;

	do{
		read = fscanf(file,
				"%31[^,],%7[^,],%31[^,],%d\n",
				subjects[records].name,
				subjects[records].code_name,
				subjects[records].professor,
				&subjects[records].ESPB);
		printf("Loaded subject %s\n", &subjects[records].name);

		usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
		if (read == 4) records++;
		if (read != 4 && !feof(file)){
			printf("Error at reading line %d in file %s",
					records, SUB_FILENAME);
		}



	}while(!feof(file));
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("File read success!\n");
	return 0;
}





int main(){
	
	Subject subjects[100];
	
	
	
	
	return readSubjects(subjects);
}
