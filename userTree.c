#include "userTree.h"

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