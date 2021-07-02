#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

string read_fasta(ifstream& pFile) {
    stringstream ss;
    string line;

    getline(pFile, line);
    while (getline(pFile, line)) {
        if (line[line.length() - 1] == '\n')
            ss << line.substr(0, line.length() - 1);
        else
            ss << line;
    }

    return ss.str();
}

string Burrows_Weller(string sequence, vector<int>* index) {
    string matrix[sequence.length() + 1];
    stringstream ss;
    ss << sequence << "$";
    string temp = ss.str();
    auto len = temp.length();

    // Index intiation
    for (int i = 0; i < len; i++)
        index->push_back(i);

    // Create all possible shiftings
    for (long unsigned int i = 0; i < len; i++) {
        matrix[i] = temp;
        temp = temp.substr(1, len - 1) + temp.substr(0, 1);
    }

    string suffixes[len];
    // Index calculation
    for (long unsigned int i = 0; i < len; i++) {
        for (long unsigned int j = 0; j < len; j++) {
            if (matrix[i][j] == '$') {
                suffixes[i] = matrix[i].substr(0, j + 1);
                cout << matrix[i] << " | " << suffixes[i] << endl;
                break;
            }
        }
    }

    // Alphabetical sort
    for (long unsigned int i = 0; i < len - 1; i++) {
        for (long unsigned int j = 0; j < len - i - 1; j++) {
            if (matrix[j] > matrix[j + 1]) {
                auto t = matrix[j];
                matrix[j] = matrix[j + 1];
                matrix[j + 1] = t;
            }
            if (suffixes[j] > suffixes[j + 1]) {
                auto t = suffixes[j];
                auto i = (*index)[j];
                suffixes[j] = suffixes[j + 1];
                suffixes[j + 1] = t;
                (*index)[j] = (*index)[j + 1];
                (*index)[j + 1] = i;
            }
        }
    }

    // Result
    stringstream res;
    for (long unsigned int i = 0; i < len; i++) {
        res << matrix[i][len - 1];
    }
    cout << "res.str(): " << res.str() << endl;
    return res.str();
}

string bw_compress(string t_seq, int* n, int* p) {
    // Finding the $
    for (unsigned int i = 0; i < t_seq.length(); i++) {
        if (t_seq[i] == '$') {
            *n = i;
            t_seq = t_seq.substr(0, i) + t_seq.substr(i + 1, t_seq.length());
            break;
        }
    }

    // Adding A's if needed for compression
    while (t_seq.length() % 4 != 0) {
        t_seq.push_back('A');
        *p += 1;
    }

    string res = "";
    uint8_t tmp = 0;
    cout << "t_seq: " << t_seq << endl;
    cout << t_seq.length() << endl;
    for (long unsigned int i = 0; i < t_seq.length(); i++) {
        // Tag the '$' position
        if (t_seq[i] == 'A')
            tmp |= 0b00;

        if (t_seq[i] == 'C')
            tmp |= 0b01;

        if (t_seq[i] == 'G')
            tmp |= 0b10;

        if (t_seq[i] == 'T')
            tmp |= 0b11;

        cout << (int) tmp << endl;

        // Reset current byte
        if (i % 4 == 3) {
            res.push_back((char) tmp);
            tmp = 0;
        }

        tmp = tmp << 2;
    }
    return res;
}

int main(int argc, char** argv) {
    // Verify is there is enough arguments
    if (argc != 4 && argc != 5) {
        throw (invalid_argument("Too many or too few arguments, 3 required (4 with option --compress)"));
    }

    // Verify is option compress is here or not
    bool compress = false;
    if (argc == 5) {
        if ((string) argv[1] == "--compress") {
            compress = true;
        } else {
            throw (invalid_argument("Option not recognized, only --compress is handled"));
        }
    }

    char* infile = argv[1 + (argc == 5)];
    char* outfile = argv[2 + (argc == 5)];
    char* f = argv[3 + (argc == 5)];

    // Opening FASTA file
    ifstream pInfile(infile);
    if (!pInfile.is_open()) {
        stringstream error;
        error << "File: \"" << infile << "\" does not exist";
        throw (invalid_argument(error.str()));
    }

    // Opening output file
    ofstream pOutfile(outfile);
    if (!pOutfile.is_open()) {
        stringstream error;
        error << "File: \"" << outfile << "\" could not be created";
        throw (invalid_argument(error.str()));
    }

    string sequence = read_fasta(pInfile);
    vector<int> index = vector<int>();
    string t_sequence = Burrows_Weller(sequence, &index);

    // Creating output flags
    int n = 0;
    int p = 0;
    int c = compress ? 1 : 0;

    if (c) {
        string t = bw_compress(t_sequence, &n, &p);
        pOutfile << c << ' ' << n << ' ' << p << ' ' << f << endl;
        for (unsigned long int i = 0; i < t_sequence.length(); i++) {
            pOutfile << index[i];
            if (i != t_sequence.length() - 1)
                pOutfile << ',';
        }
        pOutfile << endl;
        pOutfile << t << endl;
    } else {
        pOutfile << c << ' ' << n << ' ' << p << ' ' << f << endl;
        for (unsigned long int i = 0; i < t_sequence.length(); i++) {
            pOutfile << index[i];
            if (i != t_sequence.length() - 1)
                pOutfile << ',';
        }
        pOutfile << endl;
        pOutfile << t_sequence << endl;
    }

    // Closing files
    pInfile.close();
    pOutfile.close();

    return 1;
}
