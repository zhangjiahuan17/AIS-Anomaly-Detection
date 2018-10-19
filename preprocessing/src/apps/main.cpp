#include <common/aisreader.h>

#include <iostream>

int main(int argc, char *argv[]) {

	AISReader ais(argv[1]);

	for (int i = 0; i < argc; ++i)
	    {
	    	std::cout << argv[i];
	    }    
	    std::cout << std::endl;

    return 0;
}
