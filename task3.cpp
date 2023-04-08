#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include "task3.h"
#include <algorithm>
#include <iostream>

void task3(std::map<std::string, std::set<std::string>>& tweeters_location,
           const std::string& username,
           const std::map<std::string, std::string>& place_map,
           const std::string& place_full_name) {
    auto it = place_map.find(place_full_name);
    if (it != place_map.end()) {
        std::string gcc = it->second;
        tweeters_location[username].insert(gcc);
        // place_map 结果正常
        // tweet location 输出结果：Tweeters Location: “3573538936: 1rnsw”
        // TODO: 查找pfn是否在pm中，若不在忽略，若在存储到tl[username]中
    }
}

bool compareTweeters(const std::tuple<std::string, int, int>& a,
                     const std::tuple<std::string, int, int>& b) {
    if (std::get<1>(a) != std::get<1>(b)) {
        return std::get<1>(a) > std::get<1>(b);
    }
    return std::get<2>(a) > std::get<2>(b);
}

void printTopTweetersInCities(const std::map<std::string, std::set<std::string>>& tweeters_location,
                              const std::map<std::string, int>& tweet_counts) {
    std::vector<std::tuple<std::string, int, int>> top_tweeters;
    for (const auto& entry : tweeters_location) {
        const std::string& author_id = entry.first;
        int city_count = entry.second.size();
        int total_tweets = tweet_counts.at(author_id);
        top_tweeters.emplace_back(author_id, city_count, total_tweets);
    }
    std::sort(top_tweeters.begin(), top_tweeters.end(), compareTweeters);

    std::cout << "Top 10 Tweeters in Most Greater Capital Cities:" << std::endl;
    for (size_t i = 0; i < 10 && i < top_tweeters.size(); ++i) {
        const auto& author_entry = top_tweeters[i];
        const std::string& author_id = std::get<0>(author_entry);
        int city_count = std::get<1>(author_entry);
        int total_tweets = std::get<2>(author_entry);

        std::cout << "#" << (i + 1) << " " << author_id << " " << city_count << " cities, " << total_tweets << " tweets" << std::endl;
        //此行cities应换成gcc
    }
}
