//
// Created by hadasswittow on 12/11/19.
//

#ifndef GOOGLEAUTOCOMPLETE_MODEL_H
#define GOOGLEAUTOCOMPLETE_MODEL_H

#include "AutoCompleteData.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include "json.hpp"
#include <experimental/filesystem>

using nlohmann::json;
using std::vector;
using std::unordered_map;
static const unsigned char FILES_SIZE = 2;

class Model {
public:
    vector<AutoCompleteData> getBestKCompletions(const string &prefix);

    vector<string> getCompletions(const string &prefix);

    void initFilesDataTable();

    void parseDataFileToTable();

private:
    bool fillCompletionsVector(const string& prefix, int score,  vector<AutoCompleteData> &completions);
    void characterExchanged(const string& prefix, vector<AutoCompleteData> &completions);
    void characterMissing(const string& prefix, vector<AutoCompleteData> &completions);
    void characterAdded(const string& prefix, vector<AutoCompleteData> &completions);
        unordered_map<string, vector<string>> data_table;
    unordered_map<string, vector<string>> files_data_table;
};


#endif //GOOGLEAUTOCOMPLETE_MODEL_H
