/*	Autor: Łukasz Zarębski
*/

#include "userTree.h"
#include <stdio.h>

void err () {
	printf("%s\n", "ERROR");
}

void ok () {
	printf("%s\n", "OK");
}

void addUser (unsigned int parentUserId, unsigned int userId) {

	 User *parentUserPtr = userPointers[parentUserId];

	 User newUser = (User*) calloc(1, sizeof(User));
	 newUser.userId = userId;
	 newUser.firstMovie = NULL;
	 newUser.nextSibling = (*parentUserPtr).firstKid;
	 newUser.firstKid = NULL;
	 newUser.parent = parentUserPtr;
	 userPointers[userId] = &newUser;

	 (*parentUserPtr).firstKid = &newUser;
}

void delUser (unsigned int userId) {
	if (userId == 0) err();
	else {

	}
}

void addMovie (unsigned int userId, long movieRating) {
	
	User *user = userPointers[userId];
	Movie movie = (Movie*) calloc(1, sizeof(Movie));
	movie.movieRating = movieRating;
	movie.nextMovie = (*user).firstMovie;

	(*user).firstMovie = &movie;
}

void delMovie (unsigned int userId, long movieRating) {

}

void marathon (unsigned int userId, long k) {

}

int main () {

	User host = (User*) calloc(1, sizeof(User));
	host.userId = 0;
	host.firstMovie = NULL;
	host.nextSibling = NULL;
	host.firstKid = NULL;
	host.parent = NULL;
	userPointers[0] = &host;

	return 0;
}