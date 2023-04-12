#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "task2.h"
#include "task1.h"
#include "task3.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <mpi.h>
using namespace std;
using namespace std::chrono;


int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int world_rank, world_size, namelen;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_rank==0) printf("Num of processes: %d\n", world_size);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processor_name, &namelen);
    printf("Process %d from machine %s\n", world_rank, processor_name);

    /**
     * constant variables.
     * */
//    const string filename = "testFilesTask3.txt";
    const string filename = "tinyTwitter.json";
//    const string filename = "smallTwitter.json";
    const string place_file = "sal.json";
//    const string filename = "/data/projects/COMP90024/bigTwitter.json";
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
    long long file_size = in_file.tellg();
    long long proc_file_size = file_size/world_size;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    printf("size of file (bytes): %lld, reading time (microseconds): %d\n", file_size, duration.count());


    /**
     * Read the tweet files. Need to be done in parallel.
     * Get username and place name.
     * */
    in_file.seekg(world_rank*proc_file_size); // TODO: [parallel] Read from different location.
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
    long long row_num_total=0;
    long long pair_num = 0;
    while (getline (in_file, input_line)) {
        if (in_file.tellg()>proc_file_size*(world_rank+1)){
            break;
        }
        row_num_total++;
        size_t pos1, pos0;

        if (seek_user) { // find user
            pos1 = input_line.find(search_term_user);
            if (pos1 != string::npos){
                size_t comma_pos = input_line.rfind(",");
                username = input_line.substr(pos1+search_term_user_len+4, comma_pos-(pos1+search_term_user_len+4)-1);

                seek_user = false;
            }
        }

        if (!seek_user) { // find place
            pos0 = input_line.find(search_term_place);
            if (pos0 != string::npos){
                size_t comma_pos = input_line.rfind(",");
                place_full_name = input_line.substr(pos0+search_term_place_len+1, comma_pos-(pos0+search_term_place_len+2));
                transform(place_full_name.begin(), place_full_name.end(), place_full_name.begin(), ::tolower); // transform to lower case
                seek_user = true;
                pair_num++;
                /**
                 * We have the username and its corresponding place_full_name now.
                 * Please try not to modify too much codes.
                 * Finish task 1, 2, 3 under this if clause.
                 * */

                /** Task 1 */
                task1(tweet_counts, username);

                /** Task 2 */
                task2(gcc_count_map, place_map, place_full_name);

                /** Task 3 */
                task3(tweeters_location, username, place_full_name, place_map);
            }
        }
    }

    printf("Process id: %d, row_num_total: %lld, pair_num: %lld, line: %s\n", world_rank, row_num_total, pair_num, input_line.c_str());


    /** Task 1 result */
    // get top tweeters name from all procs. Form a map to store the name, values of all top 10 tweeters among all procs.
    vector<pair<string, int>> top_tweeters_vec = printTop10Tweeters(tweet_counts, false);
    char* local_top_names = new char[10*32];
    for (size_t i = 0; i < 10 && i < top_tweeters_vec.size(); i++) {
        strcpy(&local_top_names[i*32], top_tweeters_vec[i].first.c_str());
    }
    char* global_top_names = new char[10*32*world_size];
    MPI_Allgather(local_top_names, 10*32, MPI_CHAR, global_top_names, 10*32, MPI_CHAR, MPI_COMM_WORLD);
    map<string, int> top_tweeters_map;
    for (int i=0; i<10*world_size; i++){
        string author_id(&global_top_names[i*32]);
        if (tweet_counts.count(author_id)>0) {
            top_tweeters_map[author_id] = tweet_counts[author_id];
        }
        else{
            top_tweeters_map[author_id] = 0;
        }
    }
    int top_tweeters_size = top_tweeters_map.size();

    // set up array to store authors' tweet name and value
    char* top_tweeters_names_individual = new char[top_tweeters_size*32];
    int* top_tweeters_value_individual = new int[top_tweeters_size]();
    int count = 0;
    for (auto it = top_tweeters_map.begin(); it != top_tweeters_map.end(); ++it) {
        strcpy(&top_tweeters_names_individual[count*32], it->first.c_str());
        top_tweeters_value_individual[count] = it->second;
        count++;
    }

    // set up receiving buffers
    char* top_tweeters_names_total = NULL;
    int* top_tweeters_value_total = NULL;
    if (world_rank==0){
        top_tweeters_names_total = new char[world_size*top_tweeters_size*32]();
        top_tweeters_value_total = new int[world_size*top_tweeters_size]();
    }

    // calculate final answer for task1
    MPI_Gather(top_tweeters_names_individual, top_tweeters_size*32, MPI_CHAR, top_tweeters_names_total, top_tweeters_size*32, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gather(top_tweeters_value_individual, top_tweeters_size, MPI_INT, top_tweeters_value_total, top_tweeters_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank==0){
        map<string, int> tweet_counts_final;
        for (int i=0; i<world_size*top_tweeters_size; i++){
            string author_id(&top_tweeters_names_total[i*32]);
            if (tweet_counts_final.count(author_id)<=0){ // author does not exist yet
                tweet_counts_final[author_id] = top_tweeters_value_total[i];
            }
            else{ // author does already exist
                tweet_counts_final[author_id] += top_tweeters_value_total[i];
            }
        }
        printTop10Tweeters(tweet_counts_final, true);
    }

    /** Task 2 result */
    int* gcc_records_individual = new int[7]();
    int* gcc_records_total = NULL;
    gcc_records_individual[0] = gcc_count_map["1gsyd"];
    gcc_records_individual[1] = gcc_count_map["2gmel"];
    gcc_records_individual[2] = gcc_count_map["3gbri"];
    gcc_records_individual[3] = gcc_count_map["4gade"];
    gcc_records_individual[4] = gcc_count_map["5gper"];
    gcc_records_individual[5] = gcc_count_map["6ghob"];
    gcc_records_individual[6] = gcc_count_map["7gdar"];
    if (world_rank==0) gcc_records_total = new int[world_size*7]();
    MPI_Gather(gcc_records_individual, 7, MPI_INT, gcc_records_total, 7, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank==0){
        std::map<std::string, int>::iterator it = gcc_count_map.begin();
        for (int i=0; i<7; i++){
            int sum = 0;
            for (int j=0; j<world_size; j++){
                sum += gcc_records_total[i+j*7];
            }
            it->second = sum;
            ++it;
        }
        testTask2(gcc_count_map);
    }

    /** Task 3 */
    vector<pair<string, vector<int>>> top_places_tweeters_vec = printTopTweetersInCities(tweeters_location, false);

    // get top tweeters name from all place procs. Form a map to store the name, values of all top 10 tweeters in cities among all procs.
    char* local_top_places_names = new char[10*32];
    for (size_t i = 0; i < 10 && i < top_places_tweeters_vec.size(); i++) {
        strcpy(&local_top_places_names[i*32], top_places_tweeters_vec[i].first.c_str());
    }
    char* global_top_places_names = new char[10*32*world_size];
    MPI_Allgather(local_top_places_names, 10*32, MPI_CHAR, global_top_places_names, 10*32, MPI_CHAR, MPI_COMM_WORLD);
    map<string, vector<int>> top_place_tweeters_map;
    for (int i=0; i<10*world_size; i++){
        string author_id(&global_top_places_names[i*32]);
        vector<int> gcc_count_records = {0,0,0,0,0,0,0};
        if (tweeters_location.count(author_id)>0){
            for (int i=0; i<7; i++){
                gcc_count_records[i] = i;
                gcc_count_records[i] = tweeters_location[author_id][i+1];
            }
        }
        top_place_tweeters_map[author_id] = gcc_count_records;
    }
    int top_places_tweeters_size = top_place_tweeters_map.size();

    // set up array to store authors' tweet name and value
    char* top_places_tweeters_names_individual = new char[top_places_tweeters_size*32];
    int* top_places_tweeters_value_individual = new int[top_places_tweeters_size*7]();
    count = 0;
    for (auto it = top_place_tweeters_map.begin(); it != top_place_tweeters_map.end(); ++it) {
        strcpy(&top_places_tweeters_names_individual[count*32], it->first.c_str());
        for (int i=0; i<7; i++){
            top_places_tweeters_value_individual[count*7+i] = it->second[i];
        }
        count++;
    }

    // set up receiving buffers
    char* top_places_tweeters_names_total = NULL;
    int* top_places_tweeters_value_total = NULL;
    if (world_rank==0){
        top_places_tweeters_names_total = new char[world_size*top_places_tweeters_size*32]();
        top_places_tweeters_value_total = new int[world_size*top_places_tweeters_size*7]();
    }

    // calculate final answer for task3
    MPI_Gather(top_places_tweeters_names_individual, top_places_tweeters_size*32, MPI_CHAR, top_places_tweeters_names_total, top_places_tweeters_size*32, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gather(top_places_tweeters_value_individual, top_places_tweeters_size*7, MPI_INT, top_places_tweeters_value_total, top_places_tweeters_size*7, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank==0){
        map<string, int*> tweet_places_counts_final;
        for (int i=0; i<world_size*top_places_tweeters_size; i++){
            string author_id(&top_places_tweeters_names_total[i*32]);
            if (tweet_places_counts_final.count(author_id)<=0){ // author does not exist yet
                int* gcc_counts = new int[8]();
                for (int j=1; j<8; j++){
                    gcc_counts[j] = top_places_tweeters_value_total[i*7+(j-1)];
                    if (top_places_tweeters_value_total[i*7+(j-1)]>0) gcc_counts[0]++;
                }
                tweet_places_counts_final[author_id] = gcc_counts;
            }
            else{ // author does already exist
                for (int j=1; j<8; j++){
                    if (top_places_tweeters_value_total[i*7+(j-1)]>0 && tweet_places_counts_final[author_id][j]==0) {
                        tweet_places_counts_final[author_id][0]+=1;
                    };
                    tweet_places_counts_final[author_id][j] += top_places_tweeters_value_total[i*7+(j-1)];
                }
            }
        }
        printTopTweetersInCities(tweet_places_counts_final, true);
    }

    if (world_rank==0){
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        printf("All tasks execution time (microseconds): %d\n", duration.count());
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
