#include "userTree.h"

typedef struct User User;
typedef struct Movie Movie;

struct Movie
{
	long movieRating;
	Movie *nextMovie;
};

struct User{
	unsigned short userId;
	Movie *firstMovie;
	User *nextSibling, *firstKid, *parent;
};