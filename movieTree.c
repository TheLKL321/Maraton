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
	hostPtr->firstMovie = NULL;
	hostPtr->nextSiblingOrParent = NULL;
	hostPtr->previousSiblingOrParent = NULL;
	hostPtr->firstKid = NULL;
	hostPtr->lastKid = NULL;
	userPointers[0] = hostPtr;
}

static unsigned int countKids (unsigned int userId){
	User *userPtr = userPointers[userId];
	unsigned int result = 0;

	User *tempKidPtr = userPtr->firstKid;
	if (tempKidPtr){
		while(tempKidPtr != userPtr){
			result++;
			tempKidPtr = tempKidPtr->nextSiblingOrParent;
		}
	}

	return result;
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
			if (!nextMoviePtr || nextMoviePtr->movieRating != movieRating){
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

	if (userPtr && moviePtr){
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
	} else err();
}

void addUser (unsigned int parentUserId, unsigned int userId) {
	if (!userPointers[userId] && userPointers[parentUserId])
	{
		User *parentPtr = userPointers[parentUserId];

		User *newUserPtr = (User*) calloc(1, sizeof(User));
		newUserPtr->userId = userId;
		newUserPtr->firstMovie = NULL;
		newUserPtr->previousSiblingOrParent = parentPtr;
		newUserPtr->firstKid = NULL;
		newUserPtr->lastKid = NULL;
		userPointers[userId] = newUserPtr;

		User *firstKidPtr = parentPtr->firstKid;
		if (firstKidPtr){
			newUserPtr->nextSiblingOrParent = firstKidPtr;
			firstKidPtr->previousSiblingOrParent = newUserPtr;
		} else {
			parentPtr->lastKid = newUserPtr;
			newUserPtr->nextSiblingOrParent = parentPtr;
		}

		parentPtr->firstKid = newUserPtr;

		ok();
	} else err();
}

void delAllMovies (Movie *firstMovie){
	if (firstMovie){
		if (firstMovie->nextMovie){
			delAllMovies (firstMovie->nextMovie);
		}
		free(firstMovie);
	}
}

void delUser (unsigned int userId) {

	if (userId != 0 && userPointers[userId]){
		User *userPtr = userPointers[userId];
		User *leftUserPtr = userPtr->previousSiblingOrParent;
		User *rightUserPtr = userPtr->nextSiblingOrParent;
		User *firstKidPtr = userPtr->firstKid;
		User *lastKidPtr = userPtr->lastKid;
		delAllMovies(userPtr->firstMovie);

		if (firstKidPtr){
			firstKidPtr->previousSiblingOrParent = leftUserPtr;
			lastKidPtr->nextSiblingOrParent = rightUserPtr;
		}

		if (leftUserPtr->firstKid == userPtr) leftUserPtr->firstKid = firstKidPtr;
		else leftUserPtr->nextSiblingOrParent = firstKidPtr;

		if (rightUserPtr->lastKid == userPtr) rightUserPtr->lastKid = lastKidPtr;
		else rightUserPtr->previousSiblingOrParent = lastKidPtr;

		userPointers[userId] = NULL;
		free(userPtr);
		ok();
	} else err();
}

void delAllUsers(unsigned int userId){
	User *user = userPointers[userId];

	delAllMovies(user->firstMovie);

	User *firstKid = user->firstKid;
	if (firstKid){
		User *secondKid = firstKid->nextSiblingOrParent;

		while(secondKid != user){
			delAllUsers(firstKid->userId);
			firstKid = secondKid;
			secondKid = firstKid->nextSiblingOrParent;
		}
		delAllUsers(firstKid->userId);
	}

	free(user);
}

Movie *marathon (unsigned int userId, long k){
	User *userPtr = userPointers[userId];

	Movie *resultMovieListStart = (Movie*) calloc(1, sizeof(Movie)); // first movie is a dummy movie
	resultMovieListStart->movieRating = -1;
	resultMovieListStart->nextMovie = NULL;
	Movie *resultMovieListEnd = resultMovieListStart;

	unsigned int kidCount = countKids(userId);
	Movie *movieLists[kidCount]; // an array of pointers to a current pointer to movie

	User *tempKidPtr = userPtr->firstKid;
	for (unsigned int i = 0; i < kidCount; ++i){
		movieLists[i] = marathon(tempKidPtr->userId, k);
		tempKidPtr = tempKidPtr->nextSiblingOrParent;
		Movie *tempMoviePtr = movieLists[i];
		movieLists[i] = movieLists[i]->nextMovie;
		free(tempMoviePtr);
	}

	Movie *userMovieList = userPtr->firstMovie;
	long highestRating;
	if (userMovieList) highestRating = (userMovieList)->movieRating;
	else highestRating = 0;

	for (long i = 0; i < k; ++i){

		unsigned int maxI;
		long maxRating = -1;
		for (unsigned int j = 0; j < kidCount; ++j){
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
			Movie *bestMoviePtr = movieLists[maxI];
			movieLists[maxI] = movieLists[maxI]->nextMovie;
			bestMoviePtr->nextMovie = NULL;
			resultMovieListEnd->nextMovie = bestMoviePtr;
			resultMovieListEnd = bestMoviePtr;
		} else if (userMovieList){
			while(userMovieList && i < k){
				Movie *bestMoviePtr = (Movie*) calloc(1, sizeof(Movie));
				*bestMoviePtr = *userMovieList;
				userMovieList = userMovieList->nextMovie;
				bestMoviePtr->nextMovie = NULL;
				resultMovieListEnd->nextMovie = bestMoviePtr;
				resultMovieListEnd = bestMoviePtr;
				i++;
			}
		} else i = k;
	}
	

	for (unsigned int i = 0; i < kidCount; ++i){
		delAllMovies(movieLists[i]);
	}
	
	return resultMovieListStart;
}