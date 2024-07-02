#pragma once
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;

struct compare
{
    bool operator()(const RelativeIndex &relative_index1, const RelativeIndex &relative_index2);
};

class SearchServer
{

private:
    InvertedIndex _index;

public:
    SearchServer(InvertedIndex &idx) : _index(idx){};

    vector<vector<RelativeIndex>> search(const vector<string> &queries_input);
};