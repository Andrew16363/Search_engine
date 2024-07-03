#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <mutex>
#include "../include/invertedindex.h"

using namespace std;
mutex Index;

bool Entry::operator==(const Entry &other) const
{
    return (doc_id == other.doc_id && count == other.count);
}

vector<string> InvertedIndex::UpdateDocumentBase(vector<string> input_docs)
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
void InvertedIndex::Indexation(string str, size_t i)
{
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
    }
}

vector<Entry> InvertedIndex::GetWordCount(const string &word)
{
    vector<Entry> WordCount;
    if (freq_dictionary.contains(word))
    {
        WordCount = freq_dictionary.at(word);
    }

    return WordCount;
}
