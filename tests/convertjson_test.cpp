#include "../include/convertjson.h"
#include "../include/invertedindex.h"
#include "../include/searchserver.h"
#include <vector>
#include "gtest/gtest.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>

TEST(ConverterJSON, getResponsesLimit)
{
    ConverterJSON Convert1;
    const string &file = "../config.json";
    const auto &field_name = "config";
    int Limit_expected = 5;
    int Limit_rel = Convert1.GetResponsesLimit(file, field_name);
    ASSERT_EQ(Limit_expected, Limit_rel);
}

TEST(ConverterJSON, getFiles)
{
    const string &file = "../config.json";
    vector<string> files;
    ConverterJSON Convert2;
    vector<string> files_expected = {"resources/file001", "resources/file002", "resources/file003", "resources/file004"};
    files = Convert2.GetTextDocuments(file);
    ASSERT_EQ(files_expected, files);
}
TEST(ConverterJSON, getRequests)
{
    ConverterJSON Convert;
    vector<string> expected = {"milk", "water", "cappuchino"};
    vector<string> word_request;
    const string &file_request = "../requests.json";
    word_request = Convert.GetRequests(file_request);
    ASSERT_EQ(expected, word_request);
}

TEST(ConverterJSON, readConfig)
{
    const auto &field_name = "config";
    ConverterJSON Convert;

    ASSERT_NO_THROW(Convert.checkFileExists("../config.json", field_name));
}
TEST(ConverterJSON, readRequests)
{
    namespace fs = filesystem;
    const string request = "../requests.json";
    ASSERT_TRUE(fs::exists(request)) << "Файл не найден: " << request;
}
TEST(ConverterJSON, putAnswers)
{
    ConverterJSON Convert_expected;
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"};
    const vector<string> request = {"milk water", "cappuchino"};
    const string answers_file = "answers.json";
    vector<vector<RelativeIndex>> expected = {
        {{2, 1},
         {0, 0.7},
         {1, 0.3}},
        {}};
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (int i = 0; i < docs.size(); ++i)
    {
        idx.Indexation(docs[i], i);
    }
    SearchServer srv(idx);
    vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(expected, result);

    if (!filesystem::exists(answers_file))
    {
        ofstream outputFile(answers_file);
        outputFile.close();
    }
    else
    {
        ofstream outFile(answers_file, ios::trunc);
        outFile.close();
    }
    Convert_expected.putAnswers(result);
    // const vector<string> docs = {
    //     "milk milk milk milk water water water",
    //     "milk water water",
    //     "milk milk milk milk milk water water water water water",
    //     "americano cappuccino"};
    // const vector<string> request = {"milk", "water", "sugar"};
    // // vector<string> word_request;
    // // word_request = Convert_expected.GetRequests(request);
    // vector<vector<RelativeIndex>> expected = {
    //     {{2, 1},
    //      {0, 0.7},
    //      {1, 0.3}},
    //     {}};
    // InvertedIndex idx;
    // idx.UpdateDocumentBase(docs);
    // for (int i = 0; i < docs.size(); ++i)
    // {
    //     idx.Indexation(docs[i], i);
    // }
    // SearchServer srv(idx);
    // vector<vector<RelativeIndex>> result = srv.search(request);
    // Convert_expected.putAnswers(result);
    // // ofstream recordring_answers("answers.json");
    // // nlohmann::ordered_json answers_relevance;
    // // Convert_expected.putAnswers(result);
    // // recordring_answers << answers_relevance;
    // // recordring_answers.close();
    // // ifstream expected_answers("answers.json");
    // // // ASSERT_EQ(expected, result);
    // // nlohmann::ordered_json read_json;
    // // expected_answers >> read_json;
    // // expected_answers.close();

    // // ASSERT_EQ(read_json, answers_relevance.dump(4)) << "Содержимое файла answers.json не соответствует ожидаемому.";
    // // Открытие и чтение файла
    // ofstream file("answers.json");
    // ASSERT_TRUE(file.is_open()) << "Файл answers.json не был открыт.";

    // // nlohmann::ordered_json file_contents;
    // // file >> file_contents;

    // // Проверка, что содержимое файла соответствует ожидаемому JSON
    // ASSERT_EQ(expected, result) << "Содержимое файла answers.json не соответствует ожидаемому.";
}
