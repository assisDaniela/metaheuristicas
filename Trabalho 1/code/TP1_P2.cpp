#include "TSPP.h"
#include <utility>
#include <stdio.h>
#include <iostream>

using namespace std;

typedef pair<int, int> pii;

void nearestNeighborV1(const matrix &mAdj, li &solution);
void nearestNeighborV2(const matrix &mAdj, li &solution);
void sequentialSolution(vi &solution, int nCitys);
pair<int, int> getMinDistance(const matrix &mAdj, int city, const vi &visited);
void swap(vi &solution, int pos1, int pos2);
void printSolution(const vi &solution);
void localSearch2opt(TSPP &tspp, vi &solution);

int main() {
  string fileName, penaltyType;
  while(cin >> fileName >> penaltyType) {
    TSPP tspp(fileName, penaltyType);
    string id = fileName + "-" + (penaltyType == "-1" ? "zero" : penaltyType);

    vi solution;
    cout << id << endl;
    cout << "Sequencial: ";
    sequentialSolution(solution, tspp.getNumCitys());
    localSearch2opt(tspp, solution);
    cout << tspp.routeAvaliation(solution) << "\n";

    li lSolution;
    cout << "Guloso V1:  ";
    nearestNeighborV1(tspp.getGraph(), lSolution);
    solution = vi(lSolution.begin(), lSolution.end());
    localSearch2opt(tspp, solution);
    cout << tspp.routeAvaliation(solution) << "\n";

    lSolution.clear();
    cout << "Guloso V2:  ";
    nearestNeighborV2(tspp.getGraph(), lSolution);
    solution = vi(lSolution.begin(), lSolution.end());
    localSearch2opt(tspp, solution);
    cout << tspp.routeAvaliation(solution) << "\n\n";
  }
  return 0;
}

void sequentialSolution(vi &solution, int nCitys) {
  solution = vi(nCitys);
  for(int i=1; i<=nCitys; i++) solution[i-1] = i;
}

// Rota começando com a cidade 1
// Acrescentar cidades ao final da rota
// [1] -> [1, x] -> [1, x, y] -> ...
// x mais próxima de 1, y mais próxima de x
void nearestNeighborV1(const matrix &mAdj, li &solution) {
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
void nearestNeighborV2(const matrix &mAdj, li &solution) {
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

void swap(vi &solution, int pos1, int pos2) {
  int aux = solution[pos1];
  solution[pos1] = solution[pos2];
  solution[pos2] = aux;
  int length = (pos2 - pos1 - 1) / 2;

  for(int i = 1; i<=length; i++) {
    int aux = solution[pos1+i];
    solution[pos1+i] = solution[pos2-i];
    solution[pos2-i] = aux;
  }
}

void printSolution(const vi &solution) {
  for(int i=0; i<solution.size(); i++) {
    cout << solution[i] << " ";
  }

  cout <<  "\n";
}

void localSearch2opt(TSPP &tspp, vi &solution) {
  vi bestSolution(solution);
  // int count = 1;
  while(true) {
    unsigned oldAvaliation, bestAvaliation;
    bestAvaliation = oldAvaliation = tspp.routeAvaliation(solution);

    for(int i=1; i<solution.size() - 1; i++) {
      if(i+3 >= solution.size()) break;
      for(int j=i+3; j<solution.size()-1; j++) {
        swap(solution, i+1, j);
        unsigned avaliation = tspp.routeAvaliation(solution);

        if(avaliation < bestAvaliation) {
          bestSolution = solution;
          bestAvaliation = avaliation;
        }
        swap(solution, i+1, j);
      }
    }

    if(bestAvaliation >= oldAvaliation) return;
    else {
      solution = bestSolution;
    }
  }
}
