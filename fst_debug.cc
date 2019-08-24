#include <iostream>
#include "fst/fstlib.h"
#include <vector>
#include <fstream>
static std::ofstream isym, osym;

void add_symbol(char c) {
    static std::unordered_set<char> set;
    if (set.find(c) == set.end()) {
        isym << c << " " << static_cast<int>(c) << std::endl;
        osym << c << " " << static_cast<int>(c) << std::endl;
        set.insert(c);
    }
}

int main(int argc, const char **argv) {

    isym = std::ofstream("isym.txt");
    osym = std::ofstream("osym.txt");

    fst::StdVectorFst fst;
    const auto start = fst.AddState();

//    std::vector<std::vector<int>> words = {{1,2,3}, {4,5,6}, {4,7}};
    std::vector<std::vector<int>> words = {{8,1,5}, {16,4,3}, {16,2}};
//    std::vector<std::vector<int>> words = {{8,1,5}, {6,4,3}, {6,2}}
//    std::vector<std::string> words = {"WAS", "ONE", "OF"};


    for (const auto &word : words) {
        auto it = word.begin();
        fst::StdVectorFst::StateId src = start;
        fst::Matcher<fst::StdVectorFst> matcher(fst, fst::MATCH_INPUT);
        matcher.SetState(start);
        while (it != word.end() && matcher.Find(*it)) {
            src = matcher.Value().nextstate;
            ++it;
            matcher.SetState(src);
        }
        while (it != word.end()) {
            const auto dst = fst.AddState();
            fst.AddArc(src, fst::StdArc(*it, *it, 0, dst));
            add_symbol(*it);
            src = dst;
            ++it;
        }
        fst.SetFinal(src, fst::StdArc::Weight::One());
        std::cout << "num states = " << fst.NumStates() << std::endl;
    }

    fst.Write("raw.fst");

    fst::RmEpsilon(&fst);
    fst.Write("rmeps.fst");

    fst::Determinize(fst, &fst);
    fst.Write("det.fst");

    fst::Minimize(&fst);
    fst.Write("min.fst");

    return 0;
}