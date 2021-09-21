#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class first_to_last {
public:
    first_to_last(char f, char l, int _ftl)
    { first = f; last = l; ftl = _ftl; }

    char first;
    char last;
    int ftl;
    int first_occ;
    int last_occ;
};

string read_file(ifstream& pFile, int* c, int* n, int* p, int* f, vector<int>* index) {
    stringstream ss;
    string line;
    int line_nb = 0;

    while (getline(pFile, line)) {
        if (line_nb == 0) {
            string tmp = "";
            int var = 0;
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {
                    if (var == 0) {
                        *c = stoi(tmp);
                    } if (var == 1) {
                        *n = stoi(tmp);
                    } if (var == 2) {
                        *p = stoi(tmp);
                    } var += 1; tmp = "";
                } else {
                    tmp.push_back(line[i]);
                }
            } *f = stoi(tmp);
        } else if (line_nb == 1) {
            string tmp = "";
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ',') {
                    index->push_back(stoi(tmp));
                    tmp = "";
                } else {
                    tmp.push_back(line[i]);
                }
            }
        } else {
            return line;
        }
        line_nb += 1;
    }
    return "";
}

string decompress(string sequence, int n, int p) {
    stringstream res;
    int index = 0;
    for (int i = 0; i < sequence.length(); i++) {
        uint8_t act = sequence[i];
        for (int j = 0; j < 4; j++) {
            if (i == sequence.length() - 1 && j + p == 4) {
                return res.str();
            } if (index == n) {
                res << '$';
            } if ((act & 0b00000011) == 0b00000000) {
                res << 'A';
            } else if ((act & 0b00000011) == 0b00000001) {
                res << 'C';
            } else if ((act & 0b00000011) == 0b00000010) {
                res << 'G';
            } else if ((act & 0b00000011) == 0b00000011) {
                res << 'T';
            } act = act >> 2; index++;
        }
    }
    return res.str();
}

vector<string> decipher_bwt(string sequence) {
    vector<string> matrix = vector<string>();

    int seq_len = sequence.length();
    int mat_len = 1;

    for (int i = 0; i < seq_len; i++) {
        matrix.push_back(string(1, sequence[i]));
    }

    while (true) {
        for (int i = 0; i < seq_len - 1; i++) {
            for (int j = 0; j < seq_len - i - 1; j++) {
                    if (matrix[j] > matrix[j + 1]) {
                        string tmp = matrix[j];
                        matrix[j] = matrix[j + 1];
                        matrix[j + 1] = tmp;
                    }
            }
        } if (seq_len == mat_len) {
            return matrix;
        } for (int i = 0; i < seq_len; i++) {
            matrix[i].insert(0, string(1, sequence[i]));
        } mat_len += 1;
    }

    return matrix;
}

vector<first_to_last> build_ftl(vector<string> matrix) {
    vector<first_to_last> res = vector<first_to_last>();
    for (int i = 0; i < matrix.size(); i++) {
        char first = matrix[i][0];
        char last = matrix[i][matrix.size() - 1];
        int ftl = 0;
        string tmp = matrix[i].substr(matrix.size() - 1, matrix.size()) + matrix[i].substr(0, matrix.size() - 1);
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[j] == tmp) {
                ftl = j;

                break;
            }
        } res.push_back(first_to_last(first, last, ftl));
        int occ = 1;
        for (int k = 0; k < matrix.size(); k++) {
            if (matrix[i][k] == '$') {
                res[i].last_occ = occ;
                break;
            } else if (matrix[i][k] == last) {
                occ++;
            }
        }
    } return res;
}

vector<int> bw_search(vector<first_to_last> ftl, string sub_seq) {
    vector<int> res = vector<int>();
    int top = 0;
    int bot = ftl.size() - 1;

    for (int i = sub_seq.size() - 1; i >= 0; i--) {
        int temp_top = -1;
        int temp_bot = -1;
        for (int j = top; j <= bot; j++) {
            if (ftl[j].last == sub_seq[i]) {
                if (temp_top == -1) {
                    temp_top = ftl[j].ftl;
                } if (temp_bot < ftl[j].ftl) {
                    temp_bot = ftl[j].ftl;
                }
            }
        } if (temp_top == -1 || temp_bot == -1) {
            return res;
        } top = temp_top; bot = temp_bot;
    } res.push_back(top); res.push_back(bot);
    return res;
}

string retreive_original(vector<first_to_last> ftl) {
    int letters = 0;
    int index = 0;
    stringstream ss;

    while (letters < ftl.size() - 1) {
        ss << ftl[index].last;
        letters++;
        index = ftl[index].ftl;
    } return ss.str();
}

int main(int argc, char** argv) {
    if (argc > 4 || argc < 2) {
        throw (invalid_argument("Too many or too few arguments, 2 required (3 with option --count-only)"));
    }

    bool checked[argc];
    bool count_only = false;

    for (int i = 1; i < argc; i++) {
        string temp(argv[i]);
        if (temp == "--count-only") {
            checked[i] = true;
            count_only = true;
        } else {
            checked[i] = false;
        }
    }

    int infile_pos = -1;
    int q_pos = -1;

    for (int i = 1; i < argc; i++) {
        if (!checked[i]) {
            if (infile_pos == -1) {
                infile_pos = i;
            } else if (q_pos == -1) {
                q_pos = i;
            }
        }
    }

    if (q_pos == -1 || infile_pos == -1) {
        throw (invalid_argument("Too many or too few arguments, RTFM"));
    }

    char* infile = argv[infile_pos];
    string q(argv[q_pos]);

    ifstream pInfile(infile);
    if (!pInfile.is_open()) {
        stringstream error;
        error << "File: \"" << infile << "\" does not exist";
        throw (invalid_argument(error.str()));
    }

    int c, n, p, f;
    vector<int> index;
    string sequence = read_file(pInfile, &c, &n, &p, &f, &index);

    if (c == 1) {
        sequence = decompress(sequence, n, p);
    }

    vector<string> mat = decipher_bwt(sequence);

    vector<first_to_last> ftl = build_ftl(mat);


    for (int i = 0; i < ftl.size(); i++) {
        ftl[ftl[i].ftl].first_occ = ftl[i].last_occ;
    }

    vector<int> top_bot = bw_search(ftl, q);
    if (count_only) {
        if (top_bot.size() == 0)
            cout << "0" << endl;
        else
            cout << top_bot[1] - top_bot[0] + 1 << endl;
        return 0;
    } if (top_bot.size() == 0) {
        return 0;
    }

    string original_sequence = retreive_original(ftl);
    reverse(original_sequence.begin(), original_sequence.end());

    for (int i = top_bot[0]; i <= top_bot[1]; i++) {
        int occ = 1;
        for (int j = original_sequence.length() - 1; j >= 0; j--) {
            if (original_sequence[j] == ftl[i].first) {
                if (occ == ftl[i].first_occ) {
                    cout << j << endl;
                }
                occ++;
            }
        }
    }
}
