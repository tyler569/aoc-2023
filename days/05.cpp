#include "aoc.h"

#include <iostream>
#include <print>
#include <sstream>
#include <vector>

std::string sample_input = R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)";

long min_location(const std::string &input)
{
    auto iss = std::istringstream(input);
    std::string line;
    std::string token;

    iss >> token;
    auto seeds = aoc::parse_numbers(iss);

    unsigned long seeds_seen = 0;

    while (!iss.eof()) {
        iss >> token;
        // std::println("{}:", token);
        iss >> token;

        long before, after, len;
        while (iss >> after >> before >> len) {
            // std::println("{} {} {}", before, after, len);

            for (size_t i = 0; i < seeds.size(); i++) {
                if (seeds_seen & (1 << i))
                    continue;
                auto seed = seeds[i];
                if (seed >= before && seed < before + len) {
                    // std::println("seed {} is in range {}-{} and is remapped to {}", seed, before,
                    //     before + len, seed + after - before);
                    seed += after - before;
                    seeds_seen |= (1 << i);
                    seeds[i] = seed;
                }
            }

            // std::print("    seeds: ");
            // for (auto seed : seeds)
            //     std::print("{} ", seed);
            // std::print("\n");
        }
        seeds_seen = 0;

        iss.clear(std::ios_base::goodbit | (iss.eof() ? std::ios_base::eofbit : 0));
    }

    return *std::min_element(seeds.begin(), seeds.end());
}

int main()
{
    auto input = aoc::input(2023, 5);
    // auto input = sample_input;

    std::println("Part 1: {}", min_location(input));

    return 0;
}