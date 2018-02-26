#include <stdio.h>
#include <unistd.h>

#define NAME "Artsem Holdvekht"

int main(){

	char str[100];
	int buffer;

	buffer = sprintf(str, "Hello CSC415, I am %s!\n", NAME);
	write(1, str, buffer);	
	
	return 0;
}
