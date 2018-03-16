#include "userTree.h"

typedef struct User User;
typedef struct Movie Movie;

struct Movie
{
	int movieRating;
	Movie *nextMovie;
};

struct User{
	int userId;
	Movie *firstMovie;
	User *nextSibling, *firstKid;
};