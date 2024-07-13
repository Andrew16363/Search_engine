#include "../include/convertjson.h"
#include "../include/invertedindex.h"
#include "../include/searchserver.h"
#include <vector>
#include "gtest/gtest.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>
#include <chrono>

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
    // ConverterJSON Convert_expected;
    // const vector<string> docs = {
    //     "milk milk milk milk water water water",
    //     "milk water water",
    //     "milk milk milk milk milk water water water water water",
    //     "americano cappuccino"};
    // const vector<string> request = {"milk water", "sugar"};
    // const string answers_file = "answers.json";
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
    // // ASSERT_EQ(expected, result);

    // if (!filesystem::exists(answers_file))
    // {
    //     ofstream outputFile(answers_file);
    //     outputFile.close();
    // }
    // else
    // {
    //     ofstream outFile(answers_file, ios::trunc);
    //     outFile.close();
    // }
    // // ASSERT_TRUE(Convert_expected.putAnswers(result)) << "OK.";

    // std::ifstream file(answers_file);
    // nlohmann::json read_json;
    // file >> read_json;
    // file.close();

    // // Проверка, что файл не пустой
    // ASSERT_FALSE(read_json.empty());

    // // Проверка соответствия записанных данных ожидаемым
    // for (size_t i = 0; i < expected.size(); ++i)
    // {
    //     string request_key = "request" + std::to_string(i + 1);
    //     if (expected[i].empty())
    //     {
    //         // Проверка, что результат помечен как false, если ответов нет
    //         ASSERT_EQ(read_json["answers"][request_key]["result"], false);
    //     }
    //     else
    //     {
    //         // Проверка, что результат помечен как true, если ответы есть
    //         ASSERT_EQ(read_json["answers"][request_key]["result"], true);
    //         for (size_t j = 0; j < expected[i].size(); ++j)
    //         {
    //             string docid_key = "docid" + std::to_string(j);
    //             string rank_key = "rank" + std::to_string(j);
    //             // Проверка соответствия docid и rank
    //             ASSERT_EQ(read_json["answers"][request_key]["relevance"][docid_key], expected[i][j].doc_id);
    //             ASSERT_EQ(read_json["answers"][request_key]["relevance"][rank_key], expected[i][j].rank);
    //         }
    //     }
    //     // const vector<string> docs = {
    //     //     "milk milk milk milk water water water",
    //     //     "milk water water",
    //     //     "milk milk milk milk milk water water water water water",
    //     //     "americano cappuccino"};
    //     // const vector<string> request = {"milk", "water", "sugar"};
    //     // // vector<string> word_request;
    //     // // word_request = Convert_expected.GetRequests(request);
    //     // vector<vector<RelativeIndex>> expected = {
    //     //     {{2, 1},
    //     //      {0, 0.7},
    //     //      {1, 0.3}},
    //     //     {}};
    //     // InvertedIndex idx;
    //     // idx.UpdateDocumentBase(docs);
    //     // for (int i = 0; i < docs.size(); ++i)
    //     // {
    //     //     idx.Indexation(docs[i], i);
    //     // }
    //     // SearchServer srv(idx);
    //     // vector<vector<RelativeIndex>> result = srv.search(request);
    //     // Convert_expected.putAnswers(result);
    //     // ofstream recordring_answers("answers.json");
    //     //  // nlohmann::ordered_json answers_relevance;
    //     //  // Convert_expected.putAnswers(result);
    //     //  // recordring_answers << answers_relevance;
    //     //  // recordring_answers.close();
    //     //  // ifstream expected_answers("answers.json");
    //     //  // // ASSERT_EQ(expected, result);
    //     // nlohmann::ordered_json read_json;
    //     //  recordring_answers >> read_json;
    //     //  // expected_answers.close();

    //     // ASSERT_EQ(read_json, answers_relevance.dump(4)) << "Содержимое файла answers.json не соответствует ожидаемому.";
    //     // // Открытие и чтение файла
    //     // ofstream file("answers.json");
    //     // ASSERT_TRUE(file.is_open()) << "Файл answers.json не был открыт.";

    //     // // nlohmann::ordered_json file_contents;
    //     // // file >> file_contents;

    //     // // Проверка, что содержимое файла соответствует ожидаемому JSON
    //     // ASSERT_EQ(expected, result) << "Содержимое файла answers.json не соответствует ожидаемому.";
    // }
    ConverterJSON Convert_expected;
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"};
    const std::vector<std::string> request = {"milk water", "sugar", "cap"};
    const std::string answers_file = "answers.json";
    std::vector<std::vector<RelativeIndex>> expected = {
        {{2, 1},
         {0, 0.7},
         {1, 0.3}},
        {}};
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (size_t i = 0; i < docs.size(); ++i)
    {

        idx.Indexation(docs[i], i);
        this_thread::sleep_for(std::chrono::seconds(2));
    }
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(expected, result);
    Convert_expected.putAnswers(result);

    std::ifstream file(answers_file);
    nlohmann::json read_json;
    file >> read_json;
    file.close();

    ASSERT_FALSE(read_json.empty());

    for (size_t i = 0; i < expected.size(); ++i)
    {
        std::string request_key = "request" + std::to_string(i + 1);
        if (expected[i].empty())
        {

            ASSERT_EQ(read_json["answers"][request_key]["result"], false);
        }
        else
        {

            ASSERT_EQ(read_json["answers"][request_key]["result"], true);
            for (size_t j = 0; j < expected[i].size(); ++j)
            {
                std::string docid_key = "docid" + std::to_string(j);
                std::string rank_key = "rank" + std::to_string(j);

                ASSERT_EQ(read_json["answers"][request_key]["relevance"][docid_key], expected[i][j].doc_id);
                ASSERT_EQ(read_json["answers"][request_key]["relevance"][rank_key], expected[i][j].rank);
            }
        }
    }
}
