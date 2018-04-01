//	Autor: Łukasz Zarębski

#include "movieTree.h" // already includes stdlib.h and stdio.h
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

unsigned long extractUnsignedLong (char string[]){
	char *endPtr;
	errno = 0;
	if (string == NULL 
		|| (strlen(string) > 2 && string[0] == '0' && string[1] == '0')) //handle 00032132 cases
		return -1;

	unsigned long result = strtol(string, &endPtr, 10);
	if ((result == 0 && endPtr == string) //check if anything could be read
		|| (result == ULONG_MAX && errno == ERANGE)  //prevent range errors
		|| (endPtr[0] != '\n' && endPtr[0] != '\000')) //handle 3213string cases
	    return -1;
	return result;
}

long extractLong (char string[]){
	unsigned long fullValue = extractUnsignedLong(string);
	if (fullValue == (unsigned long) -1 || fullValue > 2147483647) return -1;
	else return (long) fullValue;
}

unsigned int extractUnsignedInt (char string[]){
	long fullValue = extractLong(string);
	if (fullValue > 65535) return -1;
	else return (unsigned int) fullValue;
}

void finishLine(){
	char x = getchar();
	while (x != '\n') x = getchar();
}

// checks for length and double spaces
bool ifCorrect(char string[]){
	bool ifContainsEndline = false, ifContainsDoublespace = false;
	ifContainsEndline = ifContainsEndline || string[0] == '\n';
	for (int i = 1; i < (int) strlen(string); ++i){
		ifContainsEndline = ifContainsEndline || string[i] == '\n';
		ifContainsDoublespace = ifContainsDoublespace || (string[i - 1] == ' ' && string[i] == ' ');
	} 

	if (!ifContainsEndline) finishLine();

	if (string[0] == '#') return true;
	return ifContainsEndline && !ifContainsDoublespace;
}

void switchFunction (char line[]){

	char *operation = strtok(line, " ");
	char *firstArgument = strtok(NULL, " ");

	if (strcmp(operation, "addMovie") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long movieRating = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && movieRating != -1 && strtok(NULL, "") == NULL){
			addMovie(userId, movieRating);
		} else err();

	} else if (strcmp(operation, "delMovie") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long movieRating = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && movieRating != -1 && strtok(NULL, "") == NULL){
			delMovie(userId, movieRating);
		} else err();

	} else if (strcmp(operation, "addUser") == 0){
		unsigned int parentUserId = extractUnsignedInt(firstArgument);
		unsigned int userId = extractUnsignedInt(strtok(NULL, " "));
		if (parentUserId != (unsigned int) -1 && userId != (unsigned int) -1 && strtok(NULL, "") == NULL){
			addUser(parentUserId, userId);
		} else err();

	} else if (strcmp(operation, "delUser") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		if (userId != (unsigned int) -1 && strtok(NULL, "") == NULL){
			delUser(userId);
		} else err();

	} else if (strcmp(operation, "marathon") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long k = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && k != -1 && strtok(NULL, "") == NULL){
			Movie *resultListStart = marathon(userId, k);
			Movie *resultList = resultListStart;

			if (resultList->movieRating != -2) {
				resultList = resultList->nextMovie;
				if (resultList){
					long i = 0;
					while (resultList->nextMovie && i < k - 1){
						printf("%li ", resultList->movieRating);
						resultList = resultList->nextMovie;
						i++;
					}
					printf("%li\n", resultList->movieRating);
				} else printf("%s\n", "NONE");
				delAllMovies(resultListStart);
			} else err();
		} else err();
	} else err();
}

int main() {

	createHost();

	char line[32];
	while (fgets(line, 32, stdin)) {
		if (ifCorrect(line)){
	  		if (line[0] != '#' && line[0] != '\n') switchFunction(line);
		} else {
			err();
	  	}
  	}

  	delAllUsers(0);

	return 0;
}