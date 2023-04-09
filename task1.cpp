#include <map>
#include <iostream>
#include "task1.h"
#include <vector>
#include <algorithm>

using namespace std;

void task1(map<string, int> &tweet_counts, const string &username) {
    tweet_counts[username]++;
    //a map tweet_counts that stores the tweet count for each user
    //If the user doesn't exist in the map, it will be automatically created with an initial value of 0, 
    // and add to 1
}

void printTop10Tweeters(const map<string, int> &tweet_counts) {
    vector<pair<string, int>> top_tweeters;
    // takes a const reference to the tweet_counts map and prints the top 10 users
    for (const auto& p : tweet_counts) {
        top_tweeters.push_back(make_pair(p.first, p.second));
    }
    // tweet_counts map and inserts to the top_tweeters vector.
    sort(top_tweeters.begin(), top_tweeters.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    //sorts the top_tweeters vector in descending order 
    //uses lambda function.

    printf("Top 10 Tweeters:\n");
    for (size_t i = 0; i < 10 && i < top_tweeters.size(); i++) {
        string output = "#" + to_string(i + 1) + top_tweeters[i].first + " " + to_string(top_tweeters[i].second);
        printf("%s\n", output.c_str());
    }
    // top 10 of the sorted top_tweeters vector 
    // (or until the end if it has fewer than 10) 
    // prints
}
