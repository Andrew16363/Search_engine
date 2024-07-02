
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iomanip>
#include "../include/convertjson.h"
#include "../include/invertedindex.h"
#include "../include/searchserver.h"
#include "gtest/gtest.h"

using namespace std;

// TEST(sample_test_case, sample_test)
// {
//   EXPECT_EQ(1, 1);
// }
// void TestInvertedIndexFunctionality(
//     const vector<string> &docs,
//     const vector<string> &requests,
//     const std::vector<vector<Entry>> &expected)
// {
//   std::vector<std::vector<Entry>> result;
//   InvertedIndex idx;
//   idx.UpdateDocumentBase(docs);
//   for (auto &request : requests)
//   {
//     std::vector<Entry> word_count = idx.GetWordCount(request);
//     result.push_back(word_count);
//   }
//   ASSERT_EQ(result, expected);
// }
// TEST(TestCaseInvertedIndex, TestBasic)
// {
//   const vector<string> docs = {
//       "london is the capital of great britain",
//       "big ben is the nickname for the Great bell of the striking clock"};
//   const vector<string> requests = {"london", "the"};
//   const vector<vector<Entry>> expected = {
//       {{0, 1}}, {{0, 1}, {1, 3}}};
//   TestInvertedIndexFunctionality(docs, requests, expected);
// }

// TEST(TestCaseInvertedIndex, TestBasic2)
// {
//   const vector<string> docs = {
//       "milk milk milk milk water water water",
//       "milk water water",
//       "milk milk milk milk milk water water water water water",
//       "americano cappuccino"};
//   const vector<string> requests = {"milk", "water", "cappuchino"};
//   const vector<vector<Entry>> expected = {
//       {{0, 4}, {1, 1}, {2, 5}}, {{0, 2}, {1, 2}, {2, 5}}, {{3, 1}}};
//   TestInvertedIndexFunctionality(docs, requests, expected);
// }

// TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord)
// {
//   const vector<string> docs = {
//       "a b c d e f g h i j k l",
//       "statement"};
//   const vector<string> requests = {"m", "statement"};
//   const vector<vector<Entry>> expected = {
//       {}, {}, {1, 1}};
//   TestInvertedIndexFunctionality(docs, requests, expected);
// }

// TEST(TestCaseSearchServer, TestSimple)
// {
//   const vector<string> docs = {
//       "milk milk milk milk water water water",
//       "milk water water",
//       "milk milk milk milk milk water water water water water",
//       "americano cappuccino"};
//   const vector<string> request = {"milk water", "sugar"};
//   const std::vector<vector<RelativeIndex>> expected = {
//       {{2, 1},
//        {0, 0.7},
//        {1, 0.3}},
//       {}};
//   InvertedIndex idx;
//   idx.UpdateDocumentBase(docs);
//   SearchServer srv(idx);
//   std::vector<vector<RelativeIndex>> result = srv.search(request);
//   ASSERT_EQ(result, expected);
// }

// TEST(TestCaseSearchServer, TestTop5)
// {
//   const vector<string> docs = {
//       "london is the capital of great britain",
//       "paris is the capital of france",
//       "berlin is the capital of germany",
//       "rome is the capital of italy",
//       "madrid is the capital of spain",
//       "lisboa is the capital of portugal",
//       "bern is the capital of switzerland",
//       "moscow is the capital of russia",
//       "kiev is the capital of ukraine",
//       "minsk is the capital of belarus",
//       "astana is the capital of kazakhstan",
//       "beijing is the capital of china",
//       "tokyo is the capital of japan",
//       "bangkok is the capital of thailand",
//       "welcome to moscow the capital of russia the third rome",
//       "amsterdam is the capital of netherlands",
//       "helsinki is the capital of finland",
//       "oslo is the capital of norway",
//       "stockholm is the capital of sweden",
//       "riga is the capital of latvia",
//       "tallinn is the capital of estonia",
//       "warsaw is the capital of poland",
//   };
//   const vector<string> request = {"moscow is the capital of russia"};
//   const std::vector<vector<RelativeIndex>> expected =
//       {
//           {{7, 1},
//            {14, 1},
//            {0, 0.666666687},
//            {1, 0.666666687},
//            {2, 0.666666687}}};
//   InvertedIndex idx;
//   idx.UpdateDocumentBase(docs);
//   SearchServer srv(idx);
//   std::vector<vector<RelativeIndex>> result = srv.search(request);
//   ASSERT_EQ(result, expected);
// }

void checkFileExists(const string &filename, const auto &field_name)
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
  }
}

int main()
{
  const auto &field_name = "config";
  const string answers_file = "answers.json";
  vector<string> text_files;
  vector<string> text_docs;
  vector<string> words;
  vector<thread> threads_files;
  vector<vector<RelativeIndex>> rel_index;
  ConverterJSON Convert;
  cout << "Starting MyEngine..." << endl;
  checkFileExists("../../config.json", field_name);
  cout << " config.json is OK! " << endl;

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

  text_docs = Convert.GetTextDocuments("../../config.json");
  words = Convert.GetRequests("../../requests.json");

  InvertedIndex index_docs;
  text_files = index_docs.UpdateDocumentBase(text_docs);

  for (int i = 0; i < text_files.size(); i++)
  {

    threads_files.emplace_back(bind(&InvertedIndex::Indexation, &index_docs, text_files[i], i));
  }

  for (auto &thread : threads_files)
  {

    thread.join();
  }
  SearchServer Server(index_docs);
  rel_index = Server.search(words);
  Convert.putAnswers(rel_index);

  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
  return 0;
}