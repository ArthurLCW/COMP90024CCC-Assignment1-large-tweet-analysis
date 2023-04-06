#include <map>
#include <iostream>
using namespace std;
int countMapElements(map<string, string> place_map){
    return place_map.size();
}

/** Need further extension and enhancement! */
string getGCC(map<string, string> place_map, map<string, int> gcc_count_map, string place_name){
    // cout<<"getGCC0 place name: "<<place_name<<endl;
    if (place_map.count(place_name)>0){ // the place name exists directly inside the place name map.
        // cout<<"getGCC1 place name: "<<place_name<<endl;
        string gcc_name = place_map[place_name];
        if (gcc_count_map.count(gcc_name)>0) return place_map[place_name];
    }
    return "";
}

void task2(map<string, int> &gcc_count_map, map<string, string> place_map, string place_name){
    // firstly, get real name, delete comma
    size_t pos_comma = place_name.find(",");
    if (pos_comma!=string::npos){ // there is such a comma
        place_name = place_name.substr(0, pos_comma);
    }

    string gcc_name = getGCC(place_map, gcc_count_map, place_name);
    if (gcc_name.size()>0){
        // cout<<"task2 gcc name0: "<<gcc_name<<", count: "<<gcc_count_map[gcc_name]<<endl;
        gcc_count_map[gcc_name] += 1;
        // cout<<"task2 gcc name1: "<<gcc_name<<", count: "<<gcc_count_map[gcc_name]<<endl;
    }
}

void testTask2(map<string, int> gcc_count_map){
    std::map<std::string, int>::iterator it = gcc_count_map.begin();
    // Iterate through the map and print the elements
    while (it != gcc_count_map.end()){
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        ++it;
    }
}