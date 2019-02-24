#include <iostream>
#include <fstream>
#include <array>
#include <thread>
#include <sstream>
#include <mutex>
#include <vector>

#include "project/spell_checker.h"
#include "utils.h"


std::mutex m;
static constexpr ContainerType type = ContainerType::Unordered_Set;

void thread_task(char* file, SpellChecker& spell_checker) {
    unsigned missed, valid, total;
    std::ifstream infile;
    infile.open(file);
    std::string line;
    if (infile.good()) {
        std::cout << log("spell checking for file " + std::string(file) + " is started");
        while (infile >> line) {
            total++;
            if (!SpellChecker::is_valid(line)) {
                continue;
            }
            valid++;
            m.lock();
            if (!spell_checker.check(line)) {
                missed++;
            }
            m.unlock();
        }
        std::cout << log("spell checking for " + std::string(file)
                +" is completed: misspeled words: " + std::to_string(missed)
                + ", valid words: " + std::to_string(valid)
                + ", total words: " + std::to_string(total)
                );
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Too few arguments. You must provide a dictionary and at least one text to check\n";
        return -1;
    }

    SpellChecker spell_checker(type);

    std::cout << log("program started");
    spell_checker.load(argv[1]);
    std::cout << log("dictionary " + std::string(argv[1]) + " is loaded to memory");

    // ======================== checking files =====================================
    std::vector<std::thread> threads;
    for (auto i = 2; i < argc; i++) {
        threads.push_back(std::thread(thread_task, argv[i], std::ref(spell_checker)));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
