/*	Autor: Łukasz Zarębski
*/

#include "userTree.h" // already includes stdlib.h
#include <stdio.h>
#include <stdbool.h>

void err () {
	fprintf(stderr,"ERROR\n");
}

void ok () {
	printf("OK\n");
}

void addUser (unsigned int parentUserId, unsigned int userId) {

	if (!userPointers[userId])
	{
		User *parentUserPtr = userPointers[parentUserId];

		User *newUserPtr = (User*) calloc(1, sizeof(User));
		newUserPtr->userId = userId;
		newUserPtr->firstMovie = NULL;
		newUserPtr->nextSibling = parentUserPtr->firstKid;
		newUserPtr->previousSibling = NULL;
		newUserPtr->firstKid = NULL;
		newUserPtr->lastKid = NULL;
		newUserPtr->parentIfOnEdge = parentUserPtr;
		userPointers[userId] = newUserPtr;

		parentUserPtr->firstKid = newUserPtr;
		if (!parentUserPtr->lastKid) parentUserPtr->lastKid = newUserPtr;

		User *nextSiblingPtr = newUserPtr->nextSibling;
		if (nextSiblingPtr){
			nextSiblingPtr->previousSibling = newUserPtr;
			if (parentUserPtr->lastKid != nextSiblingPtr) nextSiblingPtr->parentIfOnEdge = NULL;
		}

		ok();
	} else err();
}

void addMovie (unsigned int userId, long movieRating) {

	if (userPointers[userId]){
		User *userPtr = userPointers[userId];
		Movie *moviePtr = (Movie*) calloc(1, sizeof(Movie));
		moviePtr->movieRating = movieRating;
		moviePtr->nextMovie = userPtr->firstMovie;

		userPtr->firstMovie = moviePtr;

		ok();
	} else err();
}

void delMovie (unsigned int userId, long movieRating) {

	User *userPtr = userPointers[userId];
	Movie *moviePtr = userPtr->firstMovie;
	if (!userPtr && moviePtr){
		if (moviePtr->movieRating == movieRating){
			userPtr->firstMovie = moviePtr->nextMovie;
			free(moviePtr);
			ok();
		} else {
			Movie *secondMoviePtr = moviePtr->nextMovie;
			while (secondMoviePtr){
				if (secondMoviePtr->movieRating == movieRating){
					moviePtr->nextMovie = secondMoviePtr->nextMovie;
					free(secondMoviePtr);
					ok();
					return;
				}
				moviePtr = secondMoviePtr;
				secondMoviePtr = secondMoviePtr->nextMovie;
			}
		}
	}
	err();
}

void delAllMovies (Movie *firstMovie){

	if (firstMovie->nextMovie){
		delAllMovies (firstMovie->nextMovie);
	}
	free(firstMovie);
}

void connectFirstKidToSiblings (User *userPtr, User *userFirstKidPtr){

	userFirstKidPtr->parentIfOnEdge = NULL;
	userFirstKidPtr->previousSibling = userPtr->previousSibling;
	userPtr->previousSibling->nextSibling = userFirstKidPtr;
}

void connectLastKidToSiblings (User *userPtr, User *userLastKidPtr){

	userLastKidPtr->parentIfOnEdge = NULL;
	userLastKidPtr->nextSibling = userPtr->nextSibling;
	userPtr->nextSibling->previousSibling = userLastKidPtr;
}

void connectFirstKidToParent (User *userFirstKidPtr, User *parentPtr){
	parentPtr->firstKid = userFirstKidPtr;
	userFirstKidPtr->parentIfOnEdge = parentPtr;
}

void connectLastKidToParent (User *userLastKidPtr, User *parentPtr){
	parentPtr->lastKid = userLastKidPtr;
	userLastKidPtr->parentIfOnEdge = parentPtr;
}

void delUser (unsigned int userId) {

	if (userId == 0) err();
	else {
		User *userPtr = userPointers[userId];
		delAllMovies(userPtr->firstMovie);

		User *parentPtr	= userPtr->parentIfOnEdge;
		User *userFirstKidPtr = userPtr->firstKid;
		User *userLastKidPtr = userPtr->lastKid;
		if (parentPtr){
			if (userPtr == parentPtr->firstKid){
				connectFirstKidToParent(userFirstKidPtr, parentPtr);
				if (userPtr == parentPtr->lastKid) connectLastKidToParent(userLastKidPtr, parentPtr);
				else connectLastKidToSiblings(userPtr, userLastKidPtr);
			} else {
				connectLastKidToParent(userLastKidPtr, parentPtr);
				connectFirstKidToSiblings(userPtr, userFirstKidPtr);
			}
		} else {
			connectFirstKidToSiblings(userPtr, userFirstKidPtr);
			connectLastKidToSiblings(userPtr, userLastKidPtr);
		}

		free(userPtr);
	}
}

unsigned int extractUnsignedInt (char string[]){

	char *trash;
	unsigned int result = (unsigned int) strtoul(string, &trash, 10);

	if (trash == string) return result;
	else return -1;
}

long extractLong (char string[]){

	char *trash;
	long result = (long) strtol(string, &trash, 10);

	if (trash == string) return result;
	else return -1;
}

void switchFunction (char operation[], char arguments[]){

}

void marathon (unsigned int userId, long k){

}

int main() {

	User *hostPtr = (User*) calloc(1, sizeof(User));
	hostPtr->userId = 0;
	hostPtr->firstMovie = NULL;
	hostPtr->nextSibling = NULL;
	hostPtr->previousSibling = NULL;
	hostPtr->firstKid = NULL;
	hostPtr->lastKid = NULL;
	hostPtr->parentIfOnEdge = NULL;
	userPointers[0] = hostPtr;


	// TODO: Handle a huuuuuge int
	// TODO: ignore # and \n

  	// TODO: free all the memory

	return 0;
}