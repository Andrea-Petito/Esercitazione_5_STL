#include <iostream>
#include "Utils.hpp"

using namespace std;

int main() {
    PolygonalMesh mesh;
    string folderPath = "./";

    if (!loadPolygonalMesh(folderPath, mesh)) {
        cerr << "Errore nel caricamento della mesh." << endl;
        return 1;
    }

    cout << "Mesh caricata correttamente." << endl;

    if (checkMeshValidity(mesh)) {
        cout << "Mesh valida." << endl;
    } else {
        cout << "Mesh NON valida." << endl;
    }
	
	for (const auto& cell : mesh.Cell2Ds) {
    vector<Cell0D> vertices;
    for (unsigned int vid : cell.vertices) {
        if (vid >= mesh.Cell0Ds.size()) {
            cerr << "Indice vertice non valido nella cella " << cell.id << endl;
            continue;
        }
        vertices.push_back(mesh.Cell0Ds[vid]);
    }

    double area = computeArea(vertices);
    cout << "Area della cella con ID " << cell.id << " = " << area << endl;
}

    PolygonalMesh expectedMesh;
    string expectedFolder = "../PolygonalMesh/";
	cout << "Tentativo di apertura da: " << expectedFolder << endl;

	if (!loadPolygonalMesh(expectedFolder, expectedMesh)) {
		cerr << "Errore nel caricamento della mesh attesa." << endl;
		return 1;
}

    if (compareMeshes(mesh, expectedMesh)) {
        cout << "La mesh caricata è IDENTICA a quella attesa." << endl;
    } else {
        cout << "La mesh caricata è DIVERSA da quella attesa." << endl;
    }

    return 0;
}
