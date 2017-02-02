/*
* Author: Julian Sniffen
* Description:
* 	This file contains the polygon class that is used to represent a triangle for this project.
*	The polygon class has an intersect method which calculates whether the triangle intersects with a ray.
*/

#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>
#include "slVector.H"
#include "Material.h"
#include "HitRecord.h"

using namespace std;

class Polygon{
	public:

		/*
		* args: vertex - the vector representing a vertex in the triangle
		* description:
		* 	inserts the vertexes into the vertices object
		*/
		void insertVertex(SlVector3 vertex);

		/*
		* args: rayDirection - the direction of the ray vector, from - the origin of the ray vector
		* description:
		* 	detects whether the polygon intersects with the given ray
		*/
		double intersect(SlVector3 rayDirection, SlVector3 from, double t0, double t1, HitRecord &hit);

		/*
		* args: rayDirection - the direction of the ray vector, from - the origin of the ray vector
		* description:
		* 	detects whether the polygon intersects with the given ray
		*/
		bool shadow(SlVector3 rayDirection, SlVector3 from);

		vector<SlVector3> vertices;
		Material material;

		/*
		* description:
		* 	overloaded the << operator for easier debugging
		*/
		friend ostream& operator<<(ostream& os, const Polygon& polygon);
};

#endif
