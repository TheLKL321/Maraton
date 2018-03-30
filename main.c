/*	Autor: Łukasz Zarębski
*/

#include "movieTree.h" // already includes stdlib.h and stdio.h
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

unsigned long extractUnsignedLong (char string[]){
	char *endPtr;
	errno = 0;
	unsigned long result = strtol(string, &endPtr, 10);
	if ((result == 0 && endPtr == string) 
		|| (result == ULONG_MAX && errno == ERANGE) 
		|| (endPtr[0] != '\n' && endPtr[0] != ' ' && endPtr[0] != '\000'))
	    return -1;
	return result;
}

unsigned int extractUnsignedInt (char string[]){
	unsigned long fullValue = extractUnsignedLong(string);
	if (fullValue == (unsigned long) -1 || fullValue > 65535) return -1;
	else return (unsigned int) fullValue;
}

long extractLong (char string[]){
	unsigned long fullValue = extractUnsignedLong(string);
	if (fullValue == (unsigned long) -1 || fullValue > 2147483647) return -1;
	else return (long) fullValue;
}

void finishLine(){
	char x[1];
	fgets(x, 1, stdin);
	while (x[0] != '\n') fgets(x, 1, stdin);
}

bool ifContainsEndline(char string[], int size){
	bool result = false;
	for (int i = 0; i < size; ++i) result = result || string[i] == '\n';
	return result;
}

void switchFunction (char line[]){

	char *operation = strtok(line, " ");
	char *firstArgument = strtok(NULL, " ");

	if (strcmp(operation, "addMovie") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long movieRating = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && movieRating != -1){
			addMovie(userId, movieRating);
		} else err();

	} else if (strcmp(operation, "delMovie") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long movieRating = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && movieRating != -1){
			delMovie(userId, movieRating);
		} else err();

	} else if (strcmp(operation, "addUser") == 0){
		unsigned int parentUserId = extractUnsignedInt(firstArgument);
		unsigned int userId = extractUnsignedInt(strtok(NULL, " "));
		if (parentUserId != (unsigned int) -1 && userId != (unsigned int) -1){
			addUser(parentUserId, userId);
		} else err();

	} else if (strcmp(operation, "delUser") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		if (userId != (unsigned int) -1){
			delUser(userId);
		} else err();

	} else if (strcmp(operation, "marathon") == 0){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long k = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && k != -1){
			Movie *resultList = marathon(userId, k);
			Movie **resultListPtr = &resultList;
			if (*resultListPtr){
				for (int i = 0; i < k; ++i){
					printf("%li ", (*resultListPtr)->movieRating);
					resultListPtr = &((*resultListPtr)->nextMovie);
				}
			} else printf("%s\n", "NONE");
			delAllMovies(resultList);
		} else err();
	} else err();
}

int main() {

	createHost();

	char line[32];
	while (fgets(line, 32, stdin)) {
		if (ifContainsEndline(line, 32)){
	  		if (line[0] != '#' && line[0] != '\n') switchFunction(line);
		} else {
			err();
			finishLine();
	  	}
  	}

  	delAllUsers(0);

	return 0;
}