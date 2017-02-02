/*
* Author: Julian Sniffen
* Description:
* 	This file contains the classes that are used to represent a parsed nff file
*/


#ifndef NFF_H
#define NFF_H

#include <vector>
#include "slVector.H"
#include "Polygon.h"
#include "Material.h"

using namespace std;

class Light {
	public:
		Light();
		Light(double x, double y, double z, double r, double g, double b);
		Light(double x, double y, double z);
		double rgb [3];
		SlVector3 position;
};

class Nff {
	public:
		double b [3];
		SlVector3 from;
		SlVector3 up;
		SlVector3 at;
		double angle;
		double hither;
		int resolution [2];
		vector <Light> lights;
		Material material;
		vector<Polygon> polygons;

		void fanVertices(vector<SlVector3> &vertices);
};

#endif
