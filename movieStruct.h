#ifndef MOVIE_STRUCT_H_
#define MOVIE_STRUCT_H_

struct Movie;

typedef struct Movie Movie;

struct Movie
{
	long movieRating;
	struct Movie *nextMovie;
};

#endif 