/*
* Author: Julian Sniffen
* Description:
*	This file contains the world class, which is used to encapsulate the project and simplify the main function.
*/

#ifndef WORLD_H
#define WORLD_H

#include <string>
#include "Nff.h"

using namespace std;

class World {
	public:
		/*
		* args: fileName - the name of the file to parse.
		* description:
		* 	parses the file and stores the data in an nff object
		*/
		void parseNff(string fileName);

		/*
		* args: rayDirection - the direction of ray, from - the origin, t0 - hither, t1 - far, depth - number of possible recursions.
		* description:
		* 	returns the color for a ray
		*/
		void color(SlVector3 rayDirection, SlVector3 from, double t0, double t1, double rgb[], int depth);

		/*
		* args: fileName - the name of the file to output the ppm image to.
		* description:
		* 	generates an image from the nff data and outputs it to ppm format.
		*/
		void generateImage(string fileName);
	private:
		Nff nff;
};

#endif