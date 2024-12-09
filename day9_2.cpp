#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


void moveFiles(vector<long long>& memLayout, long long r, map<long long, vector<long long>>& freeBlocks, int currIdxToSee) {
    if (r <= 0 || currIdxToSee == -1) return;

    while (memLayout[r] == -1) r--;

    while (memLayout[r] > currIdxToSee) r--;

    long long rend = r, rstart = r;

    while (rstart > 0 && memLayout[rstart - 1] == memLayout[rstart]) {
        rstart--;
    }

    long long block_size = rend - rstart + 1;
    
    long long best_idx = -1;
    long long best_size = -1;

    for (long long size = block_size; size <= freeBlocks.rbegin()->first; size++) {
        if (freeBlocks.find(size) != freeBlocks.end()) {
            for (long long i : freeBlocks[size]) {
                if (i < rstart) {
                    if (best_idx == -1 || i < best_idx) {
                        best_idx = i;
                        best_size = size;
                    }
                }
            }
        }
    }

    if (best_idx != -1) {
        long long target_idx = best_idx;
        for (long long j = rstart; j <= rend; j++) {
            memLayout[target_idx] = memLayout[j];
            memLayout[j] = -1;
            target_idx++;
        }

        freeBlocks[best_size].erase(std::remove(freeBlocks[best_size].begin(), freeBlocks[best_size].end(), best_idx), freeBlocks[best_size].end());
        if (freeBlocks[best_size].empty()) freeBlocks.erase(best_size);

        long long leftover_space = best_size - block_size;
        if (leftover_space > 0) {
            freeBlocks[leftover_space].push_back(target_idx);
        }

        moveFiles(memLayout, rstart - 1, freeBlocks, currIdxToSee - 1);
    } else {
        moveFiles(memLayout, rstart - 1, freeBlocks, currIdxToSee - 1);
    }
}

long long getChecksum(vector<long long>& memLayout) {
    long long checksum = 0;
    for (long long i = 0; i < memLayout.size(); i++) {
        if (memLayout[i] == -1) continue;
        checksum += memLayout[i] * i;
    }
    return checksum;
}

int main() {
    string line;
    getline(cin, line);
    
    vector<long long> memLayout;
    map<long long, vector<long long>> freeBlocks;
    
    long long idx = 0;
    for (long long i = 0; i < line.length(); i++) {
        if (i % 2 == 0) { // file
            long long filesize = line[i] - '0';
            for (long long j = 0; j < filesize; j++) {
                memLayout.push_back(idx);
            }
            idx++;
        } else { // free space
            long long freesize = line[i] - '0';
            freeBlocks[freesize].push_back(memLayout.size());
            for (long long j = 0; j < freesize; j++) {
                memLayout.push_back(-1);
            }
        }
    }
    
    moveFiles(memLayout, memLayout.size() - 1, freeBlocks, idx - 1);

    long long checksum = getChecksum(memLayout);
    cout << "Final checksum: " << checksum << endl;

    return 0;
}
