#include "movieStruct.h"

#ifndef USER_STRUCT_H_
#define USER_STRUCT_H_

struct User;

typedef struct User User;

struct User{
	unsigned short userId;
	Movie *firstMovie;
	struct User *nextSiblingOrParent,		// Parent if this is a last kid
				*previousSiblingOrParent,	// Parent if this is a first kid
				*firstKid,
				*lastKid;
};

#endif 