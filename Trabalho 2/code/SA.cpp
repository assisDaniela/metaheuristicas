#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <chrono>
#include <sys/time.h>
#include "TSPP.h"
#include "utils.h"

using namespace std;

int params = 9;
double PARAMS[][3] = {
  {10000, 0.000000001, 0.1},
  {10000, 0.000001, 0.3},
  {1000000, 0.0001, 0.1},
  {1000000, 0.0000001, 0.3},
  {1000000, 0.000000001, 0.75},
  {1000000, 0.0000001, 0.75},
  {10000, 0.000000001, 0.75},
  {10000, 0.000000001, 0.3},
  {1000, 0.000000001, 0.1},
  {10000, 0.0000001, 0.5}
};

double T_MAX;
double T_MIN;
double R;
int MAX_ITERATIONS;
int MAX_NO_IMPROVEMENT=5000;

time_t now = time(nullptr);
string fileName = "results/SA_" + to_string(now) + "_" + to_string(params) + ".csv";
ofstream out(fileName);

void printSolution(const vi &solution);
void report(int iteration, unsigned avaliation, double T);
void storeResults();
void SA(TSPP &tspp, string cityId);

vector<string> memoryResults(3);

int main() {
  vector<string> head(3);
  head[0] = "City-penalty";
  head[1] = "Init Avaliation";
  head[2] = "Best avaliation";

  T_MAX = PARAMS[params][0];
  T_MIN = PARAMS[params][1];
  R = PARAMS[params][2];

  for(int i=0; i<3; i++) {
    out << head[i];
    out << (i == 2 ? "\n" : ",");
  }

  srand(time(NULL));
  string fileName, penaltyType;

  while(true) {
    cin >> fileName;
    if(cin.eof()) break;
    cin >> penaltyType;
    TSPP tspp(fileName, penaltyType);
    penaltyType = (penaltyType == "-1" ? "zero" : penaltyType);

    try {
      cout << fileName << "-" << penaltyType << endl;
      string id = fileName + '-' + penaltyType;
      for(int i=0; i<5; i++) {
      cout << "Iteration " << i << endl;
        SA(tspp, id);
        storeResults();
      }
    } catch(const char* msg) {
      cout << msg << endl; 
    }
  }

  out.close();

  return 0;
}

void SA(TSPP &tspp, string cityId) {

  int nCitys = tspp.getNumCitys();

  vi solution;
  // randomSolution(solution, tspp.getNumCitys());

  nearestNeighborV2(tspp, solution, 1);

  vi bestSolution = vi(solution.begin(), solution.end());

  unsigned bestAvaliation = tspp.routeAvaliation(bestSolution);
  memoryResults[1] = to_string(bestAvaliation);

  cout << "Init solution avaliation: " << bestAvaliation << "\n";
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

  cout << "\nIteration stop" << "\n\tIterations: " << iterations << "\n\tBest avaliation: " << bestAvaliation;
  cout << "\n\n-------------------------------------------------\n\n";
 
  memoryResults[0] = cityId;
  memoryResults[2] = to_string(bestAvaliation);
}

void report(int iteration, unsigned avaliation, double T) {
  cout << "New best solution:\n";
  cout << "\tIteration: " << iteration << endl;
  cout << "\tAvaliation: " << avaliation << "\n";
  cout << "\tTemperature: " << T << "\n\n";
  return;
}


void storeResults() {
  for(int i=0; i<memoryResults.size(); i++) {
    out << memoryResults[i];
    out << (i == memoryResults.size() - 1 ? "\n" : ",");
  }
}
