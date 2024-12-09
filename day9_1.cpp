#include <iostream>
#include <string>
#include <vector>
using namespace std;

void removeFragments(vector<long long>& memLayout, long long l, long long r){
    if(l >= r) return;
    while(memLayout[l] != -1) l++;
    while(memLayout[r] == -1) r--;
    if(l < r){
        swap(memLayout[l], memLayout[r]);
        removeFragments(memLayout, l+1, r-1);   
    }
}

long long getChecksum(vector<long long>& memLayout){
    long long checksum = 0;
    for(long long i = 0; i < memLayout.size(); i++){
        if(memLayout[i] == -1) break;
        checksum += memLayout[i]*i;
    }
    return checksum;
}

int main() {
    string line;
    getline(cin, line);
    
    vector<long long> memLayout;
    
    long long idx = 0;
    for(long long i = 0; i < line.length(); i++){
        if(i%2 == 0){//file
            long long filesize = line[i]-'0';
            for(long long j = 0; j < filesize; j++) memLayout.push_back(idx);
            idx++;
        }else{//free space
            long long freesize = line[i]-'0';
            for(long long j = 0; j < freesize; j++) memLayout.push_back(-1);
        }
    }
    
    removeFragments(memLayout, 0, memLayout.size()-1);

    long long checksum = getChecksum(memLayout);
    cout << "Final result: " << checksum << endl;

    return 0;
}
