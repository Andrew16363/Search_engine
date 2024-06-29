#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "convertjson.h"
#include "invertedindex.h"
#include <nlohmann/json.hpp>

using namespace std;

bool RelativeIndex::operator==(RelativeIndex &other) const
{
    return (doc_id == other.doc_id && rank == other.rank);
}

vector<string> ConverterJSON::GetTextDocuments(const string &file)
{
    ifstream stream_config(file);
    nlohmann::json Docs;
    stream_config >> Docs;
    stream_config.close();
    for (const auto &doc : Docs["files"])
    {
        Documents.push_back(doc);
    }
    return Documents;
}

int ConverterJSON::GetResponsesLimit(const string &file, const auto &field_name)
{
    ifstream stream_config(file);
    nlohmann::json Count_Answers;
    stream_config >> Count_Answers;
    stream_config.close();
    LimitResponses = Count_Answers["config"]["max_responses"];
    return LimitResponses;
}

vector<string> ConverterJSON::GetRequests(const string &file_request)
{
    ifstream stream_request(file_request);
    nlohmann::json Request;
    stream_request >> Request;
    stream_request.close();
    for (const auto &item : Request["requests"])
    {
        Requests.emplace_back(item);
    }

    return Requests;
}
void ConverterJSON::putAnswers(vector<vector<RelativeIndex>> Answers)

{
    ofstream recordring_answers("../answers.json");
    nlohmann::ordered_json answers_relevance;
    string doc = "docid";
    answers_relevance["answers"];
    string request = "request";
    int n = 0;
    int m = 0;
    for (size_t i = 0; i < Answers.size(); ++i)
    {
        if (Answers.size() > LimitResponses)
        {
            Answers[i].erase(Answers[i].begin() + LimitResponses, Answers[i].end());
        }
    }

    for (int i = 0; i < Answers.size(); ++i)
    {
        request += to_string(i + 1);

        if (Answers[i].size() == 0)
        {
            answers_relevance["answers"][request]["result"] = false;
        }
        else if (Answers[i].size() > 1)
        {
            answers_relevance["answers"][request]["result"] = true;

            for (auto &it : Answers[i])
            {

                answers_relevance["answers"][request]["relevance"]["docid" + to_string(n++)] = it.doc_id;
                answers_relevance["answers"][request]["relevance"]["rank" + to_string(m++)] = it.rank;
            }
        }
        else
        {
            answers_relevance["answers"][request]["result"] = true;
            for (auto &it : Answers[i])
            {
                answers_relevance["answers"][request]["docid"] = it.doc_id;
                answers_relevance["answers"][request]["rank"] = it.rank;
            }
        }
        request = "request";
        n = 0;
        m = 0;
    }
    recordring_answers << answers_relevance;
    recordring_answers.close();
}
