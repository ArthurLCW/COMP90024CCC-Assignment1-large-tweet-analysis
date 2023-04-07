#include <map>
#include <set>
using namespace std;

void task3(map<string, set<string>> &tweeters_location, const string &username, const string &gcc_name);
void printTop10CityTweeters(map<string, set<string>> &tweeters_location, const map<string, int> &tweet_counts);