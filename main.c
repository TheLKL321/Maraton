/*	Autor: Łukasz Zarębski
*/

#include "movieTree.h" // already includes stdlib.h and stdio.h
#include <errno.h>
#include <limits.h>
#include <string.h>

// TODO: check 1337zlawartosc
unsigned long extractUnsignedLong (char string[]){
	char *endptr;
	errno = 0;
	unsigned long result = strtol(string, &endptr, 10);
	if ((result == 0 && endptr == string) || (result == ULONG_MAX && errno == ERANGE))
	    return -1;
	return result;
}

unsigned int extractUnsignedInt (char string[]){
	unsigned long fullValue = extractUnsignedLong(string);
	unsigned int result = (unsigned int) fullValue;
	if (fullValue != (unsigned long) -1 && fullValue == result) return result;
	else return -1;
}

long extractLong (char string[]){
	unsigned long fullValue = extractUnsignedLong(string);
	long result = (long) fullValue;
	if (fullValue != (unsigned long) -1 && fullValue == (unsigned long) result) return result;
	else return -1;
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

	} else if (strcmp(operation, "delUser")){
		unsigned int userId = extractUnsignedInt(firstArgument);
		if (userId != (unsigned int) -1){
			delUser(userId);
		} else err();

	} else if (strcmp(operation, "marathon")){
		unsigned int userId = extractUnsignedInt(firstArgument);
		long k = extractLong(strtok(NULL, " "));
		if (userId != (unsigned int) -1 && k != -1){
			Movie *resultList = marathon(userId, k);
			Movie **resultListPtr = &resultList;
			if (*resultListPtr){
				for (int i = 0; i < k; ++i){
					printf("%li\n", (*resultListPtr)->movieRating);
					resultListPtr = &((*resultListPtr)->nextMovie);
				}
			} else printf("%s\n", "NONE");
			delAllMovies(resultList);
		} else err();
	} else err();
}

int main() {

	createHost();

	// TODO: ignore # and \n

  	delAllUsers(0);

	return 0;
}