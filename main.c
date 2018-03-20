/*	Autor: Łukasz Zarębski
*/

#include "movieTree.h" // already includes stdlib.h and stdio.h

unsigned int extractUnsignedInt (char string[]){

	char *trash;
	unsigned int result = (unsigned int) strtoul(string, &trash, 10);

	if (trash == string) return result;
	else return -1;
}

long extractLong (char string[]){

	char *trash;
	long result = (long) strtol(string, &trash, 10);

	if (trash == string) return result;
	else return -1;
}

void switchFunction (char operation[], char arguments[]){

}

int main() {

	createHost();


	// TODO: Handle a huuuuuge int
	// TODO: ignore # and \n

  	// TODO: free all the memory

	return 0;
}