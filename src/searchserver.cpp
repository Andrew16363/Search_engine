#include <string>
#include <vector>
#include <sstream>
#include "include/invertedindex.h"
#include "include/convertjson.h"
#include "include/searchserver.h"

using namespace std;

bool compare::operator()(const RelativeIndex &relative_index1, const RelativeIndex &relative_index2)
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

vector<vector<RelativeIndex>> SearchServer::search(const vector<string> &queries_input)
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
