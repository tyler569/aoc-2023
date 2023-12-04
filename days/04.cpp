#include "aoc.h"

#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::string sample_input = R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";

std::pair<int, std::vector<int>> parse(const std::string &input)
{
    std::istringstream iss(input);
    std::vector<int> card_winners;

    int total_score = 0;
    std::string line;
    while (std::getline(iss, line)) {
        auto [card, cards] = aoc::split(line, ":");
        auto [winning, haves] = aoc::split(cards, "|");

        std::istringstream iss_card { std::string(card) };
        std::istringstream iss_winning { std::string(winning) };
        std::istringstream iss_have { std::string(haves) };

        std::vector<int> winning_cards;
        std::vector<int> have_cards;

        std::string stmp;
        int card_number;
        iss_card >> stmp >> card_number;

        int tmp;
        while (iss_winning >> tmp)
            winning_cards.push_back(tmp);
        while (iss_have >> tmp)
            have_cards.push_back(tmp);

        int score = 0;
        int count = 0;
        for (auto &winner : winning_cards)
            for (auto &have : have_cards)
                if (winner == have) {
                    count++;
                    if (score == 0)
                        score = 1;
                    else
                        score *= 2;
                }

        card_winners.push_back(count);
        total_score += score;
    }
    return { total_score, card_winners };
}

std::unordered_map<int, long> child_card_count;

long count_cards_for_card(std::vector<int> card_winners, int card)
{
    if (child_card_count.contains(card))
        return child_card_count[card];

    long count = 1;
    auto winners = card_winners[card];

    for (auto i = 0; i < winners; i++)
        count += count_cards_for_card(card_winners, card + i + 1);
    child_card_count[card] = count;
    return count;
}

int main()
{
    auto input = aoc::input(2023, 4);
    // auto input = sample_input;

    auto [total_score, card_winners] = parse(input);
    std::println("Part 1: {}", total_score);

    // std::println("Part 2: {}", count_cards_for_card(card_winners, 0));

    auto total_cards = 0L;
    for (size_t i = 0; i < card_winners.size(); i++)
        total_cards += count_cards_for_card(card_winners, i);
    std::println("Part 2: {}", total_cards);
}
