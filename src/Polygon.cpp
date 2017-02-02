#include "Polygon.h"

void Polygon::insertVertex(SlVector3 vertex){
	vertices.push_back(vertex);	
}

double Polygon::intersect(SlVector3 rayDirection, SlVector3 from, double t0, double t1, HitRecord &hit){
	SlVector3 a, b, c;
	//set the edges of the triangle
	a = vertices[0];
	b = vertices[1];
	c = vertices[2];

	//calculate m
	double m, a1, a2, a3, b1, b2, b3, c1, c2, c3, i, j, k;
	/*
	| a1 a2 a3 | | beta  | = | i |
	| b1 b2 b3 | | gamma | = | j |
	| c1 c2 c3 | | t     | = | k |
	*/
	a1 = a.x() - b.x();
	a2 = a.x() - c.x();
	a3 = rayDirection.x();
	b1 = a.y() - b.y();
	b2 = a.y() - c.y();
	b3 = rayDirection.y();
	c1 = a.z() - b.z();
	c2 = a.z() - c.z();
	c3 = rayDirection.z();
	i = a.x() - from.x();
	j = a.y() - from.y();
	k = a.z() - from.z();

	m = (a1*((b2*c3)-(b3*c2))) + (b1*((a3*c2)-(a2*c3))) + (c1*((a2*b3)-(b2*a3)));

	//calculate gamma
	double gamma;
	/*
	| a1 i a3 |
	| b1 j b3 |
	| c1 k c3 |
	*/
	gamma = (c3*((a1*j)-(i*b1))) + (b3*((i*c1)-(a1*k))) + (a3*((b1*k)-(j*c1)));
	gamma = gamma/m;

	if(gamma < 0 || gamma > 1){
		return 0;
	}

	//calculate beta
	double beta;
	/*
	| i a2 a3 |
	| j b2 b3 |
	| k c2 c3 |
	*/
	beta = (i*((b2*c3)-(b3*c2))) + (j*((a3*c2)-(a2*c3))) + (k*((a2*b3)-(b2*a3)));
	beta = beta/m;

	if(beta < 0 || beta > (1 - gamma)){
		return 0;
	}

	//calculate t
	double t;
	/*
	| a1 a2 i |
	| b1 b2 j |
	| c1 c2 k |
	*/
	t = -((c2*((a1*j)-(i*b1))) + (b2*((i*c1)-(a1*k))) + (a2*((b1*k)-(j*c1))));
	t = t/m;

	//if intersection exists and is closer than the current intersection, replace the current intersection
	if(t > t0 && t < hit.t){
		hit.t = t;
		hit.material = material;
		hit.intersected = true;
		hit.intersection = from + rayDirection*t;
		hit.normal = cross((b-a),(c-a));
		normalize(hit.normal); 
	}
	return t;
}

bool Polygon::shadow(SlVector3 rayDirection, SlVector3 from){
	SlVector3 a, b, c;
	//set the edges of the triangle
	a = vertices[0];
	b = vertices[1];
	c = vertices[2];

	//calculate m
	double m, a1, a2, a3, b1, b2, b3, c1, c2, c3, i, j, k;
	/*
	| a1 a2 a3 | | beta  | = | i |
	| b1 b2 b3 | | gamma | = | j |
	| c1 c2 c3 | | t     | = | k |
	*/
	a1 = a.x() - b.x();
	a2 = a.x() - c.x();
	a3 = rayDirection.x();
	b1 = a.y() - b.y();
	b2 = a.y() - c.y();
	b3 = rayDirection.y();
	c1 = a.z() - b.z();
	c2 = a.z() - c.z();
	c3 = rayDirection.z();
	i = a.x() - from.x();
	j = a.y() - from.y();
	k = a.z() - from.z();

	m = (a1*((b2*c3)-(b3*c2))) + (b1*((a3*c2)-(a2*c3))) + (c1*((a2*b3)-(b2*a3)));

	//calculate gamma
	double gamma;
	/*
	| a1 i a3 |
	| b1 j b3 |
	| c1 k c3 |
	*/
	gamma = (c3*((a1*j)-(i*b1))) + (b3*((i*c1)-(a1*k))) + (a3*((b1*k)-(j*c1)));
	gamma = gamma/m;

	if(gamma < 0 || gamma > 1){
		return false;
	}

	//calculate beta
	double beta;
	/*
	| i a2 a3 |
	| j b2 b3 |
	| k c2 c3 |
	*/
	beta = (i*((b2*c3)-(b3*c2))) + (j*((a3*c2)-(a2*c3))) + (k*((a2*b3)-(b2*a3)));
	beta = beta/m;

	if(beta < 0 || beta > (1 - gamma)){
		return false;
	}

	//calculate t
	double t;
	/*
	| a1 a2 i |
	| b1 b2 j |
	| c1 c2 k |
	*/
	t = -((c2*((a1*j)-(i*b1))) + (b2*((i*c1)-(a1*k))) + (a2*((b1*k)-(j*c1))));
	t = t/m;

	if(t > 1e-6){
		return true;
	}else{
		return false;
	}
}

ostream& operator<<(ostream& os, const Polygon& polygon)
{
    os << polygon.vertices[0] << endl;
    os << polygon.vertices[1] << endl;
    os << polygon.vertices[2] << endl;
    os << "color: " << polygon.material.rgb[0] << ' ' << polygon.material.rgb[1] << ' ' << polygon.material.rgb[2];
    return os;
}