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

    ConverterJSON Convert_expected;
    const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"};
    const std::vector<std::string> request = {"m", "statement"};
    const std::string answers_file = "answers.json";
    std::vector<std::vector<RelativeIndex>> expected = {
        {}, {{1, 1}}};
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (size_t i = 0; i < docs.size(); ++i)
    {

        idx.Indexation(docs[i], i);
        this_thread::sleep_for(std::chrono::seconds(3));
    }
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(expected, result);

    Convert_expected.putAnswers(result);

    ifstream file(answers_file);
    nlohmann::json read_json;
    file >> read_json;
    file.close();
    ifstream file_expected("expected.json");
    nlohmann::json read_json_expected;
    file_expected >> read_json_expected;
    file_expected.close();

    ASSERT_EQ(read_json, read_json_expected);
}
