#pragma once
#include <vector>

struct Cell0D {
	unsigned int id;
	int marker;
	double x, y;
};

struct Cell1D {
	unsigned int id;
	int marker;
	unsigned int origin, end;
};

struct Cell2D {
	unsigned int id;
	int marker;
	std::vector<unsigned int> vertices;
	std::vector<unsigned int> edges;
};

struct PolygonalMesh {
	std::vector<Cell0D> Cell0Ds;
	std::vector<Cell1D> Cell1Ds;
	std::vector<Cell2D> Cell2Ds;
};