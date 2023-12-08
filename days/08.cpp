#include "aoc.h"

#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <unordered_map>

std::string sample_input = R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";

std::string sample_input2 = R"(LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ))";

struct element {
    std::string name;
    std::string left;
    std::string right;
};

std::istream &operator>>(std::istream &is, element &e)
{
    std::string token;
    is >> e.name >> token >> std::ws;
    is.ignore(1);
    is >> e.left >> e.right;
    e.left.pop_back();
    e.right.pop_back();
    return is;
}

int main()
{
    auto input = aoc::input(2023, 8);
    // auto input = sample_input2;
    auto iss = std::istringstream(input);

    std::string lr_instructions;
    iss >> lr_instructions;

    std::vector<element> elements;
    for (element e; iss >> e;) {
        elements.push_back(e);
        e = element {};
    }

    std::unordered_map<std::string, std::pair<std::string, std::string>> map;
    for (auto &e : elements) {
        map[e.name] = { e.left, e.right };
    }

    size_t iterations;
    std::string current = std::string("AAA");
    for (iterations = 0;; iterations++) {
        auto c = lr_instructions[iterations % lr_instructions.size()];
        auto &[left, right] = map[current];
        if (c == 'L') {
            current = left;
        } else {
            current = right;
        }
        std::println("went {} to {}", c, current);
        if (current == "ZZZ") {
            break;
        }
    }
    std::print("Part 1: {}\n", iterations + 1);
}
