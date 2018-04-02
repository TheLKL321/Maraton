#include "userStruct.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef MOVIE_TREE_H_
#define MOVIE_TREE_H_

void err ();
void createHost ();

void addMovie (unsigned int userId, long movieRating);
void delMovie (unsigned int userId, long movieRating);
void delAllMovies (Movie *firstMovie);
void addUser (unsigned int parentUserId, unsigned int userId);
void delUser (unsigned int userId);
void delAllUsers(unsigned int userId);
Movie *marathon (unsigned int userId, long k);

#endif 