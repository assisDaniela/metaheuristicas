#include "TSPP.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

using namespace std;

time_t now = time(nullptr);
string fileName = "results/grasp_" + to_string(now) + ".csv";
ofstream out(fileName);
vector<string> memoryResults(4);

typedef list<pair<int, unsigned>>::iterator ITL;

void storeResults();
void printReport(int iteration, unsigned avaliation);
void GRASP(TSPP &tspp, double initAlpha, int MAX_ITERATIONS, int MAX_NO_IMPROVEMENT);
void constructSolution(TSPP &tspp, double alpha, vi &greedyRandSolution);
void printSolution(const vi &solution) {
  for(int i=0; i<solution.size(); i++) {
    cout << solution[i] << " ";
  }
  cout << endl;
}

int main() {
  srand(time(NULL));
  string fileName, penaltyType;

  int maxIterations = 10000;
  int maxNoImprovement = 2500;
  // int numPertubations = 5;

  while(cin >> fileName >> penaltyType) {
    TSPP tspp(fileName, penaltyType);
    penaltyType = (penaltyType == "-1" ? "zero" : penaltyType);

    try {
      cout << fileName << "-" << penaltyType << endl;
      string cityId = fileName + "-" + penaltyType;
      memoryResults[0] =  cityId;
      GRASP(tspp, -1.0, maxIterations, maxNoImprovement);
      storeResults();
    } catch(const char* msg) {
      cout << msg << endl; 
    }
  }

  return 0;
}

void GRASP(TSPP &tspp, double initAlpha, int MAX_ITERATIONS, int MAX_NO_IMPROVEMENT) {
  double alpha = 0.0;
  if(initAlpha > 0) {
    alpha = initAlpha;
  } else {
    while(alpha < 0.1 && alpha > 0.9) {
      alpha = rand() % RAND_MAX;
    }
  }

  vi bestSolution;
  constructSolution(tspp, alpha, bestSolution);
  SA(tspp, bestSolution, 10000, 0.000001, 0.7, 2500);
  unsigned bestAvaliation = tspp.routeAvaliation(bestSolution);
  memoryResults[1] = to_string(bestAvaliation);
  cout << "First avaliation: " << bestAvaliation << endl;
  int bestInIteration = 0;
  int noImprovement = 0;
  for(int iteration=1; iteration < MAX_ITERATIONS && noImprovement < MAX_NO_IMPROVEMENT; iteration++) {
    vi currSolution;
    alpha = (initAlpha > 0 ? initAlpha : rand() / RAND_MAX);
    constructSolution(tspp, alpha, currSolution);
    // cout << "\tSA start " << iteration << "\n";
    SA(tspp, currSolution, 100000, 0.000001, 0.75, 1000);
    // cout << "\tSA end " << iteration << "\n";
    unsigned currAvaliation = tspp.routeAvaliation(currSolution);

    if(currAvaliation < bestAvaliation) {
      bestSolution = currSolution;
      bestAvaliation = currAvaliation;
      bestInIteration = iteration;
      printReport(iteration, bestAvaliation);
      noImprovement = 0;
    } else {
      noImprovement++;
    }
  }

  cout << "\nIteration stop";
  cout << "\n\tIteration: " << bestInIteration;
  cout << "\n\tNo Improvement: " << noImprovement;
  cout << "\n\tBest avaliation: " << bestAvaliation;
  cout << "\n\n-------------------------------------------------\n\n";

  memoryResults[2] = to_string(bestAvaliation);
  memoryResults[3] = to_string(bestInIteration);
}

void constructSolution(TSPP &tspp, double alpha, vi &greedyRandSolution) {
  int startCity = (rand() % tspp.getNumCitys()) + 1;
  vector<bool> visited(tspp.getNumCitys(), false);

  visited[startCity-1] = true;
  list<pair<int, unsigned>> CL;

  greedyRandSolution = vi(tspp.getNumCitys());
  greedyRandSolution[0] = startCity;
  for(int pos=1; pos < tspp.getNumCitys(); pos++) {
    unsigned max = 0, min = -1;
    for(int nextCity=1; nextCity<=tspp.getNumCitys(); nextCity++) {
      if(nextCity == startCity || visited[nextCity-1]) continue;
      unsigned cost = tspp.getArcCost(startCity, nextCity) + tspp.getPenalty(nextCity, pos);
      // cout << startCity << " -> " << nextCity << " = " << cost << endl;
      if(max < cost) max = cost;
      if(min > cost || min == -1) min = cost;
      CL.push_back(make_pair(nextCity, cost));
    }

    unsigned thresh = min + (max - min) * alpha;
    vector<int> RCL;
    for(ITL it=CL.begin(); it != CL.end(); it++) {
      unsigned cost = it->second;
      if(cost <= thresh) RCL.push_back(it->first);
    }

    int randomSelection = rand() % RCL.size();
    int choice = RCL[randomSelection];
    greedyRandSolution[pos] = choice;
    CL.clear();
    startCity = choice;
    visited[choice-1] = true;
  }
}

void printReport(int iteration, unsigned avaliation) {
  cout << "New best solution:\n";
  cout << "\tIteration: " << iteration << endl;
  cout << "\tAvaliation: " << avaliation << "\n\n";
  return;
}

void storeResults() {
  out << "Instance,Initial,Best,InIteration\n";
  for(int i=0; i<memoryResults.size(); i++) {
    out << memoryResults[i];
    out << (i == memoryResults.size() - 1 ? "\n" : ",");
  }
}