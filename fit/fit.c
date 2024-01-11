/*
 * A fitness program. Helps you excercise while you program or whatever.
 * At the begining to calibrate it asks you to do some excercises to failure,
 * depending on excercises, it will slowly raise reps and sets over time.
 *
 * When started, the program starts a random timer. After the timer is up, a 
 * 2-10 min excercise is created randomly and printed to the user.
 * 
 * After an excercise, the work is recorded into a file and the excercises
 * are updated, such that user can improve over time.
 *
 * List of excercises: Pushups, Squats, Sit-ups, Streching.
 *
 * List can be expanded with user-added excercises.
 *
 * The files are formated like so:
 *
 * excercises.txt:
 * Excercise | Type | Difficulty | Record Reps
 *   ^string    ^string     ^int      ^int
 *
 *
 * progress.txt
 * Excersise | Reps | Time and Date
 *   ^string   ^int   ^string
 *
 *
 * Made by Shoof_
 */

#include<stdio.h>
#include<string.h>
#define MAX_EX 32
#define EX_FILE "excercises.txt"
#define STATS_FILE "stats.txt"

#define MAX_EX_NAME 64
#define MAX_TYPE 64
#define MAX_DIFF 10

/* 
 * An Excercise structure, holds the name of the excercise, its type, its difficulty
 * and users' record reps.
 */
struct Ex{
	char name[MAX_EX_NAME];
	char type[MAX_TYPE];
	int  diff;
	int  max_reps;
};

/* 
 * Reads the excercises from the file EX_FILE and returns them as an array of structs Ex.
 * In case that the program is being run first-time, it creates the files EX_FILE and STATS_FILE
 * and writes the default excercises into EX_FILE, and initializes STATS_FILE, then returns 
 * the array of structs Ex.
 */
 struct Ex* readExcercises(){
	//Check if file EX_FILE exists to be read, and initialize with excercises if not.
	
	FILE* ex_file = fopen(EX_FILE, "r");
	
	if (ex_file == NULL){
		printf("\nFile %s not found, Initializing...", EX_FILE);
		

		//Load defaults into the file, then read from it.
		ex_file = fopen(EX_FILE, "w");
		fprintf(ex_file, "Pushups, Arms/back, 7, 0\nSitups, Abs, 4, 0\nSquats, Legs, 4, 0\nStretching, Stretching, 0, 0");

	}
	printf("\nFile %s exist, trying to read...", EX_FILE);
	//Free file so we can read from it.
	
	fclose(ex_file);
	ex_file = fopen(EX_FILE, "r");

	struct Ex excercises[MAX_EX];
	struct Ex excercise;
	for(int i = 0; i < MAX_EX; i++){
		memset(&excercise, 0, sizeof(excercise));
		fscanf(ex_file, "%63[^|\n] | %63[^|\n] | %d | %d", excercise.name, excercise.type, excercise.diff, excercise.max_reps);
		if (strlen(excercise.name) < 1) break;
	}	




	
		
	//Check if file STATS_FILE exist and initialize if neccessary.
	
	FILE* stats_file = fopen(STATS_FILE, "r");

	if (stats_file == NULL){
		printf("\nFile %s not found, Initializing...", STATS_FILE);
	}else{
		printf("\nFile %s exist, trying to read...", STATS_FILE);
		
		//Free file
		fclose(stats_file);
	}


	
}



int main(){
	readExcercises();




	return 0;
}
