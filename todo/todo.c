/*
 * This program is my first project in C. It represents a to-do list.
 * You can add maximum of 11 items to the list.
 * Each item has a title, description and a priority. 
 * When outputing the list the items are ordered based on priority.
 * You can add and remove tasks with simple terminal commands.
 * 
 * The file is formated:
 * 1 number_of_elements
 * 2 Title1 Description1 Priority1
 * 3 TItle2 Description2 Priority2
 * etc..
 *
 *
 *
 *
 * Made by Shoof_
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TASKS 11
#define FILE_NAME "task_data.txt"
char NEWLINE_CHAR[] = "~";


#define MAX_STDIN  1024



int number_of_elements = -1;





#define MAX_TITLE 29
#define MAX_DESC 1024
typedef struct Task{
	char title[MAX_TITLE];
	char desc[MAX_DESC];
	int priority;
}Task;


void clearInputBuffer() {
	
	int c;
//	if ((c = getchar()) == EOF) return;
	while ((c = getchar()) != '\n' && c != EOF);
}




// Reads a file into an array passed by reference
// returns:
// 	-1: error, file could not be created inside of the initial setup run or error when reading the file.
// 	 1: initial setup success, file initialized.
// 	 0: file exists, successfully read.
// I know I shouldn't, but for now we trust the file input.
int readFile(struct Task *tasks){
	
	//Try to read file.
	FILE* file = fopen(FILE_NAME, "r");
	
	if (file == NULL){
		//If this is the first time the program has been run, init the file.
		printf("File %s not found, creating...\n", FILE_NAME);

		//Create the file.
		file = fopen(FILE_NAME, "w");
		if (file == NULL){
			//Unable to create file.
			printf("File %s cannot be created, initial setup failed, exiting...\n", FILE_NAME);
			fclose(file);
			return -1;

		}

		//File succesfully created, number of elements set to 0 (the first line of the file).
		fprintf(file, "%d\n", 0);
		number_of_elements = 0;
		fclose(file);
		return 1;
	
	}
	else{
		//The program has been run before, the file exists, and is oppened.
		//Reading file and updateing the array and number_of_elements over reference.
		if ((fscanf(file, "%d", &number_of_elements) == 1) && (number_of_elements >=  0)){
			printf("Reading file with %d elements\n", number_of_elements);
		}
		else{
			printf("Error reading file. Number of elements = %d\n", number_of_elements);
			fclose(file);
			return -1;
		}
		
			
		//Loading elements into the array.
		for (int i = 0; i < number_of_elements; i++){
			
			fscanf(file, " %19[^|\n]| %1023[^|\n]|%d", tasks[i].title, tasks[i].desc, &tasks[i].priority);
			tasks[i].title[strcspn(tasks[i].title, "\n")] = '\0';
			tasks[i].desc[strcspn(tasks[i].desc, "\n")] = '\0';
		}

		printf("Success! Loaded %d tasks\n", number_of_elements);
		fclose(file);
		return 0;
	}
	
}

int writeFile(struct Task *tasks){
	printf("\nTrying to write to file...");
	FILE* file = fopen(FILE_NAME, "w");

	if (file == NULL){
		printf("Error writing file.");
		fclose(file);
		return -1;
	}

	//Writing to file
	fprintf(file, "%d\n", number_of_elements);

	for (int i = 0; i < number_of_elements; i++){
		fprintf(file, "%s|%s|%d\n", tasks[i].title, tasks[i].desc, tasks[i].priority);
	}

	printf("\nFile with %d elements saved to disk.", number_of_elements);
	return 0;
}

int addTask(struct Task *tasks){
	printf("Adding task...\n\n");
	
	if (number_of_elements == MAX_TASKS){
		printf("Cannot add, maximum reached.\n");
		return -1;
	}
	char buff[MAX_STDIN];
	printf("Enter title, up to %d characters\n>", MAX_TITLE - 1);
	fgets(buff, sizeof(buff), stdin);
	buff[strcspn(buff,"\n")] = '\0';
	strncpy(tasks[number_of_elements].title, buff, MAX_TITLE - 1);
	tasks[number_of_elements].title[MAX_TITLE - 1] = '\0';

	memset(buff, sizeof(buff), 0);

	printf("Enter description. Up to %d characters\n\tuse %s to add newlines\n>", MAX_DESC - 1, NEWLINE_CHAR);
	fgets(buff, sizeof(buff), stdin);
	buff[strcspn(buff, "\n")] = '\0';
	strncpy(tasks[number_of_elements].desc, buff, MAX_DESC);
	tasks[number_of_elements].desc[MAX_DESC - 1] = '\0';

	
	int priority;
	printf("Enter the task priority, 0 is first priority, tasks can share priority.:\n");
	scanf("%d", &priority);
	tasks[number_of_elements].priority =  priority;


	number_of_elements++;
	printf("Adding task successful.\n");
	
}

int removeTask(struct Task *tasks){
	int num;

	printf("Enter the ID of the task to remove\n:");
	scanf("%d", &num);


	if (num < 0 || num > number_of_elements - 1){
		printf("Cannot remove. No task with id %d", num);
		return -1;
	}
	
	for (int i = num; i < number_of_elements; i++){
		tasks[i] = tasks[i+1];
	}
	number_of_elements--;
	
	return 0;
}


//Returns 0 when sorted
int isSorted(struct Task *tasks){
	for (int i = 0; i < number_of_elements - 1; i++){
		if (tasks[i].priority > tasks[i+1].priority) return 0;
	}
	return 1;
}

int sortPriority(struct Task *tasks){
	while(!isSorted(tasks)){
		for (int i = 0; i < number_of_elements - 1; i++){
			if (tasks[i].priority > tasks[i+1].priority){
				int temp = tasks[i].priority;
				tasks[i].priority = tasks[i+1].priority;
				tasks[i+1].priority = temp;
			}
		}
	}

	return 0;

}


int drawTasks(struct Task *tasks){
	for (int x = 0; x < 50; x++) printf("#");
	printf("\n# ID\t#  Title\t\t\t# Priority\n");
	for (int x = 0; x < 50; x++) printf("-");
	for (int i = 0; i < number_of_elements; i++){
		printf("\n# %d\t#  %s", i, tasks[i].title);
		if (strlen(tasks[i].title) + 3 > 8*3) printf("\t");
		for (int y = 0; y < 2 - (strlen(tasks[i].title - 5)) / 8; y++) printf("\t");
		printf("#  %d", tasks[i].priority);
	}

	printf("\n");
	for (int x = 0; x < 50; x++) printf("#");
	printf("\n");
	
}

int inspect(struct Task *tasks){
	int num;

	printf("Enter the ID of the task you wish to inspect\n:");
	scanf("%d", &num);
	
	printf("\033[2J\033[H");	
	
	char descBuff[MAX_DESC];
	strcpy(descBuff, tasks[num].desc);
	for(int i = 0; i < strlen(descBuff); i++){ 
		if (!strstr(descBuff, NEWLINE_CHAR)) break;
		descBuff[strcspn(descBuff, NEWLINE_CHAR)] = '\n';	
	}

	printf("Task: %s\n\n\t%s\n\n\t\tPress enter to continue...", tasks[num].title, descBuff);
	
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	getchar();
	return 0;
	
}

void changePriority(struct Task *tasks){
	int num, set;
	printf("Task ID:\n");
	scanf("%d", &num);
	
	printf("New priority:\n");
	scanf("%d", &set);

	tasks[num].priority = set;

}

int main(){
	struct Task tasks[MAX_TASKS];
	memset(tasks, 0, sizeof(tasks));
	
	printf("\033[2J\033[H");	
	readFile(tasks);
	char input = ' ';
	
	for(;;){
	
		sortPriority(tasks);	
		drawTasks(tasks);
	
		printf("[A]dd [R]emove [I]nspect [C]hange_priority [Q]uit\n");
		scanf(" %c", &input);
		input = tolower(input);
		clearInputBuffer();
		switch(input){
			case 'q':
				goto exit_main_loop;
			case 'a':
				addTask(tasks);
				break;
			case 'r':
				removeTask(tasks);
				break;
			case 'i':
				inspect(tasks);
				break;
			case 'c':
				changePriority(tasks);
				break;
		
		}

	
	printf("\033[2J\033[H");	
	}
	exit_main_loop:
	
	
	writeFile(tasks);
	return 0;
	
}
