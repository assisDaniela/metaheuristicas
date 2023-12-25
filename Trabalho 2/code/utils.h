#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include "TSPP.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

typedef vector<vector<unsigned>> matrix;
typedef vector<unsigned> vi;
typedef list<unsigned> li;
typedef vector<vector<double>> vvd;
typedef pair<int, int> pii;

void nearestNeighborV1(TSPP &tspp, vi &solution);
void nearestNeighborV2(TSPP &tspp, vi &solution, int startCity);
unsigned SA(TSPP &tspp, vi &initSolution, double T_MAX, double T_MIN, double R, int MAX_NO_IMPROVEMENT);
unsigned costPath(TSPP &tspp, vi &solution, int start, int end);
unsigned twoOtp(TSPP &tspp, vi &solution, int start, int end);
void printSolution(const vi &solution);
void randomSolution(vi &solution, int numCitys);

// Implementations

// Rota começando com a cidade 1
// Acrescentar cidades ao final da rota
// [1] -> [1, x] -> [1, x, y] -> ...
// x mais próxima de 1, y mais próxima de x
void nearestNeighborV1(TSPP &tspp, vi &solution) {
  int nCitys = tspp.getNumCitys();
  vector<bool> visited(nCitys, false);
  int currCity = 0;
  int visitedCitysCount = 1;
  visited[0] = true;
  solution.push_back(currCity+1);

  matrix mAdj = tspp.getGraph();

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
void nearestNeighborV2(TSPP &tspp, vi &vSolution, int startCity) {
  int ext1=startCity-1, ext2=-1;
  int nCitys = tspp.getNumCitys();

  matrix mAdj = tspp.getGraph();

  li solution;
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

  vSolution = vi(solution.begin(), solution.end());
}

unsigned costPath(TSPP &tspp, vi &solution, int start, int end) {
  unsigned cost = 0;
  for(int i=start; i<end; i++) {
    int city1 = solution[i];
    int city2 = solution[i+1];
    cost += tspp.getArcCost(city1, city2);
  }

  for(int i=start+1; i<=end-1; i++) {
    cost += tspp.getPenalty(solution[i], i);
  }

  return cost;
}

unsigned twoOtp(TSPP &tspp, vi &solution, int start, int end) {
  unsigned oldCost = costPath(tspp, solution, start, end);
  int length = (end - start - 1) / 2;
  for(int i = 1; i<=length; i++) {
    int aux = solution[start+i];
    solution[start+i] = solution[end-i];
    solution[end-i] = aux;
  }
  unsigned newCost = costPath(tspp, solution, start, end);

  return oldCost - newCost;
}

// void printSolution(const vi &solution) {
//   for(int i=0; i<solution.size(); i++) {
//     cout << solution[i]+1 << " ";
//   }

//   cout << endl;
// }

unsigned SA(TSPP &tspp, vi &initSolution, double T_MAX, double T_MIN, double R, int MAX_NO_IMPROVEMENT) {

  int nCitys = tspp.getNumCitys();

  vi bestSolution = vi(initSolution.begin(), initSolution.end());

  unsigned bestAvaliation = tspp.routeAvaliation(bestSolution);

  // cout << "Init solution avaliation: " << bestAvaliation << "\n";
  int iterations;
  double t = T_MAX;
  for(iterations=0; t > T_MIN; iterations++) {
    int noImprovement = 0;
    vi currSolution(bestSolution.begin(), bestSolution.end());
    unsigned currAvaliation = bestAvaliation;
    while(noImprovement < MAX_NO_IMPROVEMENT) {
      int start, end;
      while(true) {
        int rand1 = rand() % tspp.getNumCitys();
        int rand2 = rand() % tspp.getNumCitys();
        if(rand1 != rand2 and abs(rand1-rand2) > 3) {
          start = min(rand1, rand2);
          end = max(rand1, rand2);
          break;
        }
      }

      int diference = twoOtp(tspp, currSolution, start, end);
      unsigned neighborAvaliation = currAvaliation - diference;

      bool update = true;
      if(currAvaliation <= neighborAvaliation) {
        double p = -1.0 * (neighborAvaliation - currAvaliation) / t;
        p = exp(p);
        double random = rand() / (double) RAND_MAX;
        update = false;

        if(random < p) {
          update = true;
        }
      } else {
        update = true;
      }

      if(!update) {
        int d = twoOtp(tspp, currSolution, start, end);
      } else {
        currAvaliation = neighborAvaliation;
      }

      if(bestAvaliation > currAvaliation) {
        bestSolution = currSolution;
        bestAvaliation = currAvaliation;
        noImprovement = 0;
      } else {
        noImprovement++;
      }
    }
    t = t * R;
  }

  // cout << "\nIteration SA stop" << "\n\tIterations: " << iterations << "\n\tBest avaliation: " << bestAvaliation;
  // cout << "\n\n-------------------------------------------------\n\n";
  return bestAvaliation;
}

void randomSolution(vi &solution, int numCitys) {
  solution = vi(numCitys);
  for(int i=0; i<numCitys; i++) {
    solution[i] = i+1;
  }

  random_shuffle(solution.begin(), solution.end());
}