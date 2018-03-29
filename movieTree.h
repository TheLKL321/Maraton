#include <stdlib.h> // needed for NULL declaration
#include <stdio.h>

#ifndef MOVIE_TREE_H_
#define MOVIE_TREE_H_

struct Movie;
struct User;

typedef struct User User;
typedef struct Movie Movie;

struct Movie
{
	long movieRating;
	struct Movie *nextMovie;
};

struct User{
	unsigned short userId;
	Movie *firstMovie;
	struct User *nextSiblingOrParent,		// Parent if this is a last kid
				*previousSiblingOrParent,	// Parent if this is a first kid
				*firstKid,
				*lastKid;
};

void err ();
void createHost ();
void delAllMovies (Movie *firstMovie);
void delAllUsers (unsigned int userId);

void addMovie (unsigned int userId, long movieRating);
void delMovie (unsigned int userId, long movieRating);
void addUser (unsigned int parentUserId, unsigned int userId);
void delUser (unsigned int userId);
Movie *marathon (unsigned int userId, long k);

#endif 