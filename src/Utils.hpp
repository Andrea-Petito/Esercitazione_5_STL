#pragma once
#include <string>
#include "PolygonalMesh.hpp"

bool loadCell0Ds(const std::string& filename, std::vector<Cell0D>& cells);

bool loadCell1Ds(const std::string& filename, std::vector<Cell1D>& cells);

bool loadCell2Ds(const std::string& filename, std::vector<Cell2D>& cells);

bool loadPolygonalMesh(const std::string& folderPath, PolygonalMesh& mesh);

double computeDistance(const Cell0D& a, const Cell0D& b);

double computeArea(const std::vector<Cell0D>& points);

bool checkMeshValidity(const PolygonalMesh& mesh);

bool compareMeshes(const PolygonalMesh& a, const PolygonalMesh& b);