#include "Material.h"

Material::Material(){

}

Material::Material(double r, double g, double b, double Kd, double Ks, double sh, double t, double iof){
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
	diffuse = Kd;
	specular = Ks;
	shine = sh;
	transmittance = t;
	indexOfRefraction = iof;
}
