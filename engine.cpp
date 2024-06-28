
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
#include "convertjson.h"
// #include "gtest/gtest.h"

using namespace std;
mutex Index;

struct Entry
{
  size_t doc_id, count;
  bool operator==(const Entry &other) const
  {
    return (doc_id == other.doc_id && count == other.count);
  }
};

// struct RelativeIndex
// {
//   size_t doc_id;
//   float rank;
//   bool operator==(RelativeIndex &other) const
//   {
//     return (doc_id == other.doc_id && rank == other.rank);
//   }
// };
struct compare
{
  bool operator()(const RelativeIndex &relative_index1, const RelativeIndex &relative_index2)
  {
    if (relative_index1.rank == relative_index2.rank)
    {
      return relative_index1.doc_id < relative_index2.doc_id;
    }
    else
    {
      return relative_index1.rank > relative_index2.rank;
    }
  }
};
/*
class ConverterJSON
{
private:
  int LimitResponses;
  vector<string> Documents;
  vector<string> Requests;

public:
  ConverterJSON() = default;

  vector<string> GetTextDocuments(const string &file)
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

  int GetResponsesLimit(const string &file, const auto &field_name)
  {
    ifstream stream_config(file);
    nlohmann::json Count_Answers;
    stream_config >> Count_Answers;
    stream_config.close();
    LimitResponses = Count_Answers["config"]["max_responses"];
    return LimitResponses;
  }

  vector<string> GetRequests(const string &file_request)
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
  void putAnswers(vector<vector<RelativeIndex>> Answers)

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
};
*/
class InvertedIndex
{
private:
  vector<string> docs;                        // список содержимого документов
  map<string, vector<Entry>> freq_dictionary; // частотный словарь

public:
  InvertedIndex() = default;
  /**
  * Обновить или заполнить базу документов, по которой будем совершать
  поиск
  * @param texts_input содержимое документов
  */
  // void UpdateDocumentBase(vector<string> input_docs)
  vector<string> UpdateDocumentBase(vector<string> input_docs)
  {
    for (int i = 0; i < input_docs.size(); ++i)
    {
      ifstream texts_input;
      texts_input.open(input_docs[i]);
      if (texts_input)
      {
        string text;
        string line;
        while (getline(texts_input, line))
        {
          text += line;
        }
        docs.push_back(text);
        texts_input.close();
      }
      else
      {
        cout << "File " << input_docs[i] << " doesn't exist" << endl;
      }
    }
    return docs;
  }
  void Indexation(string str, size_t i)
  {

    lock_guard<mutex> lock(Index);
    istringstream ss(str);
    string word;

    while (ss >> word)
    {
      if (freq_dictionary.contains(word))
      {

        for (auto a : freq_dictionary[word])
        {

          if ((freq_dictionary[word].at(freq_dictionary[word].size() - 1)).doc_id == i)
          {

            (freq_dictionary[word].at(freq_dictionary[word].size() - 1)).count += 1;
            break;
          }
          else
          {
            freq_dictionary[word].push_back({i, 1});
            break;
          }
        }
      }
      else
      {
        freq_dictionary[word].push_back({i, 1});
      }
    }

    // for(auto it:freq_dictionary)
    // {
    // cout<<"index "<<"["<<it.first<<"] = ";
    // for(auto a: it.second)
    //{
    // cout<<"{"<<a.doc_id<<","<<a.count<<"}"<<endl;
    // }
    //}
  }
  // end function UpdateDocumentBase

  vector<Entry> GetWordCount(const string &word)
  {
    vector<Entry> WordCount;
    if (freq_dictionary.contains(word))
    {
      WordCount = freq_dictionary.at(word);
    }
    return WordCount;
  }
};

class SearchServer
{

private:
  InvertedIndex _index;

public:
  /**
  * @param idx в конструктор класса передаётся ссылка на класс
  InvertedIndex,
  *
  чтобы SearchServer мог узнать частоту слов встречаемых в
  запросе
  */

  SearchServer(InvertedIndex &idx) : _index(idx){};
  /**
  * Метод обработки поисковых запросов
  * @param queries_input поисковые запросы взятые из файла
  requests.json
  * @return возвращает отсортированный список релевантных ответов для
  заданных запросов
  */
  vector<vector<RelativeIndex>> search(const vector<string> &queries_input)
  {
    vector<vector<RelativeIndex>> RelevanceIndex;
    multimap<size_t, string> words_requests;
    vector<Entry> max_count;
    size_t maxElement = 0;
    int flag = 0;
    vector<size_t> relevance;
    vector<RelativeIndex> Relatives;

    for (int n = 0; n < queries_input.size(); ++n)
    {
      istringstream word(queries_input[n]);
      string separate_words;
      while (word >> separate_words)
      {
        max_count = _index.GetWordCount(separate_words);
        for (auto it : max_count)
        {
          if (it.count > maxElement)
          {
            maxElement = it.count;
          }
        }
        if (maxElement != 0)
        {
          words_requests.insert(make_pair(maxElement, separate_words));
        }

        maxElement = 0;
      }
      // конец цикла перебора слов из каждого запроса
      max_count.clear();
      if (words_requests.size() != 0)
      {
        for (auto it = words_requests.begin(); it != words_requests.end(); ++it)
        {
          max_count = _index.GetWordCount(it->second);

          for (auto iterator : max_count)
          {
            if (Relatives.size() == 0)
            {
              Relatives.push_back({RelativeIndex{iterator.doc_id, static_cast<float>(iterator.count)}});
            }
            else
            {
              flag = 0;
              for (int i = 0; i < Relatives.size(); ++i)
              {

                if (iterator.doc_id == Relatives[i].doc_id)
                {
                  replace(begin(Relatives), end(Relatives),
                          (RelativeIndex{Relatives[i].doc_id, static_cast<float>(Relatives[i].rank)}), (RelativeIndex{Relatives[i].doc_id, static_cast<float>(Relatives[i].rank + iterator.count)}));
                  flag = 1;
                  break;
                }
              }
              if (flag == 0)
              {
                Relatives.push_back({RelativeIndex{iterator.doc_id, static_cast<float>(iterator.count)}});
              }
            }
          }
        }
        for (int i = 0; i < Relatives.size(); ++i)
        {

          if (Relatives[i].rank > maxElement)
          {
            maxElement = Relatives[i].rank;
          }
        }

        for (int i = 0; i < Relatives.size(); ++i)
        {

          replace(begin(Relatives), end(Relatives),
                  (RelativeIndex{Relatives[i].doc_id, static_cast<float>(Relatives[i].rank)}), (RelativeIndex{Relatives[i].doc_id, static_cast<float>(Relatives[i].rank / maxElement)}));
        }
        sort(Relatives.begin(), Relatives.end(), compare());
      }
      RelevanceIndex.push_back(Relatives);
      maxElement = 0;
      Relatives.clear();
      words_requests.clear();
      max_count.clear();
    }

    return RelevanceIndex;
  };
};

void checkFileExists(const string &filename, const auto &field_name)
{
  if (!filesystem::exists(filename))
  {
    throw runtime_error("config file is missing");
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
  const string answers_file = "../answers.json";
  vector<string> text_files;
  vector<string> text_docs;
  vector<string> words;
  vector<thread> threads_files;
  vector<vector<RelativeIndex>> rel_index;
  ConverterJSON Convert;
  cout << "Starting MyEngine..." << endl;
  checkFileExists("../config.json", field_name);
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

  text_docs = Convert.GetTextDocuments("../config.json");

  Convert.GetResponsesLimit("../config.json", field_name);
  words = Convert.GetRequests("../requests.json");

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