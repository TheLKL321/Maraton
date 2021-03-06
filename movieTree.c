#include "movieTree.h"

static User *userPointers[65536] = { NULL };

void err () {
	fprintf(stderr,"ERROR\n");
}

static void ok () {
	printf("OK\n");
}

static void memExit (){
	delAllUsers(0);
	exit(1);
}

void createHost () {
	User *hostPtr = (User*) calloc(1, sizeof(User));
	if (!hostPtr)
		exit(1);
	hostPtr->userId = 0;
	userPointers[0] = hostPtr;
}

static unsigned int countKids (unsigned int userId) {
	User *userPtr = userPointers[userId];
	unsigned int result = 0;

	User *tempKidPtr = userPtr->firstKid;
	if (tempKidPtr)
		while(tempKidPtr != userPtr) {
			result++;
			tempKidPtr = tempKidPtr->nextSiblingOrParent;
		}

	return result;
}

// adds a new Movie at the start of a movie list and returns the new start of the list
static Movie *placeMovie (long movieRating, Movie *nextMoviePtr){
	Movie *moviePtr = (Movie*) calloc(1, sizeof(Movie));
	if (!moviePtr)
		memExit();
	
	moviePtr->movieRating = movieRating;
	moviePtr->nextMovie = nextMoviePtr;

	return moviePtr;
}

// adds a Movie at the end of a movie list and returns the new end of the list
static Movie *addToList(Movie *movieListEnd, Movie *moviePtr) {
	moviePtr->nextMovie = NULL;
	movieListEnd->nextMovie = moviePtr;
	return moviePtr;
}

void addMovie (unsigned int userId, long movieRating) {
	User *userPtr = userPointers[userId];

	if (userPtr) {
		Movie *tempMoviePtr = userPtr->firstMovie;
		if (!tempMoviePtr || tempMoviePtr->movieRating < movieRating) {
			userPtr->firstMovie = placeMovie(movieRating, tempMoviePtr);
			ok();
		} 
		else if (tempMoviePtr->movieRating != movieRating) {
			Movie *nextMoviePtr = tempMoviePtr->nextMovie;
			while (nextMoviePtr && nextMoviePtr->movieRating > movieRating) {
				tempMoviePtr = tempMoviePtr->nextMovie;
				nextMoviePtr = tempMoviePtr->nextMovie;
			}

			if (!nextMoviePtr || nextMoviePtr->movieRating != movieRating) {
				tempMoviePtr->nextMovie = placeMovie(movieRating, nextMoviePtr);
				ok();
			} 
			else err();

		} 
		else err();

	} 
	else err();
}

void delMovie (unsigned int userId, long movieRating) {
	User *userPtr = userPointers[userId];

	if (userPtr && userPtr->firstMovie) {
		Movie *moviePtr = userPtr->firstMovie;
		if (moviePtr->movieRating == movieRating) {
			userPtr->firstMovie = moviePtr->nextMovie;
			free(moviePtr);
			ok();
		} 
		else {
			Movie *secondMoviePtr = moviePtr->nextMovie;
			while (secondMoviePtr) {
				if (secondMoviePtr->movieRating == movieRating) {
					moviePtr->nextMovie = secondMoviePtr->nextMovie;
					free(secondMoviePtr);
					ok();
					return;
				}
				moviePtr = secondMoviePtr;
				secondMoviePtr = secondMoviePtr->nextMovie;
			}
			err();
		}
	} 
	else err();
}

void delAllMovies (Movie *firstMovie) {
	if (firstMovie) {
		delAllMovies(firstMovie->nextMovie);
		free(firstMovie);
	}
}

void addUser (unsigned int parentUserId, unsigned int userId) {
	if (!userPointers[userId] && userPointers[parentUserId]) {
		User *parentPtr = userPointers[parentUserId];

		User *newUserPtr = (User*) calloc(1, sizeof(User));
		if (!newUserPtr)
			memExit();
		newUserPtr->userId = userId;
		newUserPtr->previousSiblingOrParent = parentPtr;
		userPointers[userId] = newUserPtr;

		User *firstKidPtr = parentPtr->firstKid;
		if (firstKidPtr) {
			newUserPtr->nextSiblingOrParent = firstKidPtr;
			firstKidPtr->previousSiblingOrParent = newUserPtr;
		} 
		else {
			parentPtr->lastKid = newUserPtr;
			newUserPtr->nextSiblingOrParent = parentPtr;
		}

		parentPtr->firstKid = newUserPtr;

		ok();
	} 
	else err();
}

void delUser (unsigned int userId) {
	if (userId != 0 && userPointers[userId]) {
		User *userPtr = userPointers[userId];
		User *leftUserPtr = userPtr->previousSiblingOrParent;
		User *rightUserPtr = userPtr->nextSiblingOrParent;
		User *firstKidPtr = userPtr->firstKid;
		User *lastKidPtr = userPtr->lastKid;
		delAllMovies(userPtr->firstMovie);
		userPtr->firstMovie = NULL;

		if (firstKidPtr){
			firstKidPtr->previousSiblingOrParent = leftUserPtr;
			lastKidPtr->nextSiblingOrParent = rightUserPtr;
		} 
		else if (rightUserPtr != leftUserPtr) {
			firstKidPtr = rightUserPtr;
			lastKidPtr = leftUserPtr;
		}

		if (leftUserPtr->firstKid == userPtr) 
			leftUserPtr->firstKid = firstKidPtr;
		else 
			leftUserPtr->nextSiblingOrParent = firstKidPtr;

		if (rightUserPtr->lastKid == userPtr) 
			rightUserPtr->lastKid = lastKidPtr;
		else 
			rightUserPtr->previousSiblingOrParent = lastKidPtr;

		userPointers[userId] = NULL;
		free(userPtr);
		ok();
	} 
	else err();
}

void delAllUsers(unsigned int userId) {
	User *user = userPointers[userId];

	delAllMovies(user->firstMovie);

	User *firstKidPtr = user->firstKid;
	if (firstKidPtr) {
		User *secondKidPtr = firstKidPtr->nextSiblingOrParent;
		while(secondKidPtr != user) {
			delAllUsers(firstKidPtr->userId);
			firstKidPtr = secondKidPtr;
			secondKidPtr = firstKidPtr->nextSiblingOrParent;
		}
		delAllUsers(firstKidPtr->userId);
	}

	free(user);
}

// finds index of a movie list (from movieLists) which first movie has the 
// highest movieRating 
unsigned int getMaxI(Movie *movieLists[], unsigned int size) {
	unsigned int maxI = -1;
	long maxRating = -1;

	for (unsigned int j = 0; j < size; ++j) {
		Movie *tempMoviePtr;
		tempMoviePtr = movieLists[j];
		if (tempMoviePtr) {
			if (tempMoviePtr->movieRating == maxRating) {
				movieLists[j] = tempMoviePtr->nextMovie;
				free(tempMoviePtr);
			} 
			else if (tempMoviePtr->movieRating > maxRating) {
				maxRating = tempMoviePtr->movieRating;
				maxI = j;
			}
		}
	}

	return maxI;
}

Movie *marathon (unsigned int userId, long k) {
	User *userPtr = userPointers[userId];
	
	Movie *resultMovieListStart = (Movie*) calloc(1, sizeof(Movie)); 
	if (!resultMovieListStart)
		memExit();
	resultMovieListStart->movieRating = -1; // first movie is a dummy movie
	Movie *resultMovieListEnd = resultMovieListStart;

	if (!userPtr) {
		free(resultMovieListStart);
		return NULL;
	}

	unsigned int kidCount = countKids(userId);
	Movie *movieLists[kidCount]; // an array of pointers to a current pointer to movie

	User *tempKidPtr = userPtr->firstKid;
	for (unsigned int i = 0; i < kidCount; ++i) {
		movieLists[i] = marathon(tempKidPtr->userId, k);
		tempKidPtr = tempKidPtr->nextSiblingOrParent;
		Movie *tempMoviePtr = movieLists[i];
		movieLists[i] = movieLists[i]->nextMovie;
		free(tempMoviePtr);
	}

	Movie *userMovieList = userPtr->firstMovie;
	long highestRating;
	if (userMovieList)
		highestRating = (userMovieList)->movieRating;
	else 
		highestRating = -1;

	for (long i = 0; i < k; ++i) {

		unsigned int maxI = getMaxI(movieLists, kidCount);

		if (maxI != (unsigned int) -1 
			&& movieLists[maxI]->movieRating > highestRating) {

			Movie *bestMoviePtr = movieLists[maxI];
			movieLists[maxI] = movieLists[maxI]->nextMovie;
			resultMovieListEnd = addToList(resultMovieListEnd, bestMoviePtr);
		} 
		else if (userMovieList) {
			while(userMovieList && i < k) {
				Movie *bestMoviePtr = (Movie*) calloc(1, sizeof(Movie));
				if (!bestMoviePtr)
					memExit();
				*bestMoviePtr = *userMovieList;
				userMovieList = userMovieList->nextMovie;
				resultMovieListEnd = addToList(resultMovieListEnd, bestMoviePtr);
				i++;
			}
		} 
		else i = k;
	}
	
	for (unsigned int i = 0; i < kidCount; ++i) 
		delAllMovies(movieLists[i]);
	
	return resultMovieListStart;
}