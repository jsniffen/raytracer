#include "Nff.h"

using namespace std;

void Nff::fanVertices(vector<SlVector3> &vertices){
	for(int i = 1; i < vertices.size() - 1; i++){
		polygons.push_back(Polygon());
		polygons[polygons.size() - 1].insertVertex(vertices[0]);
		polygons[polygons.size() - 1].insertVertex(vertices[i]);
		polygons[polygons.size() - 1].insertVertex(vertices[i + 1]);
		polygons[polygons.size() - 1].material = material;
	}
	vertices.clear();
}

Light::Light(){

}

Light::Light(double x, double y, double z, double r, double g, double b){
	position = SlVector3(x, y, z);
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

Light::Light(double x, double y, double z){
	position = SlVector3(x, y, z);
}
