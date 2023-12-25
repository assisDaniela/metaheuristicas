#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>
#include <time.h>
#include <fstream>

using namespace std;

struct Item {
    int id;
    unsigned color;
    unsigned size;
};

struct Avaliation {
    unsigned recipients;
    double eval;
};

struct Info {
    int recipient;
    unsigned residual;
};

typedef pair<vector<Item>, unsigned> Recipient;
typedef vector<Recipient> Solution;
typedef list<Recipient> SolutionL;
typedef vector<Info> Residual;
typedef pair<unsigned, unsigned> pairI;

bool sortResidual(Recipient a, Recipient b) { return a.second > b.second; }
bool sortItems(Item a, Item b) { return a.size > b.size; }
bool decrescent(Item a, Item b) { return a.size > b.size; }

class CBPP {
    public:
        CBPP(string instanceFilePath);
        CBPP(int capacity) { this->capacity = capacity; };
        void printInstance();
        Item getItem(int item);
        Item getItemIn(int pos) { return itens[pos]; }
        void shuffleItens();
        int getNumItens() { return this->itens.size(); }
        unsigned getCapacity() { return this->capacity; }
        vector<Item> getItens() { return this->itens; }
        void addItem(pairI item);

    private:
        void createInstance(string instanceFilePath);
        int nItens, capacity;
        vector<Item> itens;
};

int params = 3;
// 10
double PARAMS[][3] = {
  {1000, 0.000001, 0.1}, // 0
  {1000, 0.000001, 0.3}, // 1
  {1000, 0.000000001, 0.1}, // 
  {1000, 0.000000001, 0.3}, // 
  {10000, 0.000001, 0.1}, // 4
  {10000, 0.000001, 0.3}, // 5
  {10000, 0.000000001, 0.1}, // 6
  {10000, 0.000000001, 0.3}, // 7
  {10000000, 0.000001, 0.1}, // 8
  {10000000, 0.000001, 0.3}, // 9
  {10000000, 0.000000001, 0.1}, // 
  {10000000, 0.000000001, 0.3}, // 
};

string INSTANCES[57] = {
    "instances/100_50_2_0.txt",
    "instances/100_50_2_1.txt",
    "instances/100_50_2_2.txt",
    "instances/100_50_5_0.txt",
    "instances/100_50_5_1.txt",
    "instances/100_50_5_2.txt",
    "instances/100_50_7_0.txt",
    "instances/100_50_7_1.txt",
    "instances/100_50_7_2.txt",
    "instances/100_200_2_0.txt",
    "instances/100_200_2_1.txt",
    "instances/100_200_2_2.txt",
    "instances/100_200_5_0.txt",
    "instances/100_200_5_1.txt",
    "instances/100_200_5_2.txt",
    "instances/100_200_7_0.txt",
    "instances/100_200_7_1.txt",
    "instances/100_200_7_2.txt",
    "instances/100_400_2_0.txt",
    "instances/100_400_2_1.txt",
    "instances/100_400_2_2.txt",
    "instances/100_400_5_0.txt",
    "instances/100_400_5_1.txt",
    "instances/100_400_5_2.txt",
    "instances/100_400_7_0.txt",
    "instances/100_400_7_1.txt",
    "instances/100_400_7_2.txt",
    "instances/100_750_2_0.txt",
    "instances/100_750_2_1.txt",
    "instances/100_750_2_2.txt",
    "instances/100_750_5_0.txt",
    "instances/100_750_5_1.txt",
    "instances/100_750_5_2.txt",
    "instances/100_750_7_0.txt",
    "instances/100_750_7_1.txt",
    "instances/100_750_7_2.txt",
    "instances/100_1000_2_0.txt",
    "instances/100_1000_2_1.txt",
    "instances/100_1000_2_2.txt",
    "instances/100_1000_5_0.txt",
    "instances/100_1000_5_1.txt",
    "instances/100_1000_5_2.txt",
    "instances/100_1000_7_0.txt",
    "instances/100_1000_7_1.txt",
    "instances/100_1000_7_2.txt",
    "instances/500_50_2_0.txt",
    "instances/500_50_2_1.txt",
    "instances/500_50_5_0.txt",
    "instances/500_50_5_1.txt",
    "instances/500_50_7_0.txt",
    "instances/500_50_7_1.txt",
    "instances/500_200_2_0.txt",
    "instances/500_200_2_1.txt",
    "instances/500_200_5_0.txt",
    "instances/500_200_5_1.txt",
    "instances/500_200_7_0.txt",
    "instances/500_200_7_1.txt"
    // "instances/500_400_2_0.txt",
    // "instances/500_400_2_1.txt",
    // "instances/500_400_5_0.txt",
    // "instances/500_400_5_1.txt",
    // "instances/500_400_7_0.txt",
    // "instances/500_400_7_1.txt",
    // "instances/1000_400_2_0.txt",
    // "instances/1000_400_5_0.txt",
    // "instances/1000_400_7_0.txt"
};

double T_MAX;
double T_MIN;
double R;
int MAX_ITERATIONS = 3000;
int MAX_NO_IMPROVEMENT=5000;
string fileInstancePath;

void initSolution(CBPP &cbpp, Solution &solution);
Avaliation avaliation(Solution &solution, int capacity);
void printSolution(Solution &solution);
void HardBFD(CBPP &cbpp, Solution &solution);
// void neighborhood(CBPP &cbpp, Solution &solution, Residual &residual, Solution &newSolution);
void SA(CBPP &cbpp);

int main() {
    srand(time(NULL));
    try {
        T_MAX = PARAMS[params][0];
        T_MIN = PARAMS[params][1];
        R = PARAMS[params][2];

        cout << "instance;init_recipients;init_avaliation;sa_recipients;sa_avaliation\n";
        // cout << "instance,recipients,avaliation\n";
        for(int i=0; i<57; i++) {
            fileInstancePath = INSTANCES[i];
            CBPP cbpp(INSTANCES[i]);
            SA(cbpp);
        }
    } catch(const char* msg) {
        cout << msg << endl;
    }

    return 0;
}

Avaliation avaliation(Solution &solution, int capacity) {
    int nonEmptyRecipients = 0;
    int numRecipients = solution.size();
    double eval = 0.0;
    for(int r=0; r<numRecipients; r++) {
        unsigned load = 0;
        int numItens = solution[r].first.size();
        if(numItens > 0) {
            nonEmptyRecipients++;
            for(int i=0; i<numItens; i++) {
                load += solution[r].first[i].size;
            }

            double aux = 1.0*load / capacity;
            aux = aux * aux;
            eval += aux;
        }
    }

    eval /= (1.0*nonEmptyRecipients);
    Avaliation auxEval;
    auxEval.eval = 1 - eval;
    auxEval.recipients = nonEmptyRecipients;
    return auxEval;
}

void initSolution(CBPP &cbpp, Solution &solution) {
    cbpp.shuffleItens();
    vector<bool> allocated(cbpp.getNumItens(), false);
    for(int i=0; i<cbpp.getNumItens(); i++) {
        Item item = cbpp.getItemIn(i);
        unsigned itemSize = item.size;
        if(allocated[i]) continue;
        bool findRecipient = false;
        for(int r=0; r<solution.size(); r++) {
            unsigned topItemColor = (solution[r].first.size() == 0 ? -1 : solution[r].first[solution[r].first.size()-1].color);
            if(solution[r].second >= item.size && item.color != topItemColor) {
                unsigned newResidual = solution[r].second - itemSize;
                solution[r].first.push_back(item);
                solution[r].second = newResidual;
                allocated[i] = true;
                findRecipient = true;
                break;
            }
        }

        if(!findRecipient) {
            unsigned residual = cbpp.getCapacity() - itemSize;
            solution.push_back(make_pair(vector<Item>(), residual));
            solution[solution.size()-1].first.push_back(cbpp.getItemIn(i));
        }
    }
}

bool checkViability(Recipient &recipiet, Item &item, int capacity) {
    unsigned itemSize = item.size;
    unsigned itemColor = item.color;
    int newResidual = recipiet.second - itemSize;
    if(newResidual < 0) return false;

    int lastItem = recipiet.first.size() - 1;
    if(lastItem < 0) return true; // sem item
    if(itemColor == recipiet.first[lastItem].color) return false;

    return true;
}

void HardBFD(CBPP &cbpp, Solution &solution) {
    // Solution solution;
    vector<Item> itens = cbpp.getItens();
    sort(itens.begin(), itens.end(), decrescent); // ordem não-crescente
    vector<bool> packed(cbpp.getNumItens(), false);
    for(int i=0; i<itens.size(); i++) {
        bool createNewRecipient = true;
        int minResidual = cbpp.getCapacity() + 1;
        int itemISize = itens[i].size;

        int recipientToPut;
        if(packed[i]) continue;
        for(int r=0; r < solution.size(); r++) {
            int residual = solution[r].second - itemISize;

            if(minResidual > residual && residual >= 0) {
                bool isViable = checkViability(solution[r], itens[i], cbpp.getCapacity()); // checar restrição de tamanho e cor
                if(isViable) {
                    minResidual = residual;
                    recipientToPut = r;
                }
            }
        }

        if(minResidual == cbpp.getCapacity()+1) { // se i não pode ser colocado em nenhum recipiente
            int itemJ = -1;
            int minResidual = cbpp.getCapacity() + 1;
            for(int j=0; j<itens.size(); j++) {
                // j já foi empacotado ou é o mesmo item ou tem cores iguais
                if(packed[j] || j == i || itens[i].color == itens[j].color) continue;

                int itemJSize = itens[j].size;
                int combinedSize = itemISize + itemJSize;
                for(int r=0; r < solution.size(); r++) {
                    int residual = solution[r].second - combinedSize;
                    if(minResidual > residual && residual >= 0) {

                        bool isViable = (itens[i].color != itens[j].color &&
                                         checkViability(solution[r], itens[j], cbpp.getCapacity()));
                        
                        if(isViable) {
                            minResidual = residual;
                            recipientToPut = r;
                            itemJ = j;
                        }
                    }
                }
            }

            if(itemJ != -1) {
                solution[recipientToPut].first.push_back(itens[itemJ]);
                solution[recipientToPut].first.push_back(itens[i]);
                solution[recipientToPut].second -= (itens[itemJ].size + itens[i].size);
                createNewRecipient = false;
                packed[itemJ] = true;
                packed[i] = true;
            }
        } else {
            solution[recipientToPut].first.push_back(itens[i]);
            solution[recipientToPut].second = minResidual;
            packed[i] = true;
            createNewRecipient = false;
        }

        if(createNewRecipient) {
            int residual = cbpp.getCapacity() - itens[i].size;
            solution.push_back(make_pair(vector<Item>(), residual));
            solution[solution.size() - 1].first.push_back(itens[i]);
            packed[i] = true;
            createNewRecipient = false;
        }
    }

    // Avaliation eval = avaliation(solution, cbpp.getCapacity());
    // cout << fileInstancePath << "," << eval.recipients << "," << eval.eval << endl;

    return;
}

void printSolution(Solution &solution) {
    for(int r=0; r<solution.size(); r++) {
        if(solution[r].first.size() == 0) continue;
        cout << "Recipiente: " << r << endl;
        for(int i=0; i<solution[r].first.size(); i++) {
            cout << "\t" << solution[r].first[i].size << " " << solution[r].first[i].color << endl;
        }
        cout << "\tResidual: " << solution[r].second << "\n";
        cout << "\n";
    }
}

void printResidual(Solution &solution) {
    for(int r=0; r<solution.size(); r++) {
        if(solution[r].second == 0) {
            if(solution.size()-1 == r) {
                cout << "\n";
            }

            continue;
        }
        cout << solution[r].second;
        cout << (r == solution.size()-1 ? "\n" : ";");
    }
}

int chooseRecipient(Solution &solution, vector<bool> &visited) {
    int random = rand() % solution.size();
    while(true) {
        if(visited[random]) {
            int start = (random + 1 < solution.size() ? random+1 : 0);
            while(start != random) {
                if(!visited[start]) return start;
                start = (start + 1 < solution.size() ? start+1 : 0);
            }

            return -1;
        } else {
            return random;
        }
    }
}

bool checkViabilitySize(Solution &solution, int r1, int r2, Item i1, Item i2) {
    long residualR1 = solution[r1].second;
    long residualR2 = solution[r2].second;

    residualR1 = residualR1 - i1.size + i2.size;
    residualR2 = residualR2 - i2.size + i1.size;

    if(residualR1 < 0 || residualR2 < 0) return false;
    return true;
}

bool checkViabilityColor(Solution &solution, int r1, int r2, Item i1, Item i2) {
    bool viable1=true, viable2=true;
    for(int i=0; i<solution[r1].first.size(); i++) {
        if(solution[r1].first[i].id == i1.id) {
            unsigned colorBefore = (i - 1 < 0 ? -1 : solution[r1].first[i-1].color);
            unsigned colorAfter(i + 1 == solution[r1].first.size() ? -1 : solution[r1].first[i+1].color);
            if(i2.color != colorBefore && i2.color != colorAfter) {
                viable1 = true;
                break;
            }
        }
    }

    for(int i=0; i<solution[r2].first.size(); i++) {
        if(solution[r2].first[i].id == i1.id) {
            unsigned colorBefore = (i - 1 < 0 ? -1 : solution[r1].first[i-1].color);
            unsigned colorAfter(i + 1 == solution[r1].first.size() ? -1 : solution[r1].first[i+1].color);
            if(i1.color != colorBefore && i1.color != colorAfter) {
                viable2 = true;
                break;
            }
        }
    }

    return (viable1 & viable2);
}

void swapItens(Solution &solution, int r1, int r2, Item i1, Item i2) {
    int indexI1, indexI2;
    for(int i=0; i<solution[r1].first.size(); i++) {
        if(solution[r1].first[i].id == i1.id) {
            indexI1 = i;
            break;
        }
    }

    for(int i=0; i<solution[r2].first.size(); i++) {
        if(solution[r2].first[i].id == i2.id) {
            indexI2 = i;
            break;
        }
    }

    unsigned residualR1 = solution[r1].second;
    unsigned residualR2 = solution[r2].second;

    residualR1 = residualR1 - i1.size + i2.size;
    residualR2 = residualR2 - i2.size + i1.size;

    solution[r1].first[indexI1] = i2;
    solution[r2].first[indexI2] = i1;

    solution[r1].second = residualR1;
    solution[r2].second = residualR2;
}

void reconstruct(CBPP &cbpp, Solution &solution, int bestRecipients) {
    int noImprovement = 0;

    SolutionL bestKeep;
    Solution bestReconstruction;
    int currBestRecipients = bestRecipients;
    while(true) {
        Solution reconstructSolution;
        SolutionL keepSolution = SolutionL(solution.begin(), solution.end());
        SolutionL::iterator it = keepSolution.begin();
        int residual = cbpp.getCapacity() * 0.1; // obrigatório pegar recipientes mais vazios
        int maxDestroy = solution.size() * 0.3, count = 0;

        while(it != keepSolution.end()) {
            if(it->second >= residual) { // obrigatório pegar recipientes mais vazios
                reconstructSolution.push_back(*it);
                it = keepSolution.erase(it);
            } else { // destruição aleatória
                int putIn  = rand() % 2;
                if(putIn == 1 && count < maxDestroy) {
                    reconstructSolution.push_back(*it);
                    it = keepSolution.erase(it);
                    count++;
                } else {
                    it++;
                }
            }
        }

        vector<Item> itens;
        for(int r=0; r < reconstructSolution.size(); r++) {
            for(int i=0; i < reconstructSolution[r].first.size(); i++) {
                itens.push_back(reconstructSolution[r].first[i]);
            }
        }

        sort(itens.begin(), itens.end(), sortItems);

        int newRecipients = keepSolution.size();
        reconstructSolution = Solution();
        
       
        for(int i=0; i<itens.size(); i++) {
            bool createNewRecipient = true;
            for(int r=0; r<reconstructSolution.size(); r++) {
                if(checkViability(reconstructSolution[r], itens[i], cbpp.getCapacity())) {
                    createNewRecipient = false;
                    reconstructSolution[r].first.push_back(itens[i]);
                    reconstructSolution[r].second -= itens[i].size;
                    break;
                }
            }

            if(createNewRecipient) {
                reconstructSolution.push_back(make_pair(vector<Item>(), cbpp.getCapacity()));
                reconstructSolution[reconstructSolution.size()-1].first.push_back(itens[i]);
                reconstructSolution[reconstructSolution.size()-1].second -= itens[i].size;
            }   
        }

        newRecipients += reconstructSolution.size();

        if(newRecipients < currBestRecipients) {
            bestReconstruction = reconstructSolution;
            bestKeep = keepSolution;
            currBestRecipients = newRecipients;
            noImprovement = 0;
        } else {
            noImprovement++;
        }

        if(noImprovement == 2000) { break; }
    }

    if(bestKeep.size() > 0 && bestReconstruction.size() > 0) {
        solution = Solution();
        for(SolutionL::iterator it=bestKeep.begin(); it != bestKeep.end(); it++) {
            solution.push_back(*it);
        }

        for(int i=0; i<bestReconstruction.size(); i++) {
            solution.push_back(bestReconstruction[i]);
        }
    }
}

void SA(CBPP &cbpp) {
    Solution bestSolution;
    Avaliation eval;
    // initSolution(cbpp, bestSolution);
    HardBFD(cbpp, bestSolution);
    // cout << "IS;" << fileInstancePath << ";";
    // printResidual(bestSolution);

    eval = avaliation(bestSolution, cbpp.getCapacity());
    double initAvaliation = eval.eval;
    unsigned initRecipient = eval.recipients;
    unsigned bestRecipient = eval.recipients;
    double bestAvaliation = eval.eval;
    double t = T_MAX;
    int iterations;
    for(; t > T_MIN;) {
        int noImprovement = 0;
        Solution currSolution = bestSolution;
        double currAvaliation = bestAvaliation;
        unsigned currRecipients = bestRecipient;
        for(iterations=0; iterations <= MAX_ITERATIONS; iterations++) {
            sort(currSolution.begin(), currSolution.end(), sortResidual);
            int numRecipients = currSolution.size();
            for(int r1=0; r1 < numRecipients; r1++) {
                if(noImprovement == 1000) break;
                if(currSolution[r1].first.size() == 0) continue; // sem itens

                vector<Item> itensR1 = currSolution[r1].first;
                sort(itensR1.begin(), itensR1.end(), sortItems);
                Item chooseI1 = itensR1[0];
                Item chooseI2;
                vector<bool> visited(numRecipients, false);
                visited[r1] = true;
                int r2;
                bool viable1, viable2;
                while(true) {
                    viable1 = false;
                    viable2 = false;
                    r2 = chooseRecipient(currSolution, visited);
                    if(r2 == -1) break;
                    visited[r2] = true;

                    vector<Item> itensR2 = currSolution[r2].first;
                    unsigned maxSize = 0;
                    int indexI2 = -1;
                    for(int i2=0; i2<itensR2.size(); i2++) {
                        if(itensR2[i2].size < chooseI1.size) {
                            if(itensR2[i2].size > maxSize) {
                                indexI2 = i2;
                                maxSize = itensR2[i2].size;
                            }
                        }
                    }
                    if(indexI2 == -1) continue;
                    chooseI2 = itensR2[indexI2];
                    viable1 = checkViabilitySize(currSolution, r1, r2, chooseI1, chooseI2);
                    viable2 = checkViabilityColor(currSolution, r1, r2, chooseI1, chooseI2);

                    if(viable1 && viable2) break;
                }

                if(!viable1 || !viable2) continue;

                Solution neighbour = currSolution;
                swapItens(neighbour, r1, r2, chooseI1, chooseI2);
                Avaliation eval = avaliation(neighbour, cbpp.getCapacity());

                // Melhor encontrado até o momento
                if(eval.eval < bestAvaliation) {
                    reconstruct(cbpp, neighbour, eval.recipients);
                    Avaliation eval = avaliation(neighbour, cbpp.getCapacity());
                    bestSolution = neighbour;
                    bestRecipient = eval.recipients;
                    bestAvaliation = eval.eval;
                    currSolution = neighbour;
                    currAvaliation = eval.eval;
                    currRecipients = eval.recipients;
                    noImprovement = 0;
                    break;
                } else { // Não é melhor encontrado
                    // Aceito se vizinho melhor
                    if(currAvaliation > eval.eval) {
                        currSolution = neighbour;
                        currRecipients = eval.recipients;
                        currAvaliation = eval.eval;
                        noImprovement = 0;
                        break;
                    }

                    // Aceita com probabilidade
                    double p = -1.0*(eval.eval - currAvaliation) / t;
                    p = exp(p);
                    double random = rand() / (double) RAND_MAX;
                    if(random < p) {
                        currSolution = neighbour;
                        currRecipients = eval.recipients;
                        currAvaliation = eval.eval;
                        noImprovement++;
                    } else {
                        noImprovement++;
                        continue;
                    }
                }
            }
        }

        t *= R;
    }

    cout <<  fileInstancePath << ";" << initRecipient << ";" << initAvaliation << ";" << bestRecipient << ";" << bestAvaliation << endl;
}

CBPP::CBPP(string fileInstancePath) {
    createInstance(fileInstancePath);
}

void CBPP::shuffleItens() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(this->itens.begin(), this->itens.end(), default_random_engine(seed));
}

void CBPP::createInstance(string fileInstancePath) {
    ifstream fin;
    fin = ifstream(fileInstancePath, ifstream::in);
    if(!fin.is_open()) {
        throw("Error opening file: " + fileInstancePath);
    }

    fin >> this->nItens;
    fin >> this->capacity;
    int idItem = 0;
    for(int i=0; i < this->nItens; i++) {
        unsigned v[2];
        for(int j=0; j<2; j++) {
            fin >> v[j];
        }

        Item item;
        item.id = idItem;
        item.size = v[0];
        item.color = v[1];
        itens.push_back(item);
        idItem++;
    }

    fin.close();
}

Item CBPP::getItem(int item) {
    return itens[item];
}

void CBPP::printInstance() {
    cout << this->nItens << "\n" << this->capacity << "\n";
    for(int i=0; i<this->nItens; i++) {
        cout << itens[i].size << " " << itens[i].color << endl;
    }
}