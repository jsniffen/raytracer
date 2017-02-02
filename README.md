# RayTracer
A simple Ray Tracer

This project is a simple ray tracer written in c++ that parses a NFF file and produces the corresponding image in PPF format.

The program starts by parsing the given nff file and storing the necessary data into an object.
Next the program generates rays for every pixel in the image.  Each ray is then tested to see if
it intersects with any of the given polygons.  If there is an intersection, the pixel takes on the
the color of the polygon; if there is no intersection the pixel takes the background color.  Next the program
calculates reflections and shadows using mirror reflection and Blinn-Phong specular lighting.  Finally, the 
program outputs the new image into ppm format.


# How to Run
```
	'make'	# builds the project
	'make clean'	# removes the executable file 'bin/runner' and the build folder
	'make tetra'	# generates an output.ppm for tetra-3.nff
	'make tea'		# generates an output.ppm for teapot-3.nff
```

To Run Manually:
```
	'bin/runner example.nff output.ppm'		# this will generate an output.ppm from a file called example.nff
```
