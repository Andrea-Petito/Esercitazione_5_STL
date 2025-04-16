#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

const double tolerance = 1e-10;

bool loadCell0Ds(const string& filename, vector<Cell0D>& cells) {
	ifstream file(filename);
	if (file.fail()) {
		cerr << "Errore nell'apertura del file" << endl;
		return false;
	}
	string line;
	getline(file, line);
	
	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		Cell0D cell;
		
		getline(ss, token, ';');
		cell.id = stoi(token);
		getline(ss, token, ';');
		cell.marker = stoi(token);
		getline(ss, token, ';');
		cell.x = stod(token);
		getline(ss, token, ';');
		cell.y = stod(token);
		
		cells.push_back(cell);
	}
	return true;
}

bool loadCell1Ds(const string& filename, vector<Cell1D>& cells) {
    ifstream file(filename);
    if (file.fail()) {
		cerr << "Errore nell'apertura del file" << endl;
		return false;
	}
    string line;
    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Cell1D cell;

        getline(ss, token, ';');
        cell.id = stoi(token);
        getline(ss, token, ';');
        cell.marker = stoi(token);
        getline(ss, token, ';');
        cell.origin = stoi(token);
        getline(ss, token, ';');
        cell.end = stoi(token);

        cells.push_back(cell);
    }

    return true;
}

bool loadCell2Ds(const string& filename, vector<Cell2D>& cells) {
    ifstream file(filename);
    if (file.fail()) {
        cerr << "Errore nell'apertura del file: " << filename << endl;
        return false;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Cell2D cell;

        getline(ss, token, ';');
        cell.id = stoi(token);
        getline(ss, token, ';');
        cell.marker = stoi(token);

        getline(ss, token, ';');
        int numVertices = stoi(token);
        for (int i = 0; i < numVertices; ++i) {
            getline(ss, token, ';');
            cell.vertices.push_back(stoi(token));
        }

        getline(ss, token, ';');
        int numEdges = stoi(token);
        for (int i = 0; i < numEdges; ++i) {
            getline(ss, token, ';');
            cell.edges.push_back(stoi(token));
        }

        cells.push_back(cell);
    }

    return true;
}

bool loadPolygonalMesh(const string& folderPath, PolygonalMesh& mesh) {
    cout << "Loading: " << folderPath + "Cell0Ds.csv" << endl;
    cout << "Loading: " << folderPath + "Cell1Ds.csv" << endl;
    cout << "Loading: " << folderPath + "Cell2Ds.csv" << endl;

    return loadCell0Ds(folderPath + "Cell0Ds.csv", mesh.Cell0Ds) &&
           loadCell1Ds(folderPath + "Cell1Ds.csv", mesh.Cell1Ds) &&
           loadCell2Ds(folderPath + "Cell2Ds.csv", mesh.Cell2Ds);
}


double computeDistance(const Cell0D& a, const Cell0D& b) {
    Vector2d va(a.x, a.y);
    Vector2d vb(b.x, b.y);
    return (vb - va).norm();
}

double computeArea(const std::vector<Cell0D>& points) {
    if (points.size() < 3) return 0.0;

    double area = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        const Cell0D& p1 = points[i];
        const Cell0D& p2 = points[(i + 1) % points.size()];
        area += (p1.x * p2.y - p2.x * p1.y);
    }

    return 0.5 * std::abs(area);
}


bool checkMeshValidity(const PolygonalMesh& mesh) {
    for (const auto& edge : mesh.Cell1Ds) {
        const auto& p1 = mesh.Cell0Ds[edge.origin];
        const auto& p2 = mesh.Cell0Ds[edge.end];
        if (abs(computeDistance(p1, p2)) < tolerance) {
            cerr << "Edge con lunghezza nulla: ID = " << edge.id << endl;
            return false;
        }
    }
    for (const auto& cell : mesh.Cell2Ds) {
        vector<Cell0D> vertices;
        for (int idx : cell.vertices) {
            vertices.push_back(mesh.Cell0Ds[idx]);
        }
        if (abs(computeArea(vertices)) < tolerance) {
            cerr << "Cella con area nulla: ID = " << cell.id << endl;
            return false;
        }
    }
	return true;
}

bool compareMeshes(const PolygonalMesh& a, const PolygonalMesh& b) {
    if (a.Cell0Ds.size() != b.Cell0Ds.size()) return false;
    if (a.Cell1Ds.size() != b.Cell1Ds.size()) return false;
    if (a.Cell2Ds.size() != b.Cell2Ds.size()) return false;

    for (size_t i = 0; i < a.Cell0Ds.size(); ++i) {
        const auto& ca = a.Cell0Ds[i];
        const auto& cb = b.Cell0Ds[i];
        if (ca.id != cb.id || ca.marker != cb.marker || ca.x != cb.x || ca.y != cb.y) return false;
    }

    for (size_t i = 0; i < a.Cell1Ds.size(); ++i) {
        const auto& ca = a.Cell1Ds[i];
        const auto& cb = b.Cell1Ds[i];
        if (ca.id != cb.id || ca.marker != cb.marker || ca.origin != cb.origin || ca.end != cb.end) return false;
    }

    for (size_t i = 0; i < a.Cell2Ds.size(); ++i) {
        const auto& ca = a.Cell2Ds[i];
        const auto& cb = b.Cell2Ds[i];
        if (ca.id != cb.id || ca.marker != cb.marker ||
            ca.vertices != cb.vertices || ca.edges != cb.edges) return false;
    }

    return true;
}
