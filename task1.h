#include <map>
#include <vector>
using namespace std;

void task1(map<string, int> &tweet_counts, const string &username);
vector<pair<string, int>> printTop10Tweeters(const map<string, int> &tweet_counts, bool print_flag);
