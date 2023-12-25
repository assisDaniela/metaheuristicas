#include <iostream>
#include <string>
#include "TSPP.h"
#include <algorithm>

using namespace std;

void printSolution(const vi &solution) {
  for(int i=0; i<solution.size(); i++) {
    cout << solution[i] << " ";
  }
}

int main() {
  string fileName, penaltyType;
  while(cin >> fileName >> penaltyType) {
    TSPP tspp(fileName, penaltyType);
    matrix graph = tspp.getGraph();
    penaltyType = (penaltyType == "-1" ? "zero" : penaltyType);

    try {

      vi solution(graph.size(), 0);

      for(int i=1; i<=solution.size(); i++) solution[i-1] = i;
      unsigned cost = tspp.routeAvaliation(solution);
      cout << fileName << "-" << penaltyType << ": " << cost;
      cout << "\n\n";

    } catch(const char* msg) {
      cout << msg << endl; 
    }
  }

  return 0;
}