#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Consts
#define MAX_EXAMS 1024
#define BUFFER_SIZE 1024

//Filenames
#define SUB_FILENAME  "subjects.csv"
#define EXM_FILENAME  "exams.csv"


//I like having fake lag so the program doesn't whiplash me b/c it's so fast
#define FAKE_LAG_DEV  150000
#define FAKE_LAG_ADD  40000
#define FAKE_LAG_SPI  5
#define FAKE_LAG      1

//Lool
//Subject struct
typedef struct {
	char name[32];
	char code_name[16];
	char professor[32];
	int  ESPB;
}Subject;



//Exam struct
typedef struct {
	char    type[16];
       	int     date; 			//ddmmyy format
	char    subject_code[16];
	char    topic[32];
	int     max_points;
	int     points;
	int     passed;
}Exam;


//TODO
//Edit/remove subject/exam
//Print exams on printSubjects screen
//
//Error handling:
//Adding large strings doesn't overflow or segfaults, but it also doesn't save the strings
//First string read is not read for some reason


//Read/write files

int readSubjects(Subject *subjects);

int readExams(Exam *exams);

int writeSubjects(Subject *subjects);

int writeExams(Exam *exams);

//Sorting by date

int isLargerDate(int date, int compare);

int isSorted (Exam *exams);

void sortByDate(Exam *exams);	

//Input

int addExam(Exam *exams);

int addSubject(Subject *subjects);

char* readString(int max_size);

int readInt(int check_date);

void awaitReturn(char *message);

//Screen
//void printScreen(Exam *exams, Subject *subjects);

char* getDate(int date);

void printExams(Exam *exams, Subject *subjects);

void printSubjects(Exam *exams, Subject *subjects);

void clearScreen();

//Returns a command in char format
//h - help
//q - quit
//e - add exam
//s - add subject
//E - edit exam
//S - edit subject
//[ - print exams
//] - print subjects
//c - clear screen ?

char parseCommand();


int main(){

	Subject subjects[100];
	memset(subjects, 0, sizeof(subjects));
	
	Exam exams[100];
	memset(exams, 0, sizeof(exams));
	
	readExams(exams);
	readSubjects(subjects);
	
	sortByDate(exams);
	
	awaitReturn("Press return key to continue...");
	
	char main_loop = 'o';
	clearScreen();
	while (main_loop != 'q'){
		main_loop = parseCommand();			
	
		switch (main_loop){
			case 'e':
				printExams(exams,subjects);
				addExam(exams);
				break;
	
			case 's':
				printSubjects(exams,subjects);
				addSubject(subjects);
				break;
			
			case '[':
				printExams(exams,subjects);
				break;
			
			case ']':
				printSubjects(exams,subjects);
				break;


			case 'c':
				clearScreen();
				break;
	
		}	
	
	}
	sortByDate(exams);
	writeSubjects(subjects);
	writeExams(exams);
	clearScreen();
	return 0;
}



void awaitReturn(char *message){

	printf("%s\n", message);	
	while(fgetc(stdin) != '\n');

}

void clearScreen(){
	printf("\e[1;1H\e[2J");
	return;
}


//Returns a command in char format
//h - help
//q - quit
//e - add exam
//s - add subject
//E - edit exam
//S - edit subject
//[ - print exams
//] - print subjects
//c - clear screen ?
//o - blank

char parseCommand(){
	printf(">");
	char *command = readString(16);


	if (!strcmp(command, "help")) return 'h';
	
	if (!strcmp(command, "quit")) return 'q';
	
	if (!strcmp(command, "add exam")) return 'e';
	
	if (!strcmp(command, "add subject")) return 's';
	
	if (!strcmp(command, "edit exam")) return 'E';
	
	if (!strcmp(command, "edit subject")) return 'S';
		
	if (!strcmp(command, "print exams")) return '[';

	if (!strcmp(command, "print subjects")) return ']';

	if (!strcmp(command, "clear")) return 'c';


	printf("Unknown command: %s\n", command);
	return 'o';
}


char* getDate(int date){

	char *odate = malloc(9);
	odate[8] = '\0';

	for (int x = 7; x >= 0; x--){
		if (x == 2 || x == 5){
			odate[x] = ',';
		}else{
		
		odate[x] = (date % 10) + '0';
		date = date/10;
		
		}
	
	}
	return odate;

}

void printExams(Exam *exams, Subject *subjects){
	
	printf("----|%-17s|-|%-9s|-|%-17s|-|%-32s|-|%-4s|-|%-4s|-|%-4s|\n",
		        	"Type",		
                        	"Date",
                        	"Code",
                        	"Topic",
	                	"Max",
                        	"Pts",
                        	"P?");
	printf("---------------------------------------------------------------------------------------------------------------\n");
	char *temp;
	int curr = 0;
	while(exams[curr].date > 0){
		printf("%-3d-|%-17s|-|%-9s|-|%-17s|-|%-32s|-|%-4d|-|%-4d|-|%-4d|\n",
			   		curr + 1,
					exams[curr].type,         
                	        	temp = getDate(exams[curr].date),
                        		exams[curr].subject_code,
                        		exams[curr].topic,
	                		exams[curr].max_points,
                        		exams[curr].points,
                        		exams[curr].passed);
		curr++;
	}
	printf("---------------------------------------------------------------------------------------------------------------");
	printf("\n");
	if (curr != 0) free(temp);
}





void printSubjects(Exam *exams, Subject *subjects){
	
	printf("----|%-33s|-|%-17s|-|%-32s|-|%-4s|\n",	
				"Name",
				"Code name",
				"Professor",
				"ESPB");
	printf("-----------------------------------------------------------------------------------------------------\n");
	int curr = 0;
	while(subjects[curr].ESPB != 0){
		printf("%-3d-|%-33s|-|%-17s|-|%-32s|-|%-4d|\n",
					curr + 1,
					subjects[curr].name,					
					subjects[curr].code_name,				
					subjects[curr].professor,			
					subjects[curr].ESPB);			
				curr++;
	}
	printf("-----------------------------------------------------------------------------------------------------");
	printf("\n");
	return;

}

char* readString(int max_size){

	int err = 0;
	if (max_size < 1) max_size = 1;
	if (max_size > BUFFER_SIZE) max_size = BUFFER_SIZE;

	char out[max_size + 1];

	do{	
		err = 0;
		if (fgets(out, sizeof(out), stdin) != NULL){
		
			int length = strlen(out);
			if (length > 0 && out[length - 1] == '\n'){
				out[length - 1] = '\0';
			}
			else{
				printf("Max length of %d exceeded, press return to continue...", max_size);
				awaitReturn("\n");
				err = 1;
				out[0] = '\0';
			}
		}
	}while(err);
	
//	if (out[strlen(out) - 1] != '\0') out[strlen(out) - 1] = '\0';
	printf("Error: %d", err);	

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	char *output = out;
	printf("\n");
	return output;
} 


int readInt(int check_date){
	
	int err = 0;	
	int num = 0;

	do {
		err = 0;
		int res = scanf("%d", &num);
		//Error end of file
		if (res == EOF) {
			printf("Error reading integer, try again\n>");
			err = 1;
		}
		//Clear if no int is found
		if (res == 0) {
			while(fgetc(stdin) != '\n');
			printf("Error reading integer, try again\n>");
			err = 1;
		}
		//Check for valid date
		if (check_date){
			
			if (num < 100000 || num > 999999) err = 1;
	
			
			int year = num % 100;
			int month = (num / 100) % 100;
			int day = (num / 10000) % 100;
			int is_leap_year = (year % 4 == 0);
		    	printf("%d,%d,%d, leap year - %d  :", day, month, year, is_leap_year);	
			
			if (month > 12 || month < 1) err = 1; 							//Month over 12
			if (day > 31 || day < 1) err = 1;							//Day over 31
			if ((day > 29) && (month == 2)) err = 1; 						//Check february 
			if ((day == 29) && (month == 2) && !is_leap_year) err = 1; 				//Check leap february 
			if ((day == 31) && (month == 4 || month == 6 || month == 9 || month == 11)) err = 1;  	//Check day
			
			if (err) printf(" is invalid date, try again\n>");
				
		}
	
	}while (err);	
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("Read:%d\n", num);
	while(fgetc(stdin) != '\n');
	return num;

	

}

int addSubject(Subject *subjects){
	//Find space for the next exam
	int n = 0;
	while (subjects[n].ESPB != 0) n++;
	
	printf("Available subject slot: %d, press return to continue...\n", n + 1);
	while(fgetc(stdin) != '\n');

	
	printf("Enter the name of the subject (Max %d characters)\n>", sizeof(subjects[n].name));
	strcpy(subjects[n].name,readString(sizeof(subjects[n].name)));	
	
	printf("Enter the code name of the subject (Max %d characters)\n>", sizeof(subjects[n].code_name));
	strcpy(subjects[n].code_name,readString(sizeof(subjects[n].code_name)));

	printf("Enter the name of the professor of the subject (Max %d characters)\n>", sizeof(subjects[n].professor));
	strcpy(subjects[n].professor,readString(sizeof(subjects[n].professor)));
		
	printf("Enter subject ESPB)\n>");
	subjects[n].ESPB = readInt(0);
	
	return 0;
}

int addExam(Exam *exams){
	//Find space for the next exam
	int n = 0;
	while (exams[n].date != 0) n++;
	
	printf("Available exam slot: %d, press return to continue...\n", n + 1);
	while(fgetc(stdin) != '\n');
	
	printf("Enter the type of the exam: (Max %d characters)\n>", sizeof(exams[n].type));
	strcpy(exams[n].type,readString(sizeof(exams[n].type)));
		
	printf("Enter the date of the exam: (Format: ddmmyy)\n>");
	exams[n].date = readInt(1);
	
	printf("Enter the subject code of the exam: (Max %d characters)\n>", sizeof(exams[n].subject_code));
	strcpy(exams[n].subject_code,readString(sizeof(exams[n].subject_code)));

	printf("Enter the topic of the exam: (Max %d characters)\n>", sizeof(exams[n].topic));
	strcpy(exams[n].topic,readString(sizeof(exams[n].topic)));

	printf("Enter the max points possible for the exam\n>");
	exams[n].max_points = readInt(0);
		
	printf("Enter the achieved points of the exam\n>");
	exams[n].points = readInt(0);
	
	printf("Is the exam passed? yes = 1, no = 0\n>");
	exams[n].passed = readInt(0);
	



	return 0;
}


//Reads in the subjects from the file in the working dir, if the file is not
//present, creates the file in the working dir.
//Read file 0
//Created file 1
//Error creating file 2
int readSubjects(Subject *subjects){

	printf("Reading subjects...\n");
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

	//File exists and is opened
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("File opened success!\n");

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	//Trying to read the file
	printf("Trying to read file...\n");
	
	int read = 0;
	int records = 0;

	do
	{
		read = fscanf(file,
				"%31[^,],%7[^,],%31[^,],%d\n",
				subjects[records].name,
				subjects[records].code_name,
				subjects[records].professor,
				&subjects[records].ESPB);
		
		
		usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV * (records * 0.2)) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
		if (subjects[records].ESPB != 0){
			printf(">>Loaded subject %s\n", &subjects[records].name);
		}
		if (read == 4) records++;
		if (read != 4 && !feof(file)){
			printf("Error at reading line %d in file %s\n",
					records, SUB_FILENAME);
		}



	}while(!feof(file));
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("File read success!\nClosing file...\n");
	
	fclose(file);

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("Done!\n\n");
	return 0;
}

//Reads in the exams from the file in the working dir, if the file is not
//present, creates the file in the working dir.
//Read file 0
//Created file 1
//Error creating file 2
int readExams(Exam *exams){
	
	printf("Reading exams...\n");
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));

	//Try to open the file
	printf("Trying to open file...\n");

	FILE *file = fopen(EXM_FILENAME, "r");

	if (file == NULL) {

	//Create the file if it isn't in the dir

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	usleep((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI));

		printf("File not found!\nTrying to create a file...\n");
	
		file = fopen(EXM_FILENAME, "w");

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

	do
	{
		read = fscanf(file,
				"%15[^,],%d,%7[^,],%63[^,],%d,%d,%d\n",
		        	exams[records].type,         
                        	&exams[records].date,
                        	exams[records].subject_code,
                        	exams[records].topic,
	                	&exams[records].max_points,
                        	&exams[records].points,
                        	&exams[records].passed);


		usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV * (records * 0.2)) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	
		if (exams[records].max_points != 0){
			printf(">>Loaded %s exam\n", &exams[records].subject_code);
		}
		if (read == 7) records++;
		if (read != 7 && !feof(file)){
			printf("Error at reading line %d in file %s\n",
					records, EXM_FILENAME);
		}



	}while(!feof(file));
	
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	
	printf("File read success!\nClosing file...\n");
	
	fclose(file);

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	
	printf("Done!\n\n");
	return 0;
}

//Writes subjects to the working directory
//Success 0
//Error 1
//File error 2
int writeSubjects(Subject *subjects){

	printf("Trying to write subjects...\n");
	
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	FILE *file = fopen(SUB_FILENAME, "w");
	
	if (file == NULL) {
		printf("Error openning file for write\n");
		return 2;
	}	
	if (subjects[0].ESPB == 0){
		printf("There are no subjects\n");
		return 1;
	
	}
	for(int x = 0; subjects[x].ESPB != 0; x++){
		fprintf(file,"%s,%s,%s,%d\n",
			       subjects[x].name, 
			       subjects[x].code_name, 
			       subjects[x].professor, 
			       subjects[x].ESPB);	
		
		printf("Written subject %s\n", subjects[x].code_name);
		usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	}


	printf("File written successfully!\nDone!\n");
	fclose(file);	

	return 0;
}

int writeExams(Exam *exams){

	printf("Trying to write exams...\n");
	
		
	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	FILE *file = fopen(EXM_FILENAME, "w");
	
	if (file == NULL) {
		printf("Error openning file for write\n");
		return 2;
	}	
	if (exams[0].date == 0){
		printf("There are no exams\n");
		return 1;
	
	}

	for(int x = 0; exams[x].date != 0; x++){
		
		if (!strcmp(exams[x].type, "")){
			printf("Error, type of exam lost, auto-correcting...\n");
			strcpy(exams[x].type, "Err");
		}//TODO fix this shit
		
		fprintf(file,"%s,%d,%s,%s,%d,%d,%d\n",
			       	exams[x].type,
			       	exams[x].date,
			       	exams[x].subject_code,
			       	exams[x].topic,
			       	exams[x].max_points,
			       	exams[x].points,
			       	exams[x].passed);	
		
		
		printf("Written exam %s, on %d\n", exams[x].type, exams[x].date);
	
		usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	}


	printf("File written successfully!\nDone!\n");
	fclose(file);	

	return 0;
}

int isLargerDate(int date, int compare){

	printf("Comparing...\n");

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)) /10 );
	
	//By year	
	if (date % 100 < compare % 100) return 0;
	if (date % 100 > compare % 100) return 1;
	//By month
	if (date % 10000 < compare % 10000) return 0;
	if (date % 10000 > compare % 10000) return 1;
	//By day
	if (date % 1000000 < compare % 1000000) return 0;
	if (date % 1000000 > compare % 1000000) return 1;
	
	return 0;	

}

int isSorted (Exam *exams){
	int current = 0;
	while (exams[current+1].date != 0){
		if (isLargerDate(exams[current].date, exams[current+1].date)){
			printf("Sorting...\n");
			usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
		 	return 0;
	
		}
		current++;
	}

	usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
	printf("Sorted!\n");
	return 1;
}

void sortByDate(Exam *exams){
	Exam temp;
	int n = 0;

	while(!isSorted(exams)){	
		while (exams[n+1].date != 0){
			if (isLargerDate(exams[n].date, exams[n+1].date)){
				printf("Swapping...\n");
				temp = exams[n];
				exams[n] = exams[n+1];
				exams[n+1] = temp;

				usleep(FAKE_LAG * ((rand() % FAKE_LAG_DEV) + FAKE_LAG_ADD * (rand() % FAKE_LAG_SPI)));
			}
			n++;	
		}
		n=0;	
	}
	printf("Sorted? %d\n", isSorted(exams));
}
