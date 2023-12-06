#include "aoc.h"
#include "range.h"

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

struct mapping {
    aoc::range source;
    long offset;
};

std::ostream &operator<<(std::ostream &stream, const mapping &m)
{
    return stream << m.source << " -> " << m.offset;
}

struct step {
    std::string name;
    std::vector<mapping> mappings;
};

std::ostream &operator<<(std::ostream &stream, const step &s)
{
    stream << s.name << ":\n";
    for (const auto &mapping : s.mappings) {
        stream << mapping << '\n';
    }
    return stream;
}

struct almanac {
    std::vector<long> seeds;
    std::vector<aoc::range> seed_ranges;
    std::vector<step> steps;
};

std::ostream &operator<<(std::ostream &stream, const almanac &alm)
{
    stream << "seeds: ";
    for (const auto &seed : alm.seeds) {
        stream << seed << ' ';
    }
    stream << '\n';

    stream << "seed ranges: ";
    for (const auto &seed_range : alm.seed_ranges) {
        stream << seed_range << ' ';
    }
    stream << '\n';

    for (const auto &step : alm.steps) {
        stream << step << '\n';
    }
    return stream;
}

almanac parse(const std::string &input)
{
    auto iss = std::istringstream(input);
    auto alm = almanac {};
    std::string line;
    std::string token;

    iss >> token;
    alm.seeds = aoc::parse_numbers(iss);

    while (!iss.eof()) {
        auto stp = step {};
        iss >> token;
        stp.name = token;
        iss >> token;

        long before, after, len;
        while (iss >> after >> before >> len) {
            stp.mappings.push_back({ { before, before + len }, after - before });
        }

        iss.clear(std::ios_base::goodbit | (iss.eof() ? std::ios_base::eofbit : 0));
        alm.steps.push_back(stp);
    }

    for (size_t i = 0; i < alm.seeds.size(); i += 2) {
        alm.seed_ranges.emplace_back(alm.seeds[i], alm.seeds[i] + alm.seeds[i + 1]);
    }

    return alm;
}

long min_location_1(const almanac &alm)
{
    std::vector seeds = alm.seeds;
    std::vector<long> next_seeds(seeds.size(), -1);
    for (const auto &step : alm.steps) {
        for (const auto &mapping : step.mappings) {
            for (size_t i = 0; i < seeds.size(); i++) {
                if (mapping.source.contains(seeds[i])) {
                    next_seeds[i] = seeds[i] + mapping.offset;
                }
            }
        }

        for (size_t i = 0; i < seeds.size(); i++) {
            if (next_seeds[i] == -1) {
                next_seeds[i] = seeds[i];
            }
        }

        std::swap(seeds, next_seeds);
        std::fill(next_seeds.begin(), next_seeds.end(), -1);
    }

    return *std::min_element(seeds.begin(), seeds.end());
}

long min_location_2(const almanac &alm)
{
    std::vector seed_ranges = alm.seed_ranges;
    std::vector<char> seen_seed_ranges(seed_ranges.size(), false);
    std::vector<aoc::range> next_seed_ranges {};
    for (const auto &step : alm.steps) {
        for (const auto &mapping : step.mappings) {
            for (size_t i = 0; i < seed_ranges.size(); i++) {
                if (seen_seed_ranges[i]) {
                    continue;
                }
                auto seed_range = seed_ranges[i];
                seed_range.distinct_overlaps(mapping.source, [&](auto part) {
                    if (!seed_range.contains(part.low()))
                        return;
                    if (mapping.source.contains(part.low())) {
                        part += mapping.offset;
                        next_seed_ranges.push_back(part);
                        seen_seed_ranges[i] = true;
                    } else {
                        seed_ranges.push_back(part);
                        seen_seed_ranges.push_back(false);
                    }
                });
            }
        }

        for (size_t i = 0; i < seed_ranges.size(); i++) {
            if (!seen_seed_ranges[i]) {
                next_seed_ranges.push_back(seed_ranges[i]);
            }
        }

        seed_ranges = next_seed_ranges;
        seen_seed_ranges = std::vector<char>(seed_ranges.size(), false);
        next_seed_ranges = {};
    }

    return std::min_element(seed_ranges.begin(), seed_ranges.end())->low();
}

int main()
{
    auto input = aoc::input(2023, 5);
    // auto input = sample_input;

    auto alm = parse(input);

    std::println("Part 1: {}", min_location_1(alm));
    std::println("Part 2: {}", min_location_2(alm));

    return 0;
}