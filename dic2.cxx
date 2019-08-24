//
// Created by ykang7 on 2019-07-14.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "fst/fstlib.h"
#include <unordered_set>

static std::ofstream isym, osym;

void add_symbol(int i, char c) {
    static std::unordered_set<char> set;
    if (set.find(c) == set.end()) {
        isym << c << " " << static_cast<int>(i) << std::endl;
        osym << c << " " << static_cast<int>(i) << std::endl;
        set.insert(c);
    }
}

bool add_to_dictionary(const std::string &word, fst::StdVectorFst &dictionary) {
    if (word.empty()) return false;

    fst::StdVectorFst::StateId src = dictionary.Start();
    fst::StdVectorFst::StateId dst;
    fst::SortedMatcher<fst::StdVectorFst> matcher(dictionary, fst::MATCH_INPUT);
    matcher.SetState(src);
    bool found = true;

    for (auto c : word) {
        int i = (int) c - 'A' + 1;
        if (found && matcher.Find(i)) {
            src = matcher.Value().nextstate;
            matcher.SetState(src);
        } else {
            found = false;
            dst = dictionary.AddState();
            dictionary.AddArc(src, fst::StdArc(i, i, 0, dst));
            add_symbol(i, c);
            src = dst;
        }
    }

    dictionary.SetFinal(dst, fst::StdArc::Weight::One());
    dictionary.NumStates();

    return true;
}

int main(int argc, const char **argv) {
    std::ios::sync_with_stdio(false);
    std::ifstream ifs;
    if (argc >= 2) {
        ifs = std::ifstream{argv[1]};
        std::cin.rdbuf(ifs.rdbuf());
    }

    fst::StdVectorFst dictionary;
    fst::StdVectorFst::StateId start = dictionary.AddState();
    dictionary.SetStart(start);

    isym = std::ofstream("isym.txt");
    osym = std::ofstream("osym.txt");

    std::unordered_set<std::string> vocab;
    for (std::string word; std::cin >> word;) {
        const auto it = vocab.find(word);
        if (it == vocab.end()) {
            vocab.insert(word);
        }
    }
    std::vector<std::string> words(vocab.begin(), vocab.end());
    std::sort(words.begin(), words.end());
    for (const auto &word : words)             
        add_to_dictionary(word, dictionary);

    dictionary.Write("raw.fst");

    fst::RmEpsilon(&dictionary);
    dictionary.Write("rmeps.fst");

    fst::Determinize(dictionary, &dictionary);
    dictionary.Write("det.fst");

    fst::Minimize(&dictionary);
    dictionary.Write("min.fst");

    return 0;
}