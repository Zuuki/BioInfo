#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

vector <string> read_fasta(ifstream& pFile) {
    vector <string> res = vector<string>();

    string line;
    getline(pFile, line);
    while (getline(pFile, line)) {
        res.push_back(line);
    }

    return res;
}

string Burrows_Weller(string sequence) {
    string matrix[sequence.length() + 1];
    stringstream ss;
    ss << "$" << sequence;
    string temp = ss.str();
    for (long unsigned int i = 0; i < temp.length(); i++) {
        matrix[i] = temp;
        temp = temp.substr(1, temp.length() - 1) + temp.substr(0, 1);
    }

    for (long unsigned int i = 0; i < temp.length(); i++) {
        cout << matrix[i] << "\n";
    }

    return temp;
}

int main(int argc, char** argv) {
    if (argc != 4 && argc != 5) {
        throw (invalid_argument("Too many or too few arguments, 3 required (4 with option --compress)"));
    }

    bool compress = false;
    if (argc == 5) {
        if (argv[1] == "--compress") {
            compress = true;
        } else {
            throw (invalid_argument("Option not recognized, only --compress is handled"));
        }
    }

    char* infile = argv[1 + (argc == 5)];
    char* outfile = argv[2 + (argc == 5)];
    char* f = argv[3 + (argc == 5)];

    ifstream pInfile(infile);
    if (!pInfile.is_open()) {
        stringstream error;
        error << "File: \"" << infile << "\" does not exist";
        throw (invalid_argument(error.str()));
    }
    ofstream pOutfile(outfile);
    if (!pOutfile.is_open()) {
        stringstream error;
        error << "File: \"" << outfile << "\" could not be created";
        throw (invalid_argument(error.str()));
    }

    vector<string> sequences = read_fasta(pInfile);
    string bw_transformee = Burrows_Weller(sequences[0]);

    pInfile.close();
    pOutfile.close();
    return 1;
}
