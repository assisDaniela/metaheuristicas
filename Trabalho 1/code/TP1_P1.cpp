#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <utility>
#include "TSPP.h"

using namespace std;
typedef pair<int, int> pii;

void printSolution(const vi &solution) {
  for(int i=0; i<solution.size(); i++) {
    cout << solution[i] << " ";
  }
}

// Rota começando com a cidade 1
// Acrescentar cidades ao final da rota
// [1] -> [1, x] -> [1, x, y] -> ...
// x mais próxima de 1, y mais próxima de x
void findSolutionV1(const matrix &mAdj, li &solution) {
  vector<bool> visited(mAdj.size(), false);
  int nCitys = mAdj.size();
  int currCity = 0;
  int visitedCitysCount = 1;
  visited[0] = true;
  solution.push_back(currCity+1);

  while(visitedCitysCount < nCitys) {
    int minDist = -1;
    int addCity;

    for(int j=0; j<nCitys; j++) {
      if(j != currCity && !visited[j]) {
        if(minDist == -1) {
          minDist = mAdj[currCity][j];
          addCity = j;
        } else {
          if(minDist > mAdj[currCity][j]) {
            minDist = mAdj[currCity][j];
            addCity = j;
          }
        }
      }
    }
    
    solution.push_back(addCity+1);
    visited[addCity] = true;
    visitedCitysCount++;
    currCity = addCity;
  }
}

pair<int, int> getMinDistance(const matrix &mAdj, int city, const vi &visited) {
  int minCost=-1, nextCity;
  for(int i=0; i<visited.size(); i++) {
    if(city != i && !visited[i]) {
      if(minCost == -1) {
        minCost = mAdj[city][i];
        nextCity = i;
      } else {
        if(minCost > mAdj[city][i]) {
          minCost = mAdj[city][i];
          nextCity = i;
        }
      }
    }
  }

  return make_pair(nextCity, minCost);
}

// Rota começando com a cidade 1
// Acrescentar cidades aos extremos
// Acrescenta-se a cidade x mais próxima de 1
// Depois, a cidade y mais próxima de x ou a cidade z mais próxima de 1 (a menor distância)
void findSolutionV2(const matrix &mAdj, li &solution) {
  int ext1=0, ext2=-1;
  int nCitys = mAdj.size();

  vi visited(nCitys, false);
  solution.push_front(ext1+1);
  visited[0] = true;
  int visitedCitysCount = 1;

  pii ext1Min, ext2Min;
  while(visitedCitysCount < nCitys) {
    ext1Min = getMinDistance(mAdj, ext1, visited);
    
    if(ext2 != -1) ext2Min = getMinDistance(mAdj, ext2, visited);
    else {
      ext2Min = ext1Min;
      ext2 = ext1;
    }


    if(ext1Min.second <= ext2Min.second) {
      solution.push_front(ext1Min.first + 1);
      ext1 = ext1Min.first;
      visited[ext1Min.first] = true;
    } else {
      solution.push_back(ext2Min.first + 1);
      ext2 = ext2Min.first;
      visited[ext2Min.first] = true;
    }

    visitedCitysCount++;
  }
}

int main() {
  string fileName, penaltyType;

  while(cin >> fileName >> penaltyType) {
    try {
      TSPP tspp(fileName, penaltyType);
      matrix mAdj = tspp.getGraph();
      unsigned cost = 0;

      penaltyType = (penaltyType == "-1" ? "zero" : penaltyType);

      li solution;
      findSolutionV1(mAdj, solution);
      cost = tspp.routeAvaliation(solution);
      cout << fileName << "-" << penaltyType << " v1: ";
      cout << cost << "\n";
            
      solution.clear();
      findSolutionV2(mAdj, solution);
      cost = tspp.routeAvaliation(solution);
      cout << fileName << "-" << penaltyType << " v2: ";
      cout << cost << "\n\n";
    } catch(const char* msg) {
      cout << msg << endl; 
    }
  }

  return 0;
}