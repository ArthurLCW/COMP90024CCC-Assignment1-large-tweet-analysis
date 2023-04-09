#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <bits/stdc++.h>
#include "task2.h"
#include "task1.h"
#include "task3.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
using namespace std;
using namespace std::chrono;


int main(){
    /**
     * constant variables.
     * */
//    const string filename = "testFilesTask3.txt";
    const string filename = "tinyTwitter.json";
//    const string filename = "smallTwitter.json";
    const int proc_num = 1;
    const string place_file = "sal.json";
//    const string place_file = "salTest.txt";

    /**
     * variables using in task 1, 2, 3.
     * Including maps, etc...
     * */
    /** Task 1 */
    map<string, int> tweet_counts; // Task 1 data structure

    /** Task 2 */
    map<string, int> gcc_count_map;
    gcc_count_map["1gsyd"]=0;
    gcc_count_map["2gmel"]=0;
    gcc_count_map["3gbri"]=0;
    gcc_count_map["4gade"]=0;
    gcc_count_map["5gper"]=0;
    gcc_count_map["6ghob"]=0;
    gcc_count_map["7gdar"]=0;
    
    /** Task 3 */
//    map<string, set<string>> tweeters_location_lxq; // Task 3 data structure
    map<string, int *> tweeters_location;

    /**
     * Read from sal.json to obtain a map of location-gcd.
     * May need to be written in parallel.
     * */
    map<string, string> place_map;
    string input_line_place, place_name, gcc_name;
    ifstream in_place_file(place_file, ios::binary);
    auto start = high_resolution_clock::now();
    while (getline (in_place_file, input_line_place)) {
        // find place name first
        size_t pos0 = input_line_place.find(": {");
        if (pos0 != string::npos) {
            size_t quotation0 = input_line_place.find("\"");
            size_t quotation1 = input_line_place.rfind("\"");
            place_name = input_line_place.substr(quotation0+1, quotation1-quotation0-1);
        }

        // find gcc name then
        size_t pos1 = input_line_place.find("gcc");
        if (pos1 != string::npos) {
            size_t quotation1 = input_line_place.rfind("\"");
            gcc_name = input_line_place.substr(pos1+7, 5);
            // cout<<place_name<<"->"<<gcc_name<<endl;
            place_map[place_name] = gcc_name;
        }
    }

    // cout<<"Place map size: "<<place_map.size()<<endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    printf("place file reading time (microseconds): %d\n", duration.count());


    /**
     * Get the tweet file size.
     * Sequential codes since it is very fast. Timing codes can be referenced and reused.
     * */
    start = high_resolution_clock::now();
    ifstream in_file(filename, ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    printf("size of file (bytes): %d, reading time (microseconds): %d\n", file_size, duration.count());

    /**
     * Read the tweet files. Need to be done in parallel.
     * Get username and place name.
     * */
    in_file.seekg(0); // TODO: [parallel] Read from different location.
    string input_line;
    string search_term_place = "\"full_name\": ";
    string search_term_user = "author_id";
    int search_term_place_len = search_term_place.length();
    int search_term_user_len = search_term_user.length();
    string place_full_name, username;

    // this boolean flag is used for dealing with incomplete json.
    // i.e., those only have place but without username.
    start = high_resolution_clock::now();
    bool seek_user = true;
    while (getline (in_file, input_line)) {
        // cout << input_line <<endl;
        size_t pos1, pos0;

        if (seek_user) { // find user
            pos1 = input_line.find(search_term_user);
            if (pos1 != string::npos){
                size_t comma_pos = input_line.rfind(",");
                username = input_line.substr(pos1+search_term_user_len+4, comma_pos-(pos1+search_term_user_len+4)-1);
                
                seek_user = false;
                // cout<<username<<endl;
            }
        }

        if (!seek_user) { // find place
            pos0 = input_line.find(search_term_place);
            if (pos0 != string::npos){
                size_t comma_pos = input_line.rfind(",");
                place_full_name = input_line.substr(pos0+search_term_place_len+1, comma_pos-(pos0+search_term_place_len+2));
                transform(place_full_name.begin(), place_full_name.end(), place_full_name.begin(), ::tolower); // transform to lower case
                seek_user = true;
                /**
                 * We have the username and its corresponding place_full_name now.
                 * Please try not to modify too much codes.
                 * Finish task 1, 2, 3 under this if clause.
                 * */
                // cout<<place_full_name<<endl;

                /** Task 1 */
                task1(tweet_counts, username);

                /** Task 2 */
                task2(gcc_count_map, place_map, place_full_name);

                /** Task 3 */
                // task3LXQ(tweeters_location_lxq, username, place_map, place_full_name);
                task3(tweeters_location, username, place_full_name, place_map);
            }
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    printf("All tasks execution time (microseconds): %d\n", duration.count());

    /** Task 1 result */
    printTop10Tweeters(tweet_counts);

    /** Task 2 result */
    testTask2(gcc_count_map);

    /** Task 3 */
    printTopTweetersInCities(tweeters_location);

    /** Task 3 result lxq */
//    printf("Tweeters Location:\n");
//    for (const auto& entry : tweeters_location_lxq) {
//        const std::string& author_id = entry.first;
//        printf("%s:", author_id.c_str());
//        for (const auto& location : entry.second) {
//            printf(" %s", location.c_str());
//        }
//        printf("\n");
//    }
//    printf("-----------------\n");
//    printTopTweetersInCitiesLXQ(tweeters_location_lxq, tweet_counts);

//    printf("Place Map:\n");
//    for (auto it = place_map.begin(); it != place_map.end(); it++) {
//        printf("%s -> %s\n", it->first.c_str(), it->second.c_str());
//    }
    
}
