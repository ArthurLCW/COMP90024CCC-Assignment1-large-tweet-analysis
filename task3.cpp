#include <map>
#include <set>
#include <iostream>
#include "task3.h"
using namespace std;


void task3(map<string, set<string>> &tweeters_location, const string &username, map<string, string> &place_map, const string place_name) {
    // place map after task 2
    if (place_map.count(place_name) > 0) {
        string gcc_name = place_map[place_name];
        tweeters_location[username].insert(gcc_name);
    }
}
    

void printTop10CityTweeters(map<string, set<string>> &tweeters_location, const map<string, int> &tweet_counts) {
    vector<pair<string, pair<int, int>>> top_city_tweeters;
    for (const auto& p : tweeters_location) {
        int num_cities = p.second.size();
        int num_tweets = tweet_counts.at(p.first);
        top_city_tweeters.push_back(make_pair(p.first, make_pair(num_cities, num_tweets)));
    }

    sort(top_city_tweeters.begin(), top_city_tweeters.end(),
        [](const auto& a, const auto& b) {
            if (a.second.first == b.second.first) {
                return a.second.second > b.second.second;
            }
            return a.second.first > b.second.first;
        });

    cout << "Top 10 Tweeters in Most Greater Capital Cities:" << endl;
    for (size_t i = 0; i < 10 && i < top_city_tweeters.size(); i++) {
        cout << "#" << i + 1 << " " << top_city_tweeters[i].first << " "
             << top_city_tweeters[i].second.first << " cities, "
             << top_city_tweeters[i].second.second << " tweets" << endl;
    }
}
