#ifndef HITRECORD_H
#define HITRECORD_H

#include "Material.h"
#include "slVector.H"

using namespace std;

class HitRecord {
	public:
		HitRecord();

		HitRecord(double intitialT);

		Material material;
		double t;
		bool intersected;
		SlVector3 intersection;
		SlVector3 normal;
};

#endif