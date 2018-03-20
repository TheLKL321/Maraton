#include <stdlib.h> // needed for NULL declaration

//TODO: divide the module
#ifndef USER_TREE_H_
#define USER_TREE_H_

void err ();
void createHost();

void addMovie (unsigned int userId, long movieRating);
void delMovie (unsigned int userId, long movieRating);
void addUser (unsigned int parentUserId, unsigned int userId);
void delUser (unsigned int userId);
void marathon (unsigned int userId, long k);

#endif 