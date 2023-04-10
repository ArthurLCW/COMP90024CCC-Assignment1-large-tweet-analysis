#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include "task3.h"
#include <algorithm>
#include <iostream>
//using namespace std;

//void task3LXQ(std::map<std::string, std::set<std::string>>& tweeters_location,
//           const std::string& username,
//           const std::map<std::string, std::string>& place_map,
//           const std::string& place_full_name) {
//    auto it = place_map.find(place_full_name);
//    if (it != place_map.end()) {
//        std::string gcc = it->second;
//        tweeters_location[username].insert(gcc);
//        // place_map 结果正常
//        // tweet location 输出结果：Tweeters Location: “3573538936: 1rnsw”
//        // TODO: 查找pfn是否在pm中，若不在忽略，若在存储到tl[username]中
//    }
//}

void task3(std::map<std::string, int *> &tweeters_location,
              const std::string& username,
              const std::string& place_full_name,
              const std::map<std::string, std::string>& place_map){
    auto it = place_map.find(place_full_name);
    if (it != place_map.end()) { // place exists in place_map. TODO: further enhancement! Adjust to stupid format.
        std::string gcc = it->second;
        int gcc_idx; // used for identifying great capital city. 1 -> 1gsyd; 2->2gmel; etc.
        if (gcc=="1gsyd") gcc_idx=1;
        else if (gcc=="2gmel") gcc_idx=2;
        else if (gcc=="3gbri") gcc_idx=3;
        else if (gcc=="4gade") gcc_idx=4;
        else if (gcc=="5gper") gcc_idx=5;
        else if (gcc=="6ghob") gcc_idx=6;
        else if (gcc=="7gdar") gcc_idx=7;
        else return;

        if (tweeters_location.count(username)<=0){ // if the user is new
            int* gcc_records = new int[8]();
            // for (int i=0; i<8; i++) gcc_records[i]=0;
            gcc_records[gcc_idx] = 1;
            gcc_records[0] = 1; // index 0 used for recording number of cities that user has tweeted.
            tweeters_location[username] = gcc_records;
//            std::cout<<"first insert for user: "<<username<<", gcc: "<<gcc_idx<<", with city records: "<<tweeters_location[username][0]<<" "
//                <<tweeters_location[username][1]<<" "<<tweeters_location[username][2]<<" "<<tweeters_location[username][3]<<" "
//                <<tweeters_location[username][4]<<" "<<tweeters_location[username][5]<<" "<<tweeters_location[username][6]<<" "
//                <<tweeters_location[username][7]<<std::endl;
        }
        else{ // the user already exists
            if (tweeters_location[username][gcc_idx]>0){ // check if the detected city is new. Not new, +1 for city number
                tweeters_location[username][gcc_idx]++;
            }
            else{ // the city does not exist in user record
                tweeters_location[username][gcc_idx]++;
                tweeters_location[username][0]++;
            }
//            std::cout<<"again insert for user: "<<username<<" with city records: "<<tweeters_location[username][0]<<" "
//                <<tweeters_location[username][1]<<" "<<tweeters_location[username][2]<<" "<<tweeters_location[username][3]<<" "
//                <<tweeters_location[username][4]<<" "<<tweeters_location[username][5]<<" "<<tweeters_location[username][6]<<" "
//                <<tweeters_location[username][7]<<std::endl;
        }
    }
}

bool compareTweeters(const std::tuple<std::string, int, int>& a,
                     const std::tuple<std::string, int, int>& b) {
    if (std::get<1>(a) != std::get<1>(b)) {
        return std::get<1>(a) > std::get<1>(b);
    }
    return std::get<2>(a) > std::get<2>(b);
}

//void printTopTweetersInCitiesLXQ(const std::map<std::string, std::set<std::string>>& tweeters_location,
//                              const std::map<std::string, int>& tweet_counts) {
//    std::vector<std::tuple<std::string, int, int>> top_tweeters;
//    for (const auto& entry : tweeters_location) {
//        const std::string& author_id = entry.first;
//        int city_count = entry.second.size();
//        int total_tweets = tweet_counts.at(author_id);
//        top_tweeters.emplace_back(author_id, city_count, total_tweets);
//    }
//    std::sort(top_tweeters.begin(), top_tweeters.end(), compareTweeters);
//
//    std::cout << "Top 10 Tweeters in Most Greater Capital Cities:" << std::endl;
//    for (size_t i = 0; i < 10 && i < top_tweeters.size(); ++i) {
//        const auto& author_entry = top_tweeters[i];
//        const std::string& author_id = std::get<0>(author_entry);
//        int city_count = std::get<1>(author_entry);
//        int total_tweets = std::get<2>(author_entry);
//
//        std::cout << "#" << (i + 1) << " " << author_id << " " << city_count << " cities, " << total_tweets << " tweets" << std::endl;
//        //此行cities应换成gcc
//    }
//}


std::vector<std::pair<std::string, std::vector<int>>> printTopTweetersInCities(const std::map<std::string, int *> &tweeters_location, const bool print_flag) {
    std::vector<std::tuple<std::string, int, int>> top_tweeters;
    for (const auto& entry : tweeters_location) {
        const std::string& author_id = entry.first;
        int city_count = entry.second[0];
        int total_tweets = entry.second[1]+entry.second[2]+entry.second[3]+entry.second[4]+entry.second[5]+entry.second[6]+entry.second[7];
        top_tweeters.emplace_back(author_id, city_count, total_tweets);
    }
    std::sort(top_tweeters.begin(), top_tweeters.end(), compareTweeters);

    std::vector<std::pair<std::string, std::vector<int>>> top_tweeters_output;
    for (size_t i = 0; i < 10 && i < top_tweeters.size(); ++i){
        // printf("start: %d\n", i);
        const auto& author_entry = top_tweeters[i];
        const std::string& author_id = std::get<0>(author_entry);
        std::map<std::string, int*>::const_iterator it = tweeters_location.find(author_id);
        std::vector<int> gcc_records = {0,0,0,0,0,0,0};
        for (int i=0; i<7; i++){
            gcc_records[i] = it->second[i+1];
        }
        top_tweeters_output.emplace_back(author_id, gcc_records);
        // printf("finish: %d\n", i);
    }

    if (print_flag){
        printf("Top 10 Tweeters in Most Greater Capital Cities:\n");
        for (size_t i = 0; i < 10 && i < top_tweeters.size(); ++i) {
            const auto& author_entry = top_tweeters[i];
            const std::string& author_id = std::get<0>(author_entry);
            int city_count = std::get<1>(author_entry);
            int total_tweets = std::get<2>(author_entry);

            std::string output = "#"+std::to_string(i+1)+" "+author_id+" "+std::to_string(city_count)+" (#"+std::to_string(total_tweets)
                                 + " tweets - ";
            bool first_tweeted_city = true;
            std::map<std::string, int*>::const_iterator it = tweeters_location.find(author_id);
            for (int i=1; i<8; i++){
                if (it->second[i]>0){
                    if (first_tweeted_city){
                        first_tweeted_city = false;
                        output += "#";
                    }
                    else{
                        output += ", #";
                    }
                    output += std::to_string(it->second[i]);
                    switch (i){
                        case 1: output += "gsyd"; break;
                        case 2: output += "gmel"; break;
                        case 3: output += "gbri"; break;
                        case 4: output += "gade"; break;
                        case 5: output += "gper"; break;
                        case 6: output += "ghob"; break;
                        case 7: output += "gdar"; break;
                    }
                }
            }
            output += ")";
            printf("%s\n", output.c_str());
        }
    }
    return top_tweeters_output;
}
