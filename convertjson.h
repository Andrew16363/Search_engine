#include <vector>
#include <string>

#pragma once
using namespace std;
struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator==(RelativeIndex &other) const;
};
class ConverterJSON
{
private:
    int LimitResponses;
    vector<string> Documents;
    vector<string> Requests;

public:
    ConverterJSON() = default;
    vector<string> GetTextDocuments(const string &file);
    int GetResponsesLimit(const string &file, const auto &field_name);
    vector<string> GetRequests(const string &file_request);
    void putAnswers(vector<vector<RelativeIndex>> Answers);
};