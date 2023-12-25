#ifndef TSPP_H
#define TSPP_H

#include <string>
#include <vector>
#include <list>
#include <fstream>

using namespace std;

typedef vector<vector<unsigned>> matrix;
typedef vector<unsigned> vi;
typedef list<unsigned> li;
typedef vector<vector<double>> vvd;

class TSPP {
  public:
    TSPP(string city, string penaltyType);
    unsigned routeAvaliation(const vi &solution);
    unsigned routeAvaliation(const li &solution);
    matrix getGraph();
    unsigned getNumCitys();
    unsigned getArcCost(int city1, int city2);
    unsigned getPenalty(int city, int routePos);
    
  private:
    vvd readLocationsFile();
    void createGraph(const vvd &locations);
    void readPenaltyFile();
    void readFile(ifstream &file, string filePath);
    unsigned euclideanDistance(double x1, double y1, double x2, double y2);
    matrix graph;
    matrix penalty;

    string city;
    string penaltyType;
    int nCitys;

};

#endif