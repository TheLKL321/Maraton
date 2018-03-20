#include <stdlib.h> // needed for NULL declaration

//TODO: divide the module
#ifndef USER_TREE_H_
#define USER_TREE_H_

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
	struct User *nextSibling, *firstKid, *parent;
};

struct User *userPointers[65535];

#endif 