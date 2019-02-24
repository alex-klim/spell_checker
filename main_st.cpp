#include <iostream>
#include <fstream>
#include <array>
#include <thread>
#include <sstream>

#include "project/spell_checker.h"
#include "utils.h"


static constexpr ContainerType type = ContainerType::Unordered_Set;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Too few arguments. You must provide a dictionary and at least one text to check\n";
        return -1;
    }
    std::string dict(argv[1]);
    SpellChecker spell_checker(type);

    std::cout << log("program started");
    spell_checker.load(argv[1]);
    std::cout << log("dictionary " + std::string(argv[1]) + " is loaded to memory");

    // ======================== checking files =====================================
    for (int i = 2; i < argc; i++) {
        unsigned missed, valid, total;
        missed = 0;
        valid = 0;
        total = 0;
        std::ifstream infile;
        infile.open(argv[i]);
        std::string line;
        if (infile.good()) {
            std::cout << log("spell checking for file " + std::string(argv[i]) + " is started");
            while (infile >> line) {
                total++;
                if (!SpellChecker::is_valid(line)) {
                    continue;
                }
                valid++;
                if (!spell_checker.check(line)) {
                    missed++;
                }
            }
            std::cout << log("spell checking for " + std::string(argv[i])
                    +" is completed: misspeled words: " + std::to_string(missed)
                    + ", valid words: " + std::to_string(valid)
                    + ", total words: " + std::to_string(total)
                    );
        }
    }
    return 0;
}
