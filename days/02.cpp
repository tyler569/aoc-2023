#include "aoc.h"

#include <iostream>
#include <print>
#include <sstream>
#include <string>

std::string sample_input = R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";

struct game {
    int id;
    int min_blue;
    int min_green;
    int min_red;
};

game parse_game(std::string game)
{
    std::istringstream stream(game);
    std::string discard;
    int game_number;
    stream >> discard >> game_number >> discard; // "Game (n):"

    int min_blue = 0, min_green = 0, min_red = 0;
    while (!stream.eof()) {
        int n;
        std::string color;
        stream >> n >> color;
        if (color.starts_with("blue")) {
            min_blue = std::max(min_blue, n);
        } else if (color.starts_with("green")) {
            min_green = std::max(min_green, n);
        } else if (color.starts_with("red")) {
            min_red = std::max(min_red, n);
        }
    }

    return { game_number, min_blue, min_green, min_red };
}

bool is_game_possible_1(game g)
{
    return g.min_blue <= 14 && g.min_green <= 13 && g.min_red <= 12;
}

long min_cubes_2(game g)
{
    return g.min_blue * g.min_green * g.min_red;
}

int main()
{
    auto s = aoc::input(2023, 2);
    std::string line;
    std::istringstream stream(s);

    int total = 0;
    long total_2 = 0;
    while (std::getline(stream, line)) {
        auto g = parse_game(line);
        if (is_game_possible_1(g)) {
            total += g.id;
        }
        total_2 += min_cubes_2(g);
    }

    std::println("Part 1: {}", total);
    std::println("Part 2: {}", total_2);

    return 0;
}