#include "convertjson.h"
#include "invertedindex.h"
#include "searchserver.h"
#include "gtest/gtest.h"

TEST(SearchServer, search)
{
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"};
    const vector<string> request = {"milk water", "sugar"};
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
}
