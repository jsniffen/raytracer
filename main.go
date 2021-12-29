package main

import (
	"fmt"
	"os"
	"strings"
	"strconv"
)

type Vector [3]float64

type Polygon struct {
	Vertices []Vector
}

type Material struct {
	Color Vector
	Diffuse float64
	Specular float64
	Shine float64
	Transmittance float64
	IndexOfRefraction float64
}

type Light struct {
	Position Vector
	Color Vector
}

type NFF struct {
	Background Vector
	From Vector
	At Vector
	Up Vector
	Angle float64
	Hither float64
	Resolution [2]uint64
	Lights []Light
	Material Material
	Polygons []Polygon
}

func parseNFF(s string) (*NFF, error) {
	nff := &NFF{}
	nff.Lights = make([]Light, 0)
	nff.Polygons = make([]Polygon, 0)
	var err error

	lines := strings.Split(s, "\n")

	for i := 0; i < len(lines); i++ {
		subs := strings.Split(lines[i], " ")

		if subs[0] == "b" {
			for i, f := range subs[1:] {
				nff.Background[i], err = strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}
			}
		} else if subs[0] == "from" {
			for i, f := range subs[1:] {
				nff.From[i], err = strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}
			}
		} else if subs[0] == "at" {
			for i, f := range subs[1:] {
				nff.At[i], err = strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}
			}
		} else if subs[0] == "up" {
			for i, f := range subs[1:] {
				nff.Up[i], err = strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}
			}
		} else if subs[0] == "angle" {
			nff.Angle, err = strconv.ParseFloat(subs[1], 64)
			if err != nil {
				return nff, err
			}
		} else if subs[0] == "hither" {
			nff.Hither, err = strconv.ParseFloat(subs[1], 64)
			if err != nil {
				return nff, err
			}
		} else if subs[0] == "resolution" {
			for i, d := range subs[1:] {
				nff.Resolution[i], err = strconv.ParseUint(d, 10, 64)
				if err != nil {
					return nff, err
				}
			}
		} else if subs[0] == "l" {
			var l Light
			for i, f := range subs[1:] {
				res, err := strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}

				if i < 3 {
					l.Position[i] = res
				} else {
					l.Color[i%3] = res
				}
			}
			nff.Lights = append(nff.Lights, l)
		} else if subs[0] == "f" {
			for i, f := range subs[1:] {
				res, err := strconv.ParseFloat(f, 64)
				if err != nil {
					return nff, err
				}

				if i < 3 {
					nff.Material.Color[i] = res
				} else if i == 4 {
					nff.Material.Diffuse = res
				} else if i == 5 {
					nff.Material.Specular = res
				} else if i == 6 {
					nff.Material.Shine = res
				} else if i == 7 {
					nff.Material.Transmittance = res
				} else {
					nff.Material.IndexOfRefraction = res
				}
			}
		} else if subs[0] == "p" {
			count, err := strconv.ParseUint(subs[1], 10, 64)
			if err != nil {
				return nff, err
			}

			var p Polygon
			p.Vertices = make([]Vector, 0)

			for j := 0; j < int(count); j++ {
				i++
				v := Vector{}
				for k, f := range strings.Split(lines[i], " ") {
					v[k], err = strconv.ParseFloat(f, 64)
					if err != nil {
						return nff, err
					}
				}
				p.Vertices = append(p.Vertices, v)
			}

			nff.Polygons = append(nff.Polygons, p)
		}
	}

	return nff, nil
}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Usage: raytracer input.nff output.jpg")
		return
	}

	b, err := os.ReadFile(os.Args[1])
	if err != nil {
		fmt.Println(err)
		return
	}

	nff, err := parseNFF(string(b))
	if err != nil {
		fmt.Println(err)
		return 
	}

	for i, p := range nff.Polygons {
		fmt.Printf("%d: %v {%d}\n", i+1, p, len(p.Vertices))
	}
}
