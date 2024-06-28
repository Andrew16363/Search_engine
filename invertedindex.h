#include <vector>
#include <string>
#include <map>

#pragma once
using namespace std;

struct Entry
{
    size_t doc_id, count;
    bool operator==(const Entry &other) const;
};

class InvertedIndex
{
private:
    vector<string> docs;
    map<string, vector<Entry>> freq_dictionary;

public:
    InvertedIndex() = default;

    vector<string> UpdateDocumentBase(vector<string> input_docs);
    void Indexation(string str, size_t i);
    vector<Entry> GetWordCount(const string &word);
};
