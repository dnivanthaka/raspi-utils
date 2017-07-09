#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unistd.h>


#define STATS_PATH "/proc/stat"

using namespace std;

int readStats(vector<string> &statsList)
{
    string line;
    int count = 0;

    ifstream stats(STATS_PATH);
    if(stats.is_open()){
        while(getline(stats, line)){
            // We are ignoring the cpu line here
            if(count++ == 0){
                continue;
            }

            if(line.compare(0, 3, "cpu") == 0){
                statsList.push_back(line);
            }
        }

        stats.close();
    }else{
        cerr << "Cannot open stats file " << endl;
        return -1;
    }
    return int(statsList.size());
}

int main(int argc, char *argv[])
{
    string line;
    int count = 0;
    //int num_cpu = 0;
    vector<string> statsList;
    vector< vector<long> > prevVal;
    /*
    ifstream stats("/proc/stat");
    if(stats.is_open()){
        while(getline(stats, line)){
            // We are ignoring the cpu line here
            if(count++ == 0){
                continue;
            }

            if(line.compare(0, 3, "cpu") == 0){
                //cout << line << endl;
                statsList.push_back(line);
                //num_cpu++;
            }
        }

        stats.close();
        cout << "No: of CPUs = " << int(statsList.size()) << endl;
        for(vector<string>::iterator it = statsList.begin(); it != statsList.end(); ++it){
            vector<string> tokens;
            istringstream iss(*it);
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
            cout << *it << atoi(tokens.at(4).c_str()) << endl;
        }
    }*/
    readStats(statsList);
    cout << "No: of CPUs = " << int(statsList.size()) << endl;
    for(int i=0;i<statsList.size();i++){
        vector<long> tmp;
        tmp.push_back(0);
        tmp.push_back(0);
        prevVal.push_back(tmp);
    }
    while(true){
        readStats(statsList);
        int i = 0;
        for(vector<string>::iterator it = statsList.begin(); it != statsList.end(); ++it){
            vector<string> tokens;
            istringstream iss(*it);
            copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
            //cout << *it << atoi(tokens.at(4).c_str()) << endl;
            long idle, total, diff_idle, diff_total;
            idle = atoi(tokens.at(4).c_str());
            for(vector<string>::iterator it2 = tokens.begin(); it2 != tokens.end(); ++it2){
                //cout <<  "|" << *it2; 
                total = total + atoi((*it2).c_str());
            }

            //vector<long> tmp = prevVal.at(i);
            //diff_idle  = idle - tmp.at(0);
            //diff_total = total - tmp.at(1);
            diff_idle  = idle;
            diff_total = total;
            cout << "Usage " << (1000 * (diff_total - diff_idle) / diff_total + 5)/10 << endl;
            cout << endl;
            //tmp.at(0) = idle;
            //tmp.at(1) = total;
            //prevVal.at(i) = tmp;

            i++;
        }
            usleep(1000000);
    }

    return 0;
}
