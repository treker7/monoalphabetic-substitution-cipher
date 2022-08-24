#include "CaesarSub.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>

/*n = 26 -> sum(i, {i, 1, n - 1}) = ((n - 1)/2)(n) = 325*/
#define MAX_SWAPS 325

using namespace std;

bool CaesarSubSolve::seededRanGenerator = false;

string removeNonAlpha(const string& str){
	string ret = "";
	for(unsigned long i = 0; i < str.length(); i++){
		if(((str[i] >= 'a') && (str[i] <= 'z')) || ((str[i] >= 'A') && (str[i] <= 'Z'))){//isalpha(str[i])
			ret += tolower(str[i]);
		}
	}
	return ret;
}
string swap(const string& str, int i, int j){
	string ret(str);
	char c = ret[j];
	ret[j] = ret[i];
	ret[i] = c;
	return ret;
}

CaesarSubSolve::CaesarSubSolve(const string& corpusFile) : i1(0), i2(1) {
	if(!seededRanGenerator){
		srand(time(0));
		seededRanGenerator = true;
	}
	
	ifstream in;
	in.open(corpusFile);
	
	if(in.fail()){
		cout << "Error opening corpus file: " << corpusFile << endl;
		cout << "Program will exit." << endl;
		exit(-1);
	}
	cout << "Processing The Corpus File..." << endl;
	string corpus = "", line;
	while(in >> line){		
		corpus += removeNonAlpha(line);		
	}
	in.close();
	cout << "Counting Quadgram Frequencies..." << endl;
	string currQuadgram;
	long length = corpus.length() - 3;	
	for(long i = 0; i < length; i++){
		currQuadgram = corpus.substr(i, 4);
		quadgramCounts[currQuadgram] += 1;
	}	
	for(auto iter = quadgramCounts.begin(); iter != quadgramCounts.end(); iter++){
		iter->second = log(iter->second / quadgramCounts.size());
	}
}
string CaesarSubSolve::getRandomKey(){
	if(!seededRanGenerator){
		srand(time(0));
		seededRanGenerator = true;
	}
	string key = "";

	vector<char> alphabet;
	for(char c = 'a'; c <= 'z'; c++){
		alphabet.push_back(c);
	}

	int i;
	while(!alphabet.empty()){
		i = rand() % alphabet.size();
		key += alphabet[i];
		alphabet.erase(alphabet.begin() + i);
	}
	return key;
}
string CaesarSubSolve::decode(const string& ciphertext, const string& key){
	string decryption = "";
	for(unsigned int i = 0; i < ciphertext.length(); i++){
		if(((ciphertext[i] >= 'a') && (ciphertext[i] <= 'z')) || ((ciphertext[i] >= 'A') && (ciphertext[i] <= 'Z'))){//isalpha(ciphertext[i])
			decryption += key[tolower(ciphertext[i]) - 'a'];
		}
	}
	return decryption;
}
double CaesarSubSolve::calculateIC(const string& ciphertext){//calculate the index of coincidence (IC)
	string text = removeNonAlpha(ciphertext);
	int counts[26];
	unsigned int i;
	double ic = 0;	

	for(i = 0; i < 26; i++)
		counts[i] = 0;
	for(i = 0; i < text.length(); i++){
		counts[(text[i] - 'a')]++;
	}

	for(i = 0; i < 26; i++){
		ic += counts[i] * (counts[i] - 1);
	}
	ic /= (text.length() * (text.length() - 1));
	return ic;
}
double CaesarSubSolve::evaluateFitness(const string& text){
	return evaluateFitness2(removeNonAlpha(text));
}
double CaesarSubSolve::evaluateFitness2(const string& text){
	double fitness = 0;
	double noQuadgram = (1.05 * log(1.0 / quadgramCounts.size()));//if the quadgram doesn't appear in the corpus at all, then it's unlikely that the text is English (assuming the corpus is a good sampling of English)

	string currQuadgram;
	int length = text.length() - 3;	
	for(int i = 0; i < length; i++){
		currQuadgram = text.substr(i, 4);
		auto iter = quadgramCounts.find(currQuadgram);
		if(iter == quadgramCounts.end()){
			fitness += noQuadgram;
		}else{
			fitness += iter->second;
		}
	}
	return fitness;
}
string CaesarSubSolve::mutateKey(const string& key){
	string mutatedKey = swap(key, i1, i2);

	if(i2 < 25){
		i2++;		
	}else{
		if(i1 < 24){
			i1++;			
		}else{//reset
			i1 = 0;	
		}
		i2 = (i1 + 1);
	}
	return mutatedKey;
}
string CaesarSubSolve::decrypt(const string& ciphertext) {
	string bestKey = getRandomKey(), currKey;
	double bestFitness = evaluateFitness2(decode(ciphertext, bestKey)), currFitness;
	int numSwaps = 0;

	while(numSwaps < MAX_SWAPS){
		currKey = mutateKey(bestKey);
		currFitness = evaluateFitness2(decode(ciphertext, currKey));
		if(currFitness > bestFitness){
			bestKey = currKey;
			bestFitness = currFitness;
			numSwaps = 0;
		}else{
			numSwaps++;
		}
	}
	cout << "KEY: " << bestKey << endl;
	cout << "FITNESS: " << bestFitness << endl;
	cout << decode(ciphertext, bestKey) << endl << endl;

	return decode(ciphertext, bestKey);
}