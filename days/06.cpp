#include "aoc.h"

#include <print>
#include <ranges>
#include <sstream>

std::string sample_input = R"(Time:      7  15   30
Distance:  9  40  200)";

long ways_to_win(long time, long distance)
{
    long ways_to_win = 0;
    std::print("Time: {} Distance {} ", time, distance);
    for (int button = 0; button < time; button++) {
        auto possible_distance = button * (time - button);
        if (possible_distance > distance)
            ways_to_win++;
    }
    std::println("Ways to win: {}", ways_to_win);
    return ways_to_win;
}

int main()
{
    auto input = aoc::input(2023, 6);
    // auto input = sample_input;

    std::string token;
    auto iss = std::istringstream(input);

    iss >> token;
    auto times = aoc::parse_numbers(iss);
    iss >> token;
    auto distances = aoc::parse_numbers(iss);

    long total_ways_to_win = 1;
    for (auto [time, distance] : std::ranges::views::zip(times, distances)) {
        total_ways_to_win *= ways_to_win(time, distance);
    }

    long real_time = 0;
    long real_distance = 0;

    bool in_distance = false;
    for (auto c : input) {
        if (isdigit(c) && in_distance) {
            real_distance *= 10;
            real_distance += c - '0';
        } else if (isdigit(c)) {
            real_time *= 10;
            real_time += c - '0';
        } else if (c == '\n') {
            in_distance = true;
        }
    }

    std::println("Part 1: {}", total_ways_to_win);
    std::println("Part 2: {}", ways_to_win(real_time, real_distance));
}