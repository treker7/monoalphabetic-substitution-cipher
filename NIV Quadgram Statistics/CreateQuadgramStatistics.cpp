#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char *argv[]){
	string bible;
	cin >> bible;	
	long length = bible.length() - 3;
	
	map<string, int> quadgramCounts;	
	string currQuadgram;
	for(long i = 0; i < length; i++){
		currQuadgram = bible.substr(i, 4);
		quadgramCounts[currQuadgram]++;
	}

	for(auto iter = quadgramCounts.begin(); iter != quadgramCounts.end(); iter++){
		cout << iter->first << " " << iter->second << endl;
	}	
	return 0;
}