#ifndef AOC_2023_RANGE_H
#define AOC_2023_RANGE_H

#include <algorithm>
#include <ostream>

namespace aoc {

class range {
    long begin, end;

public:
    range(long begin, long end)
        : begin(begin)
        , end(end)
    {
    }

    [[nodiscard]] bool intersects(const range &other) const
    {
        return (begin < other.end) && (end > other.begin);
    }

    [[nodiscard]] range overlap(const range &other) const
    {
        return { std::max(begin, other.begin), std::min(end, other.end) };
    }

    [[nodiscard]] range merged(const range &other) const
    {
        return { std::min(begin, other.begin), std::max(end, other.end) };
    }

    [[nodiscard]] bool contains(long value) const { return (value >= begin) && (value < end); }

    [[nodiscard]] long low() const { return begin; }
    [[nodiscard]] long high() const { return end; }

    template <class F> void distinct_overlaps(const range &other, F f) const
    {
        if (*this == other) {
            f(*this);
        } else if (begin == other.begin) {
            f(overlap(other));
            f(range { overlap(other).end, end });
        } else if (end == other.end) {
            f(range { begin, overlap(other).begin });
            f(overlap(other));
        } else if (intersects(other)) {
            f(range { std::min(begin, other.begin), overlap(other).begin });
            f(overlap(other));
            f(range { overlap(other).end, std::max(end, other.end) });
        }
    }

    bool operator==(const range &other) const
    {
        return (begin == other.begin) && (end == other.end);
    }

    range &operator+=(long offset)
    {
        begin += offset;
        end += offset;
        return *this;
    }

    range operator+(long offset) const { return { begin + offset, end + offset }; }

    bool operator<(const range &other) const
    {
        return (begin < other.begin) || ((begin == other.begin) && (end < other.end));
    }

    friend std::ostream &operator<<(std::ostream &os, const range &range)
    {
        return os << "[" << range.begin << ", " << range.end << "]";
    }
};

}

#endif // AOC_2023_RANGE_H