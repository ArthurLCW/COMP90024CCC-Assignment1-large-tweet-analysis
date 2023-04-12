#include <tuple>
#include <vector>

void task3(std::map<std::string, int *> &tweeters_location,
           const std::string& username,
           const std::string& place_full_name,
           const std::map<std::string, std::string>& place_map);
std::vector<std::pair<std::string, std::vector<int>>> printTopTweetersInCities(
        const std::map<std::string, int *> &tweeters_location,
        const bool print_flag);