#include <tuple>
//void task3LXQ(std::map<std::string, std::set<std::string>>& tweeters_location,
//           const std::string& username,
//           const std::map<std::string, std::string>& place_map,
//           const std::string& place_full_name);

void task3(std::map<std::string, int *> &tweeters_location,
           const std::string& username,
           const std::string& place_full_name,
           const std::map<std::string, std::string>& place_map);
void printTopTweetersInCities(const std::map<std::string, int *> &tweeters_location);

//    void printTopTweetersInCitiesLXQ(
//    const std::map<std::string, std::set<std::string>>& tweeters_location,
//    const std::map<std::string, int>& tweet_counts);




