#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

#include "World.h"
#include "Nff.h"

using namespace std;

void World::parseNff(string fileName){
	string line;
	string loopType;
	int numToLoop = 0;
	double rgb [3];
	vector<SlVector3> vertices;
	ifstream myfile(fileName.c_str());
	if (myfile.is_open()){
		while (getline(myfile,line)){
			//tokenize the line into a vector
			vector<string> tokens;
			istringstream iss(line);
			string token;
			while(getline(iss, token, ' ')){
				tokens.push_back(token);
			}

			if(numToLoop > 0 && loopType == "fan"){
				//splitting a polygon of into a fan of triangles
				vertices.push_back(SlVector3(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str())));
				numToLoop--;
				if(!numToLoop){
					nff.fanVertices(vertices);
				}
			}
			if(numToLoop > 0 && loopType == "polygon"){
				nff.polygons[nff.polygons.size() - 1].insertVertex(SlVector3(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str())));
				nff.polygons[nff.polygons.size() - 1].material = nff.material;
				numToLoop--;
			}else if(tokens[0] == "b"){
				//background color
				nff.b[0] = atof(tokens[1].c_str());
				nff.b[1] = atof(tokens[2].c_str());
				nff.b[2] = atof(tokens[3].c_str());
			}else if (tokens[0] == "from"){
				//from
				nff.from = SlVector3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			}else if(tokens[0] == "at"){
				//at
				nff.at = SlVector3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			}else if(tokens[0] == "up"){
				//up
				nff.up = SlVector3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			}else if(tokens[0] == "angle"){
				//angle
				nff.angle = atof(tokens[1].c_str());
			}else if(tokens[0] == "hither"){
				//hither
				nff.hither = atof(tokens[1].c_str());
			}else if(tokens[0] == "resolution"){
				//resolution
				nff.resolution[0] = atof(tokens[1].c_str());
				nff.resolution[1] = atof(tokens[2].c_str());
			}else if(tokens[0] == "l"){
				//light
				Light light;
				if(tokens.size() > 4){
					light = Light(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()), atof(tokens[6].c_str()));
				}else{
					light = Light(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
				}
				nff.lights.push_back(light);
			}else if(tokens[0] == "f"){
				//material
				rgb[0] = atof(tokens[1].c_str());
				rgb[1] = atof(tokens[2].c_str());
				rgb[2] = atof(tokens[3].c_str());
				nff.material = Material(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()), atof(tokens[6].c_str()), atof(tokens[7].c_str()), atof(tokens[8].c_str()));
			}else if(tokens[0] == "p" || tokens[0] == "pp"){
				//polygon
				if(atof(tokens[1].c_str()) == 3){
					//triangle
					numToLoop = atof(tokens[1].c_str());
					loopType = "polygon";
					nff.polygons.push_back(Polygon());
				}else{
					numToLoop = atof(tokens[1].c_str());
					loopType = "fan";
				}
			}
		}
		myfile.close();
	}
}

void World::color(SlVector3 rayDirection, SlVector3 from, double t0, double t1, double rgb[], int depth){
	//loop through each polygon
	HitRecord hit = HitRecord(999);
	for(int k = 0; k < nff.polygons.size(); k++){
		//calculate intersection and set hitrecord
		nff.polygons[k].intersect(rayDirection, from, t0, t1, hit);
	}

	//color in the pixels
	if(hit.intersected){
		//intensity of light
		double intensity = 1/sqrt(nff.lights.size());

		//for each light
		for(int k = 0; k < nff.lights.size(); k++){
			SlVector3 lightDir = nff.lights[k].position - hit.intersection;
			normalize(lightDir);
			SlVector3 viewDir = from - hit.intersection;
			normalize(viewDir);
			SlVector3 halfVector = (viewDir + lightDir);
			normalize(halfVector);

			//loop through each polygon
			bool shadow = false;
			for(int s = 0; s < nff.polygons.size(); s++){
				if(nff.polygons[s].shadow(lightDir, hit.intersection)){
					shadow = true;
				}
			}
			if(!shadow){
				double diffuse = hit.material.diffuse*fmax(0, dot(lightDir, hit.normal));
				double specular = hit.material.specular*pow(fmax(0, (dot(hit.normal, halfVector))), hit.material.shine);

				rgb[0] += hit.material.rgb[0]*intensity*diffuse;
				rgb[1] += hit.material.rgb[1]*intensity*diffuse;
				rgb[2] += hit.material.rgb[2]*intensity*diffuse;

				rgb[0] += intensity*specular;
				rgb[1] += intensity*specular;
				rgb[2] += intensity*specular;
			}
		}
		if(depth < 6 && hit.material.specular > 0){
			//calculate reflection direction
			SlVector3 viewDirection = hit.intersection - from;
			normalize(viewDirection);
			SlVector3 reflectionDirection = viewDirection - 2*dot(viewDirection, hit.normal)*hit.normal;
			normalize(reflectionDirection);

			//create a new blank color
			double newRgb [3] = {0, 0, 0};
			//find the new color with reflection direction
			color(reflectionDirection, hit.intersection, 1e-6, t1, newRgb, depth + 1);

			//add the reflection color*specular component
			rgb[0] += hit.material.specular*newRgb[0];
			rgb[1] += hit.material.specular*newRgb[1];
			rgb[2] += hit.material.specular*newRgb[2];
		}
	}else{
		rgb[0] += nff.b[0]; 
		rgb[1] += nff.b[1]; 
		rgb[2] += nff.b[2]; 
	}
}

void World::generateImage(string fileName){
	//calculate distance and height
	double distance = mag(nff.from - nff.at);
	double h = tan((nff.angle/2)*M_PI/180)*distance;

	//set l, r, b, t accordingly
	double l, r, b, t;
	l = -h;
	r = h;
	b = h;
	t = -h;

	//calculate basis coordinate system and normalize
	SlVector3 rayDirection, w, right, up;
	w = nff.from - nff.at;
	right = cross(nff.up, w);
	up = cross(w, right);
	normalize(w);
	normalize(up);
	normalize(right);

	double u,v;
	unsigned char pixels[nff.resolution[0]][nff.resolution[1]][3];

	//loop through each pixel
	for(int i = 0; i < nff.resolution[0]; i++){
		//calculate u
		u = l + (r - l)*(i + 0.5)/nff.resolution[0];
		for(int j = 0; j < nff.resolution[1]; j++){
			//caculate v
			v = b + (t - b)*(j + 0.5)/nff.resolution[1];

			//initialize pixels
			double rgb[3] = {0, 0, 0};

			//calculate ray
			rayDirection = -w*distance + right*u + up*v;
			normalize(rayDirection);

			color(rayDirection, nff.from, 0, 999, rgb, 0);

			pixels[j][i][0] = fmin(255, fmax(0, rgb[0]*255));
			pixels[j][i][1] = fmin(255, fmax(0, rgb[1]*255));
			pixels[j][i][2] = fmin(255, fmax(0, rgb[2]*255));
		}
	}

	FILE *f = fopen(fileName.c_str(), "wb");
	fprintf(f, "P6\n%d %d\n%d\n", nff.resolution[0], nff.resolution[1], 255);
	fwrite(pixels, 1, nff.resolution[0]*nff.resolution[1]*3, f);
	fclose(f);
}
