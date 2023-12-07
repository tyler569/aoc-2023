#include "aoc.h"

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>

std::string sample_input = R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";

struct hand {
    std::string cards {};
    std::string original_cards {};
    int score {};
    char card_values[13] {};
    int part { 1 };

    hand() = default;

    explicit hand(std::string cards)
        : cards(std::move(cards))
        , original_cards(this->cards)
        , score(0)
    {
        init();
    }

    void init()
    {
        std::fill(std::begin(card_values), std::end(card_values), 0);
        for (char c : cards) {
            if (c == 'T')
                card_values[8]++;
            else if (c == 'J')
                card_values[9]++;
            else if (c == 'Q')
                card_values[10]++;
            else if (c == 'K')
                card_values[11]++;
            else if (c == 'A')
                card_values[12]++;
            else
                card_values[c - '2']++;
        }
    }

    [[nodiscard]] std::optional<int> has_n_of_a_kind(int n, int at_least = 0) const
    {
        for (int i = at_least; i < 13; i++) {
            if (card_values[i] == n)
                return i;
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::pair<int, int>> has_full_house() const
    {
        if (auto three = has_n_of_a_kind(3); three.has_value()) {
            if (auto two = has_n_of_a_kind(2); two.has_value()) {
                return std::make_pair(*three, *two);
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::pair<int, int>> has_two_pair() const
    {
        if (auto first = has_n_of_a_kind(2); first.has_value()) {
            if (auto second = has_n_of_a_kind(2, static_cast<int>(*first) + 1);
                second.has_value()) {
                return std::make_pair(*first, *second);
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] int high_card() const
    {
        for (int i = 12; i >= 0; i--) {
            if (card_values[i] > 0)
                return i;
        }
        return -1;
    }

    static std::strong_ordering cmp_card(char a, char b)
    {
        int v1 = a == 'A' ? 12
            : a == 'K'    ? 11
            : a == 'Q'    ? 10
            : a == 'J'    ? 9
            : a == 'T'    ? 8
                          : a - '2';
        int v2 = b == 'A' ? 12
            : b == 'K'    ? 11
            : b == 'Q'    ? 10
            : b == 'J'    ? 9
            : b == 'T'    ? 8
                          : b - '2';
        return v1 <=> v2;
    }

    static std::strong_ordering cmp_card_2(char a, char b)
    {
        int v1 = a == 'A' ? 12
            : a == 'K'    ? 11
            : a == 'Q'    ? 10
            : a == 'J'    ? -1
            : a == 'T'    ? 8
                          : a - '2';
        int v2 = b == 'A' ? 12
            : b == 'K'    ? 11
            : b == 'Q'    ? 10
            : b == 'J'    ? -1
            : b == 'T'    ? 8
                          : b - '2';
        return v1 <=> v2;
    }

    template <typename T>
    [[nodiscard]] std::strong_ordering cmp_hand(const hand &other, T getter) const
    {
        auto us = getter(*this), them = getter(other);
        if (us.has_value() && them.has_value()) {
            for (auto [a, b] : std::ranges::views::zip(original_cards, other.original_cards)) {
                if (cmp_card(a, b) != std::strong_ordering::equal) {
                    if (part == 1) {
                        return cmp_card(a, b);
                    } else {
                        return cmp_card_2(a, b);
                    }
                }
            }
        } else if (us.has_value()) {
            return std::strong_ordering::greater;
        } else if (them.has_value()) {
            return std::strong_ordering::less;
        }
        return std::strong_ordering::equal;
    }

    std::strong_ordering operator<=>(const hand &other) const
    {
        auto c1 = cmp_hand(other, [](const hand &h) { return h.has_n_of_a_kind(5); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        c1 = cmp_hand(other, [](const hand &h) { return h.has_n_of_a_kind(4); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        c1 = cmp_hand(other, [](const hand &h) { return h.has_full_house(); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        c1 = cmp_hand(other, [](const hand &h) { return h.has_n_of_a_kind(3); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        c1 = cmp_hand(other, [](const hand &h) { return h.has_two_pair(); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        c1 = cmp_hand(other, [](const hand &h) { return h.has_n_of_a_kind(2); });
        if (c1 != std::strong_ordering::equal)
            return c1;
        for (auto [a, b] : std::ranges::views::zip(cards, other.cards)) {
            if (cmp_card(a, b) != std::strong_ordering::equal)
                return cmp_card(a, b);
        }
        return std::strong_ordering::equal;
    }

    static char to_card_char(int i)
    {
        return i == 11 ? 'A' : i == 10 ? 'K' : i == 9 ? 'Q' : i == 8 ? 'T' : '2' + i;
    }

    [[nodiscard]] std::string best_joker_replacement() const
    {
        std::vector<hand> candidates;
        for (int i1 = 0; i1 < (cards[0] == 'J' ? 12 : 1); i1++) {
            char c1 = cards[0] == 'J' ? to_card_char(i1) : cards[0];
            for (int i2 = 0; i2 < (cards[1] == 'J' ? 12 : 1); i2++) {
                char c2 = cards[1] == 'J' ? to_card_char(i2) : cards[1];
                for (int i3 = 0; i3 < (cards[2] == 'J' ? 12 : 1); i3++) {
                    char c3 = cards[2] == 'J' ? to_card_char(i3) : cards[2];
                    for (int i4 = 0; i4 < (cards[3] == 'J' ? 12 : 1); i4++) {
                        char c4 = cards[3] == 'J' ? to_card_char(i4) : cards[3];
                        for (int i5 = 0; i5 < (cards[4] == 'J' ? 12 : 1); i5++) {
                            char c5 = cards[4] == 'J' ? to_card_char(i5) : cards[4];
                            candidates.emplace_back(std::string { c1, c2, c3, c4, c5 });
                        }
                    }
                }
            }
        }

        std::sort(candidates.begin(), candidates.end());
        return candidates.back().cards;
    }

    void print_best() const
    {
        if (has_n_of_a_kind(5)) {
            std::print("Five of a kind");
        } else if (has_n_of_a_kind(4)) {
            std::print("Four of a kind");
        } else if (has_full_house()) {
            std::print("Full house");
        } else if (has_n_of_a_kind(3)) {
            std::print("Three of a kind");
        } else if (has_two_pair()) {
            std::print("Two pair");
        } else if (has_n_of_a_kind(2)) {
            std::print("One Pair");
        } else {
            std::print("High card");
        }
    }
};

std::istream &operator>>(std::istream &is, hand &h) { return is >> h.cards >> h.score; }

int main()
{
    auto input = aoc::input(2023, 7);
    // auto input = sample_input;

    auto iss = std::istringstream(input);
    std::vector<hand> hands;
    for (hand h; iss >> h;) {
        h.original_cards = h.cards;
        h.init();
        hands.push_back(h);
        h = hand {};
    }

    std::sort(hands.begin(), hands.end());
    unsigned long total = 0;
    for (size_t i = 0; i < hands.size(); i++) {
        std::print("{}: {} ", i + 1, hands[i].cards);
        hands[i].print_best();
        std::print(" ({} bid -> {} score)\n", hands[i].score, hands[i].score * (i + 1));
        total += hands[i].score * (i + 1);
    }

    std::print("Part 1: {}\n", total);

    for (auto &hand : hands) {
        hand.cards = hand.best_joker_replacement();
        hand.part = 2;
        hand.init();
    }

    std::sort(hands.begin(), hands.end());
    total = 0;
    for (size_t i = 0; i < hands.size(); i++) {
        std::print("{}: {} {} ", i + 1, hands[i].cards, hands[i].original_cards);
        hands[i].print_best();
        std::print(" ({} bid -> {} score)\n", hands[i].score, hands[i].score * (i + 1));
        total += hands[i].score * (i + 1);
    }

    if (total >= 249645980) // 257074183)
        std::println("(too high)");
    if (total <= 249387818)
        std::println("(too low)");

    std::print("Part 2: {}\n", total);
}
