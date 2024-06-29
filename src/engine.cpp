
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
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
// #include "gtest/gtest.h"

using namespace std;

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

  return 0;
}