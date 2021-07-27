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
    for (long unsigned int i = 0; i < len; i++)
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
    return res.str();
}

string bw_compress(string t_seq, int* n, int* p) {
    // Finding the $
    for (long unsigned int i = 0; i < t_seq.length(); i++) {
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
    for (long unsigned int i = 0; i < t_seq.length(); i++) {
        if (t_seq[i] == 'C')
            tmp ^= 0b01000000;

        if (t_seq[i] == 'G')
            tmp ^= 0b10000000;

        if (t_seq[i] == 'T')
            tmp ^= 0b11000000;

        // Reset current byte
        if (i % 4 == 3) {
            res.push_back((char) tmp);
            tmp = 0;
        }

        tmp = tmp >> 2;
    }
    return res;
}

int main(int argc, char** argv) {
    // Verifying that there is enough arguments
    if (argc < 4 && argc > 8) {
        throw (invalid_argument("Too many or too few arguments, RTFM"));
    }

    // Verify is option compress is here or not
    bool compress = false;
    bool progressive = false;
    int k = -1;
    bool checked[argc];
    for (int i = 1; i < argc; i++) {
        checked[i] = false;
        if ((string) argv[i] == "--compress") {
            compress = true;
            checked[i] = true;
        }
        else if ((string) argv[i] == "--progressive") {
            if (i == argc - 1) {
                throw (invalid_argument("Missing k after --progressive"));
            }
            progressive = true;
            k = atoi(argv[i + 1]);
            checked[i] = true;
            checked[i + 1] = true;
        }
    }

    // Arguments searching
    int infile_pos = -1;
    int outfile_pos = -1;
    int f_pos = -1;

    for (int i = 1; i < argc; i++) {
        if (!checked[i]) {
            if (infile_pos == -1)
                infile_pos = i;
            else if (outfile_pos == -1)
                outfile_pos = i;
            else if (f_pos == -1)
                f_pos = i;
        }
    }

    if (infile_pos == -1 || outfile_pos == -1 || f_pos == -1) {
        throw (invalid_argument("Too few arguments, RTFM"));
    }

    char* infile = argv[infile_pos];
    char* outfile = argv[outfile_pos];
    int f = atoi(argv[f_pos]);

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

    // Sequence read from fasta file
    string sequence = read_fasta(pInfile);

    // Vector containing the list of indexes
    vector<vector<int>> indexes = vector<vector<int>>();

    // Needed if we have --progressive
    vector<string> sequence_split = vector<string>();

    // Vector containing the transformed sequences
    vector<string> t_sequences = vector<string>();
    if (k != -1) {
        
    } else {
        vector<int> tmp = vector<int>();
        indexes.push_back(tmp);
        sequence_split.push_back(sequence);
    }

    for (int i = 0; i < sequence_split.size(); i++) {
        t_sequences.push_back(Burrows_Weller(sequence_split[i], &indexes[i]));
    }

    // Creating output flags
    int n = 0;
    int p = 0;
    int c = compress ? 1 : 0;
    if (k == -1) {
        if (c) {
            string t = bw_compress(t_sequences[0], &n, &p);
            pOutfile << c << ' ' << n << ' ' << p << ' ' << f << endl;
            for (unsigned long int i = 0; i < t_sequences[0].length(); i += f) {
                pOutfile << indexes[0][i];
                if (i != t_sequences[0].length() - 1 && i + f < t_sequences[0].length())
                    pOutfile << ',';
            }
            //if ((t_sequences[0].length() - 1) % f != 0)
            //    pOutfile << indexes[0][t_sequences[0].length() - 1];
            pOutfile << endl;
            pOutfile << t;
        } else {
            pOutfile << c << ' ' << n << ' ' << p << ' ' << f << endl;
            for (unsigned long int i = 0; i < t_sequences[0].length(); i += f) {
                pOutfile << indexes[0][i];
                if (i != t_sequences[0].length() - 1 &&  i + f < t_sequences[0].length())
                    pOutfile << ',';
            }
            //if ((t_sequences[0].length() - 1) % f != 0)
            //    pOutfile << indexes[0][t_sequences[0].length() - 1];
            pOutfile << endl;
            pOutfile << t_sequences[0];
        }
    }

    // Closing files
    pInfile.close();
    pOutfile.close();

    return 1;
}
