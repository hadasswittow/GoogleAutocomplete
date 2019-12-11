//
// Created by hadasswittow on 12/10/19.
//

#ifndef GOOGLEAUTOCOMPLETE_AUTOCOMPLETEDATA_H
#define GOOGLEAUTOCOMPLETE_AUTOCOMPLETEDATA_H


#include <string>
#include <utility>

using std::string;

class AutoCompleteData {
public:
    AutoCompleteData(string  completed_sentence,string  source_text,int offset,int score)
    :completed_sentence(std::move(completed_sentence)),source_text(std::move(source_text)),offset(offset),score(score){};
    AutoCompleteData(const AutoCompleteData& other);
    friend std::ostream & operator << (std::ostream &os, const AutoCompleteData &ac);

    string toString()const;

private:
    string completed_sentence;
    string source_text;
    int offset;
    int score;

};
inline AutoCompleteData::AutoCompleteData(const AutoCompleteData& other){
    completed_sentence = other.completed_sentence;
    source_text = other.source_text;
    offset = other.offset;
    score = other.score;
}
inline string AutoCompleteData::toString() const{
    return std::to_string(score) + " " + source_text + " " + std::to_string(offset) + " " + completed_sentence;
}

inline std::ostream & operator << (std::ostream &os, const AutoCompleteData &ac){
    os<< ac.toString();
    return os;

}
#endif //GOOGLEAUTOCOMPLETE_AUTOCOMPLETEDATA_H
