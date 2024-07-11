#include <iostream>
#include <string>
#include <vector>
#include "../include/invertedindex.h"
#include "gtest/gtest.h"

TEST(InvertedIndex, updateDocumentBase)
{

    InvertedIndex indx;
    vector<string> input_docs = {
        "../resources/file001",
        "../resources/file002",
        "../resources/file003",
        "../resources/file004",
    };
    vector<string> docs_expected = {"milk milk milk milk water water", "milk water water", "milk milk milk milk milk water water water water water", "americano cappuchino"};
    ASSERT_EQ(indx.UpdateDocumentBase(input_docs), docs_expected);
}
TEST(InvertedIndex, GetWordCount)
{
    InvertedIndex index;
    index.Indexation("americano cappuchino", 3);
    vector<Entry> WordCount_expected = {{3, 1}};
    vector<Entry> result;
    result = index.GetWordCount("cappuchino");
    ASSERT_EQ(WordCount_expected, result);
}