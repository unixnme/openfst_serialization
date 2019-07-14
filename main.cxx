#include <iostream>
#include <fst/fstlib.h>
#include <fstream>

int foo() {
	// A vector FST is a general mutable FST 
	fst::StdVectorFst fst;

	// Adds state 0 to the initially empty FST and make it the start state. 
	fst.AddState();   // 1st state will be state 0 (returned by AddState) 
	fst.SetStart(0);  // arg is state ID

	// Adds two arcs exiting state 0.
	// Arc constructor args: ilabel, olabel, weight, dest state ID. 
	fst.AddArc(0, fst::StdArc(1, 1, 0.5, 1));  // 1st arg is src state ID
	fst.AddArc(0, fst::StdArc(2, 2, 1.5, 1));

	// Adds state 1 and its arc. 
	fst.AddState();
	fst.AddArc(1, fst::StdArc(3, 3, 2.5, 2));

	// Adds state 2 and set its final weight. 
	fst.AddState();
	fst.SetFinal(2, 3.5);  // 1st arg is state ID, 2nd arg weight
	std::stringbuf buffer;
	std::ostream os{&buffer};

	fst.Write("binary.fst");
    fst.Write(os, fst::FstWriteOptions{});
    std::cout << buffer.str().size() << std::endl;

    std::istream is{&buffer};
    auto new_fst = fst::StdVectorFst::Read(is, fst::FstReadOptions{});
    new_fst->Write("test.fst");

    return 0;
}

int main(int argc, const char **argv) {
    foo();
	return 0;
}
