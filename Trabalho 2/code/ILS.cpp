#include "TSPP.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

using namespace std;

vector<bool> posUsedToPertub;

time_t now = time(nullptr);
string fileName = "results/ILS_" + to_string(now) + ".csv";
ofstream out(fileName);
vector<string> memoryResults(3);

bool doubleBridgePertub(TSPP &tspp, vi &solution);
// void twoOtp(vi &solution, int start, int end);
// void printSolution(const vi &solution);
void ILS(TSPP &tspp, int maxIterations, int maxNoImprovement, string id);
bool localSearch2Opt(TSPP &tspp, vi &solution);
unsigned acceptSolution(TSPP &tspp, vi &bestSolution, vi &pertubSolution);
void storeResults();

int main() {
  srand(time(NULL));
  string fileName, penaltyType;

  int maxIterations = 10000;
  int maxNoImprovement = 1000;

  while(cin >> fileName >> penaltyType) {
    TSPP tspp(fileName, penaltyType);
    penaltyType = (penaltyType == "-1" ? "zero" : penaltyType);

    try {
      cout << fileName << "-" << penaltyType << endl;
      string cityId = fileName + "-" + penaltyType;
      // int maxPertubation = (tspp.getNumCitys() - 7) * 0.3;
      ILS(tspp, maxIterations, maxNoImprovement, cityId);
      storeResults();
    } catch(const char* msg) {
      cout << msg << endl; 
    }
  }

  return 0;
}

void printReport(int iteration, unsigned avaliation, int numPertubation) {
  cout << "New best solution:\n";
  cout << "\tIteration: " << iteration << endl;
  cout << "\tPertubations: " << numPertubation << endl;
  cout << "\tAvaliation: " << avaliation << "\n\n";
  return;
}

void ILS(TSPP &tspp, int maxIterations, int maxNoImprovement, string id) {
  vi solution;

  // Solução inicial
  // randomSolution(solution, tspp.getNumCitys());
  // nearestNeighborV1(tspp, solution);
  nearestNeighborV2(tspp, solution, 1);
  
  unsigned bestAvaliation = tspp.routeAvaliation(solution);
  cout << "Init avaliation: " << bestAvaliation << endl;

  memoryResults[0] = id;
  memoryResults[1] = to_string(bestAvaliation);

  // Local search
  int numPertubation = 5, maxNumPertubations = 5;
  vi bestSolution(solution.begin(), solution.end());
  localSearch2Opt(tspp, bestSolution);
  bestAvaliation = tspp.routeAvaliation(bestSolution);
  cout << "Local Search avaliation: " << bestAvaliation << endl;
  vi currSolution(bestSolution);
  solution.clear();

  unsigned noImprovement = 0;
  int iteration;
  bool stop = false;
  
  for(iteration=1; iteration < maxIterations && noImprovement < maxNoImprovement ; iteration++) {
    vi pertubSolution(currSolution);
    posUsedToPertub = vector<bool>(tspp.getNumCitys() - 7, false);
    // cout << "\tStart pertubation\n";
    for(int d=1; d <= numPertubation; d++) {
      bool resp = doubleBridgePertub(tspp, pertubSolution);
      if(!resp) break;
    }

    unsigned avaliation = SA(tspp, pertubSolution, 1000000, 0.000000001, 0.75, 2500);
    if(avaliation >= bestAvaliation) {
      // cout << "SA avaliation: " << avaliation << " Iteration: " << iteration << "Pertubations: " << numPertubation << endl;
      noImprovement++;
      numPertubation--;
      numPertubation = (numPertubation == 0 ? 5 : numPertubation);
    } else {
      // Ótimo local gerado pela pertubação é melhor que a melhor solução encontrada
      bestSolution = pertubSolution;
      bestAvaliation = avaliation;
      noImprovement = 0;
      currSolution = pertubSolution;
      printReport(iteration, avaliation, numPertubation);
      numPertubation = 5;
    }
  }

  cout << "\nBest route with avaliation = " << bestAvaliation << " in " << iteration << " iterations\n\n";
  cout << "--------------------------------------------------------\n\n";
  memoryResults[2] = to_string(bestAvaliation);
}

bool localSearch2Opt(TSPP &tspp, vi &solution) {
  vi bestSolution(solution);
  while(true) {
    unsigned oldAvaliation, bestAvaliation;
    bestAvaliation = oldAvaliation = tspp.routeAvaliation(solution);

    for(int i=0; i<solution.size(); i++) {
      if(i+3 >= solution.size()) break;
      for(int j=i+3; j<solution.size()-1; j++) {
        unsigned difference = twoOtp(tspp, solution, i, j);
        unsigned avaliation = bestAvaliation - difference;

        if(avaliation < bestAvaliation) {
          bestSolution = solution;
          bestAvaliation = avaliation;
        }

        twoOtp(tspp, solution, i, j);
      }
    }

    if(bestAvaliation >= oldAvaliation) return false;
    else {
      solution = bestSolution;
    }
  }

  return true;
}

unsigned acceptSolution(TSPP &tspp, vi &bestSolution, vi &pertubSolution) {
  unsigned bestAvaliation = tspp.routeAvaliation(bestSolution);
  unsigned pertubAvaliation = tspp.routeAvaliation(pertubSolution);

  if(pertubAvaliation < bestAvaliation) return pertubAvaliation;
  return -1;
}

bool doubleBridgePertub(TSPP &tspp, vi &solution) {
  
  int start;
  int limit = solution.size() - 7;

  start = rand() % limit;
  bool find = false;
  int old = start;
  while(true) {
    if(!posUsedToPertub[start]) {
      find = true;
      break;
    } else {
      start = (start + 1 == posUsedToPertub.size() ? 0 : start+1);
    }

    if(start == old) break;
  }

  if(!find) return false;
  posUsedToPertub[start] = true;

  int end = start + 7;
  unsigned cost = 0;
  cost = twoOtp(tspp, solution, start+2, end);
  cost = twoOtp(tspp, solution, start, end-2);
  cost = twoOtp(tspp, solution, start+2, end);

  return true;
}

void storeResults() {
  for(int i=0; i<memoryResults.size(); i++) {
    out << memoryResults[i];
    out << (i == memoryResults.size() - 1 ? "\n" : ",");
  }
}