#include "CaesarSub.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]){
	if(argc < 3 || argc > 4){
		cout << "Usage: " << argv[0] << " <corpus_filename> <ciphertext> [num_iterations]" << endl;
		return -1;
	}
	string shark = "\n                                 ,-\n                               ,'::|\n                              /::::|\n                            ,'::::o\\    \t                          _.. \n         ____........-------rm -rf /*    \t\t               ,-' /\n _.--\"\"\"\". . . .      .   .  .  .  \"\"`-._                           ,-' .;'\n<. - :::::o......  ...   . . .. . .  .  .\"\"--._                  ,-'. .;'\n `-._  ` `\":`:`:`::||||:::::::::::::::::.:. .  \"\"--._ ,'|     ,-'.  .;'\n     \"\"\"_=--       //'{};.. ````:`:`::::::::::.:.:.:. .`-`._-'.   .;'\n         \"\"--.__     \\(       \\               ` ``:`:``:::: .   .;'\n                \"\\\"\"--.:-.     `.                             .:/\n\t          \\. /    `-._   `.\"\"-----.,-..::(--\"\".\\\"\"`.  `:\\ \n  \t           `P         `-._ \\          `-:\\          `. `:\\\n   \t\t                   \"\"            \"            `-._)";
	cout << shark << endl << endl;

	string corpusFile(argv[1]), ciphertext(argv[2]), input = "";
	unsigned int numIterations = (argc == 4) ? atoi(argv[3]) : 200;
	double ic;

	cout << "\n\t[Automated Monoalphabetic Substition Cipher Cryptanalysis]\n";
	ic = CaesarSubSolve::calculateIC(ciphertext);
	cout << "\t[Index of Coincidence (IC): " <<  ic << "]\n\n";

	if(abs(ic - IC_ENGLISH) > .015)
		cout << "Warning: Index of Coincidence differs substantially from " << IC_ENGLISH << ".\n\n";

	CaesarSubSolve caesarSubSolver(corpusFile);

	cout << "\n\t[Possible Decryptions]\n\n";
	for(unsigned int i = 0; i < numIterations; i++){
		caesarSubSolver.decrypt(ciphertext);
	}
	return 0;
}