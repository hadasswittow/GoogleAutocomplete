//
// Created by hadasswittow on 12/11/19.
//

#include "Model.h"

void Model::initFilesDataTable() {
    string files[FILES_SIZE] = {"/home/hadasswittow/CLionProjects/GoogleAutocomplete/Moses_and_the_Sages__Bible.txt",
                                "/home/hadasswittow/CLionProjects/GoogleAutocomplete/William_Shakespeare__Hamlet.txt"};
    string files_names[FILES_SIZE] = {"Moses_and_the_Sages__Bible", "William_Shakespeare__Hamlet"};
    std::vector<std::string> lines;
    for (unsigned char i = 0; i < FILES_SIZE; ++i) {
        lines.clear();
        std::ifstream file(files[i]);
        if (file.is_open()) {
            std::string line;
            int i = 0;
            while (getline(file, line)) {
                lines.push_back(line);
            }
        } else {
            throw "file not found!!\n";
        }
        files_data_table[files_names[i]] = lines;
        file.close();
    }


}

vector<string> Model::getCompletions(const string &word) {
    vector<AutoCompleteData> completions = getBestKCompletions(word);
    vector<string> str_vector;
    for (auto it = completions.begin(); it != completions.end(); ++it) {
        str_vector.push_back(it->toString());
    }
    return str_vector;
}

void Model::parseDataFileToTable() {
    std::ifstream file("/home/hadasswittow/CLionProjects/GoogleAutocomplete/data.json");
    if (file.is_open()) {
        json data_json = json::parse(file);
        for (json::iterator it = data_json.begin(); it != data_json.end(); ++it) {
            vector<string> vals = it.value();
            data_table[it.key()] = vals;
        }
    }
    file.close();
}

bool searchAndDeleteLowerScore(int score, vector<AutoCompleteData> &completions) {
    for (auto it = completions.end(); it != completions.begin(); --it) {
        if (it->getScore() < score) {
            completions.erase(it);
            return true;
        }
    }
    return false;
}

bool Model::fillCompletionsVector(const string &prefix, int score, vector<AutoCompleteData> &completions) {
    int index;
    string source;
    string full_sentence;
    int offset;
    vector<string> ans;
    bool filled = false;
    if (data_table.find(prefix) != data_table.end()) {
        ans = data_table[prefix];
        for (auto &it : ans) {
            // if there are enough completions and none of them is with a smaller score return
            // if there is enough completions but there is one with lower score the searchAndDeleteLowerScore will
            // delete it and we will add the better prefix
            if ((completions.size() == 5) && !searchAndDeleteLowerScore(score, completions))
                return filled;
            index = it.find(' ');
            source = it.substr(0, index);
            offset = stoi(it.substr(index + 1));
            full_sentence = files_data_table[source][offset];
            completions.emplace_back(full_sentence, source, offset, score);
            filled = true;
        }
    }
    return filled;
}

int getScore(const string &prefix, int i, bool flag, bool miss = false) {
    // returns the score according to flag- (false for replacement and true for add/delete of letter) and location
    // of the letter that was replaced/added/deleted.
    int score = (prefix.size() - 1 + miss) * 2;
    if (i < 4)
        score -= ((flag + 1) * (5 - i));
    else
        score -= ((flag + 1) * 1);
    return score;
}

void Model::characterExchanged(const string &prefix, vector<AutoCompleteData> &completions) {
    int score;
    bool exchange_found = false;
    string exchanged_prefix;
    string letters = "abcdefghijklmnopqrstuvwxyz1234567890_";
    for (size_t i = prefix.size(); i > 0; --i) {
        score = getScore(prefix, (i - 1), false); // set the score according to the location of the exchange
        for (char let: letters) {
            exchanged_prefix = prefix;
            if (exchanged_prefix[i - 1] != let) {
                exchanged_prefix[i - 1] = let;
                exchange_found = fillCompletionsVector(exchanged_prefix, score, completions);
                // if we have enough completions and we found already an exchanged prefix, we dont need to check other
                // options for exchanged characters because their score will be lower than the current prefix score
                if ((completions.size() == 5) && (exchange_found))
                    return;
            }
        }
    }

}

void Model::characterMissing(const string &prefix, vector<AutoCompleteData> &completions) {
    int score;
    bool missing_found = false;
    string fixed_prefix;
    string letters = "abcdefghijklmnopqrstuvwxyz1234567890_";
    for (size_t i = prefix.size(); i > 0; --i) {
        score = getScore(prefix, i, true, true); // set the score according to the location of the exchange
        for (char let: letters) {
            fixed_prefix = prefix;
            fixed_prefix.insert(fixed_prefix.begin() + i, let);
            missing_found = fillCompletionsVector(fixed_prefix, score, completions);
            // if we have enough completions and we found already a missing prefix so we dont need to check other
            // options for missing caracters because their score will be lower than the current prefix score
            if ((completions.size() == 5) && (missing_found))
                return;
        }
    }
    for (char let: letters) {
        fixed_prefix = prefix;
        score = getScore(prefix, 0, true, true);
        fixed_prefix.insert(fixed_prefix.begin(), let);
        fillCompletionsVector(fixed_prefix, score, completions);
    }

}

void Model::characterAdded(const string &prefix, vector<AutoCompleteData> &completions) {
    int score;
    bool added_found = false;
    string fixed_prefix;
    for (size_t i = prefix.size(); i > 0; --i) {
        score = getScore(prefix, (i - 1), true); // set the score according to the location of the exchange
        fixed_prefix = prefix;
        fixed_prefix.erase(fixed_prefix.begin() + i - 1);
        added_found = fillCompletionsVector(fixed_prefix, score, completions);
        // if we have enough completions and we found already an added char prefix so we dont need to check other
        // options for added characters because their score will be lower than the current prefix score
        if ((completions.size() == 5) && (added_found))
            return;
    }

}

vector<AutoCompleteData> Model::getBestKCompletions(const string &prefix) {
    vector<AutoCompleteData> completions;
    vector<string> ans;
    fillCompletionsVector(prefix, prefix.size() * 2, completions); //first check if the prefix as is is in the table
    if (completions.size() < 5) { // if not enough completions found check and add exchanged/added/missing char in prefix
        characterExchanged(prefix, completions);
        characterAdded(prefix, completions);
        characterMissing(prefix, completions);
    }

    return completions;

}