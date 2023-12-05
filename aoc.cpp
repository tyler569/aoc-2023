#include "aoc.h"

#include <curl/curl.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace aoc {

size_t write_string(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    auto data = (std::string *)userdata;
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string download(int year, int day)
{
    std::string data;
    std::string url = std::format("https://adventofcode.com/{}/day/{}/input", year, day);

    std::cout << "Downloading " << url << std::endl;

    auto cookie = getenv("AOC_SESSION");
    if (!cookie) {
        std::cerr << "AOC_SESSION not set" << std::endl;
        exit(1);
    }

    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github.com/tyler569/aoc-2023 / aoc@choam.me");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string);
        auto res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }

    return data;
}

std::string cache_filename(int year, int day)
{
    std::string filename;
    auto home = getenv("HOME");
    if (!home) {
        std::cerr << "HOME not set" << std::endl;
        exit(1);
    }
    filename.append(home);
    filename.append("/.cache/aoc/");
    filename.append(std::to_string(year));
    filename.append("/");
    filename.append(std::to_string(day));
    return filename;
}

std::optional<std::string> get_from_cache(std::string_view filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string input(int year, int day)
{
    auto filename = cache_filename(year, day);
    auto content = get_from_cache(filename);
    if (content) {
        return *content;
    }

    std::cerr << "Cache miss, downloading " << year << " day " << day << std::endl;

    auto str = download(year, day);
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    auto file = std::ofstream(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open cache file " << filename << std::endl;
        exit(1);
    }
    file << str;

    return str;
}

std::vector<long> parse_numbers(std::istream &iss)
{
    std::vector<long> numbers;

    long tmp;
    while (iss >> tmp) {
        numbers.push_back(tmp);
    }

    iss.clear(std::ios_base::goodbit | (iss.eof() ? std::ios_base::eofbit : 0));

    return numbers;
}

std::vector<long> parse_numbers(const std::string &string)
{
    std::istringstream iss(string);
    return parse_numbers(iss);
}

}