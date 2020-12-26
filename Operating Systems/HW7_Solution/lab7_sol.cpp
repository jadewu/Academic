// Code for FIFO Algorithm

#include <malloc.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

int *frame, traceSize;
vector<int> pageTrace;

//Function to generate the random trace sequence
void genRandomTrace(int highestPageNumber, int n){
    for (int i=0; i<n; i++){
        pageTrace[i] = rand() % (highestPageNumber + 1);
    }
}

int fifo(std::vector<int>& trace, int f) {
    queue<int> q;
    unordered_set<int> seen;
    int len = trace.size();
    int fault = 0;  // count of fault
    for(int i = 0; i < len; i++) {
        /* Check if trace[i] is in queue */
        if(seen.find(trace[i]) != seen.end())
            continue;
        /* If not in memory, increase page fault count */
        fault++;
        if(q.size() == f) {
            seen.erase(q.front());
            q.pop();
            q.push(trace[i]);
            seen.insert(trace[i]);
        }
        else {
            seen.insert(trace[i]);
            q.push(trace[i]);
        }
    }
    return fault;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int highestPageNumber = pow(2, k) - 1;
    int frameMaxIndex = pow(2, k);
    pageTrace.resize(n);
    traceSize = n;

    /* Initialize random seed */
    srand(time(NULL));
    genRandomTrace(highestPageNumber, n);

    std::vector<int> faultcount;
    for(int f = 4; f <= pow(2, k); f++) {
        faultcount.push_back(fifo(pageTrace, f));
    }

    for(auto num : faultcount) {
        std::cout << num << " ";
    }
    
    std::cout << std::endl;
    for(int i = 0; i < faultcount.size()-1; i++) {
        if(faultcount[i] < faultcount[i+1]) {
            std::cout << "Belady Anomaly found at " << std::endl;
            std::cout << faultcount[i] << " " << faultcount[i+1] << std::endl;
            break;
        }
    }

    return 0;
}