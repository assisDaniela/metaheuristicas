#include "TSPP.h"
#include <iostream>
#include <math.h>

TSPP::TSPP(string city, string penaltyType) {
  this->city = city;
  this->penaltyType = penaltyType;
  vvd locations = this->readLocationsFile();
  createGraph(locations);
  readPenaltyFile();
}

matrix TSPP::getGraph() { return graph; }
unsigned TSPP::getNumCitys() { return this->nCitys; }

void TSPP::readFile(ifstream &file, string filePath) {
  file = ifstream(filePath, ifstream::in);

  if(!file.is_open()) {
    throw("Error opening file: " + filePath);
  }

  return;
}

vvd TSPP::readLocationsFile() {
  ifstream fin;
  vvd locations;
  try {
    string filePath = "instancias/" + this->city + ".tspp";
    readFile(fin, filePath);
    int n;
    fin >> this->nCitys;
    string entry;

    locations = vvd(this->nCitys, vector<double>(2));

    for(int i=0; i<this->nCitys; i++) {
      for(int j=0; j<3; j++) {
        fin >> entry;
        if(j == 0) continue;
        locations[i][j-1] = stod(entry);
      }
    }

  } catch(const char* msg) {
    fin.close();
    cout << msg << endl;
  }

  fin.close();
  return locations;
}

void TSPP::readPenaltyFile() {
  ifstream fin;
  try {
    if(penaltyType == "-1") {
      penalty = matrix(nCitys, vi(nCitys, 0));
    } else {
      string filePath = "instancias/" + city + "-" + penaltyType + ".txt";
      readFile(fin, filePath);
      fin >> nCitys;

      penalty = matrix(nCitys, vi(nCitys, 0));
      unsigned entry;
      for(int i=0; i<nCitys; i++) {
        for(int j=0; j<nCitys; j++) {
          fin >> entry;

          penalty[i][j] = entry;
        }
      }
    }
  } catch (const char* msg) {
    fin.close();
    cout << msg << endl;
  }

  fin.close();
}

void TSPP::createGraph(const vvd &locations) {
  this->graph = matrix(this->nCitys, vi(this->nCitys, 0));

  for(int i=0; i<this->nCitys; i++) {
    for(int j=i+1; j<this->nCitys; j++) {
      if(i == j) continue;

      graph[i][j] = euclideanDistance(locations[i][0], locations[i][1], locations[j][0], locations[j][1]);
      graph[j][i] = graph[i][j];
    }
  }
}

unsigned TSPP::euclideanDistance(double x1, double y1, double x2, double y2) {
  double aux = pow(x1-x2, 2) + pow(y1-y2, 2);
  unsigned dist = (unsigned)(sqrt(aux)+0.5);
  return dist;
}

unsigned TSPP::routeAvaliation(const vi &solution) {
  unsigned cost = 0;
  for(int i=0; i<solution.size()-1; i++) {
    int curr = solution[i] - 1;
    int next = solution[i+1] - 1;
    cost += graph[curr][next] + penalty[curr][i];
  }

  int last = solution[solution.size()-1] - 1;
  cost += graph[last][0] + penalty[last][nCitys-1];

  return cost;
}

unsigned TSPP::routeAvaliation(const li &solution) {
  vi vSolution(solution.begin(), solution.end());
  return routeAvaliation(vSolution);
}