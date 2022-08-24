#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main(int argc, char *argv[]){
	string curr;
	while(cin >> curr){
		for(int i = 0; i < curr.length(); i++){
			if(isalpha(curr[i])){
				cout.put(tolower(curr[i]));
			}
		}
	}
	
	return 0;
}