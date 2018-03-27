#include "movieTree.h"

static User *userPointers[65536] = { NULL };

void err () {
	fprintf(stderr,"ERROR\n");
}

static void ok () {
	printf("OK\n");
}

void createHost (){
	User *hostPtr = (User*) calloc(1, sizeof(User));
	hostPtr->userId = 0;
	hostPtr->kidCount = 0;
	hostPtr->firstMovie = NULL;
	hostPtr->nextSibling = NULL;
	hostPtr->previousSibling = NULL;
	hostPtr->firstKid = NULL;
	hostPtr->lastKid = NULL;
	hostPtr->parentIfOnEdge = NULL;
	userPointers[0] = hostPtr;
}

void addMovie (unsigned int userId, long movieRating) {
	if (userPointers[userId]){
		User *userPtr = userPointers[userId];

		Movie *moviePtr = (Movie*) calloc(1, sizeof(Movie));
		moviePtr->movieRating = movieRating;

		Movie *tempMoviePtr = userPtr->firstMovie;
		if (!tempMoviePtr) {
			moviePtr->nextMovie = NULL;
			userPtr->firstMovie = moviePtr;
		} else if (tempMoviePtr->movieRating < movieRating){
			moviePtr->nextMovie = tempMoviePtr;
			userPtr->firstMovie = moviePtr;
		} else {
			Movie *nextMoviePtr = tempMoviePtr->nextMovie;
			while (nextMoviePtr && nextMoviePtr->movieRating > movieRating){
				tempMoviePtr = tempMoviePtr->nextMovie;
				nextMoviePtr = tempMoviePtr->nextMovie;
			}
			if (nextMoviePtr->movieRating != movieRating){
				tempMoviePtr->nextMovie = moviePtr;
				moviePtr->nextMovie = nextMoviePtr;
			}
		}

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

void addUser (unsigned int parentUserId, unsigned int userId) {
	if (!userPointers[userId])
	{
		User *parentPtr = userPointers[parentUserId];

		User *newUserPtr = (User*) calloc(1, sizeof(User));
		newUserPtr->userId = userId;
		newUserPtr->kidCount = 0;
		newUserPtr->firstMovie = NULL;
		newUserPtr->nextSibling = parentPtr->firstKid;
		newUserPtr->previousSibling = NULL;
		newUserPtr->firstKid = NULL;
		newUserPtr->lastKid = NULL;
		newUserPtr->parentIfOnEdge = parentPtr;
		userPointers[userId] = newUserPtr;

		parentPtr->kidCount++;
		parentPtr->firstKid = newUserPtr;
		if (!parentPtr->lastKid) parentPtr->lastKid = newUserPtr;

		User *nextSiblingPtr = newUserPtr->nextSibling;
		if (nextSiblingPtr){
			nextSiblingPtr->previousSibling = newUserPtr;
			if (parentPtr->lastKid != nextSiblingPtr) nextSiblingPtr->parentIfOnEdge = NULL;
		}

		ok();
	} else err();
}

void delAllMovies (Movie *firstMovie){
	if (firstMovie->nextMovie){
		delAllMovies (firstMovie->nextMovie);
	}
	free(firstMovie);
}

static void connectFirstKidToSiblings (User *userPtr, User *userFirstKidPtr){
	userFirstKidPtr->parentIfOnEdge = NULL;
	userFirstKidPtr->previousSibling = userPtr->previousSibling;
	userPtr->previousSibling->nextSibling = userFirstKidPtr;
}

static void connectLastKidToSiblings (User *userPtr, User *userLastKidPtr){
	userLastKidPtr->parentIfOnEdge = NULL;
	userLastKidPtr->nextSibling = userPtr->nextSibling;
	userPtr->nextSibling->previousSibling = userLastKidPtr;
}

static void connectFirstKidToParent (User *userFirstKidPtr, User *parentPtr){
	parentPtr->firstKid = userFirstKidPtr;
	userFirstKidPtr->parentIfOnEdge = parentPtr;
}

static void connectLastKidToParent (User *userLastKidPtr, User *parentPtr){
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

		parentPtr->kidCount--;
		userPointers[userId] = NULL;
		free(userPtr);
	}
}

void delAllUsers(unsigned int userId){
	User *user = userPointers[userId];

	delAllMovies(user->firstMovie);

	User *firstKid = user->firstKid;
	if (firstKid){
		User *secondKid = firstKid->nextSibling;

		while(secondKid){
			delAllUsers(firstKid->userId);
			firstKid = secondKid;
			secondKid = firstKid->nextSibling;
		}
		delAllUsers(firstKid->userId);
	}

	free(user);
}

Movie *marathon (unsigned int userId, long k){
	User *userPtr = userPointers[userId];
	Movie *movieLists[userPtr->kidCount]; // an array of pointers to a current pointer to movie
	Movie **resultMovieListPtr;

	User *tempKid = userPtr->firstKid;
	for (unsigned int i = 0; i < userPtr->kidCount; ++i){
		movieLists[i] = marathon(tempKid->userId, k);
		tempKid = tempKid->nextSibling;
	}

	Movie **userMovieListPtr = &(userPtr->firstMovie);
	long highestRating = (*userMovieListPtr)->movieRating;
	for (long i = 0; i < k; ++i){

		unsigned int maxI;
		long maxRating = -1;
		for (unsigned int j = 0; j < userPtr->kidCount; ++i){
			Movie *tempMoviePtr;
			tempMoviePtr = movieLists[j];
			if (tempMoviePtr){
				if (tempMoviePtr->movieRating == maxRating) {
					movieLists[j] = tempMoviePtr->nextMovie;
					free(tempMoviePtr);
				}
				else if (tempMoviePtr->movieRating > maxRating){
					maxRating = tempMoviePtr->movieRating;
					maxI = j;
				}
			}
		}

		if (maxRating > highestRating){
			Movie *bestMoviePtr;
			bestMoviePtr = movieLists[maxI];
			movieLists[maxI] = movieLists[maxI]->nextMovie;
			bestMoviePtr->nextMovie = *resultMovieListPtr;
			resultMovieListPtr = &bestMoviePtr;
		} else {
			while(*userMovieListPtr && i<k){
				Movie *bestMoviePtr = (Movie*) calloc(1, sizeof(Movie));
				*bestMoviePtr = **userMovieListPtr;
				bestMoviePtr->nextMovie = *resultMovieListPtr;
				resultMovieListPtr = &bestMoviePtr;
				userMovieListPtr = &((*userMovieListPtr)->nextMovie);
				i++;
			}
		}
	}

	for (unsigned int i = 0; i < userPtr->kidCount; ++i){
		delAllMovies(movieLists[i]);
	}
	
	return *resultMovieListPtr;
}