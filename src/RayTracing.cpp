/*
* Author: Julian Sniffen
* Description:
*	This file contains the main function for this project.
*/

#include <iostream>
#include <vector>
#include "World.h"
#include "Nff.h"

using namespace std;

int main(int argc, char* argv[]){
	World world;

	//ensure the program is run with proper arguments
	if(argv[1] && argv[2]){
		world.parseNff(argv[1]);
		world.generateImage(argv[2]);
	}else{
		cout << "please run program with proper arguments!" << endl;
	}
	
	return 0;
}