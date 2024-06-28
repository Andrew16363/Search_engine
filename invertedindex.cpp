#include <string>
#include <vector>
#include <fstream>
#include "invertedindex.h"

using namespace std;

struct Entry
{
    size_t doc_id, count;
    bool operator==(const Entry &other) const
    {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex
{
private:
    vector<string> docs;
    map<string, vector<Entry>> freq_dictionary;

public:
    InvertedIndex() = default;

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
    }

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
