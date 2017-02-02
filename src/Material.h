/*
* Author: Julian Sniffen
* Description:
* 	This file contains the classes that are used to represent a parsed nff file
*/


#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
	public:
		Material();
		Material(double r, double b, double g, double Kd, double Ks, double sh, double t, double iof);
		double rgb [3];
		double diffuse;
		double specular;
		double shine;
		double transmittance;
		double indexOfRefraction;
};

#endif