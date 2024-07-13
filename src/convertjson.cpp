#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "../include/convertjson.h"
#include "../include/invertedindex.h"
#include <nlohmann/json.hpp>

using namespace std;

bool RelativeIndex::operator==(const RelativeIndex &other) const
{
    return (doc_id == other.doc_id && rank == other.rank);
}
void ConverterJSON::checkFileExists(const string &filename, const string &field_name)
{
    if (!filesystem::exists(filename))
    {
        throw runtime_error("config file is missed");
    }
    else
    {
        ifstream filecheck(filename);
        nlohmann::json file;
        filecheck >> file;
        filecheck.close();
        const nlohmann::json &field_value = file[field_name];
        if (field_value.empty())
        {
            throw "config file is empty!!";
        }
        else
        {
            cout << " config.json is OK! " << endl;
        }
    }
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
    ConverterJSON::GetResponsesLimit("../config.json", "config");
    ofstream recording_answers("answers.json");
    if (!recording_answers.is_open())
    {
        cout << "Файл не найден!" << endl;
        return;
    }

    nlohmann::ordered_json answers_relevance;
    answers_relevance["answers"] = nlohmann::ordered_json::object();

    for (size_t i = 0; i < Answers.size(); ++i)
    {
        string request = "request" + to_string(i + 1);

        if (Answers[i].empty())
        {
            answers_relevance["answers"][request]["result"] = false;
            continue;
        }

        answers_relevance["answers"][request]["result"] = true;
        if (Answers[i].size() > LimitResponses)
        {
            Answers[i].erase(Answers[i].begin() + LimitResponses, Answers[i].end());
        }

        if (Answers[i].size() > 1)
        {
            for (size_t j = 0; j < Answers[i].size(); ++j)
            {
                answers_relevance["answers"][request]["relevance"]["docid" + to_string(j)] = Answers[i][j].doc_id;
                answers_relevance["answers"][request]["relevance"]["rank" + to_string(j)] = Answers[i][j].rank;
            }
        }
        else
        {
            answers_relevance["answers"][request]["docid"] = Answers[i][0].doc_id;
            answers_relevance["answers"][request]["rank"] = Answers[i][0].rank;
        }
    }

    recording_answers << answers_relevance.dump(4);
    recording_answers.close();
}
