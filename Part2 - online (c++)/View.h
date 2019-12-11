//
// Created by hadasswittow on 12/11/19.
//

#ifndef GOOGLEAUTOCOMPLETE_VIEW_H
#define GOOGLEAUTOCOMPLETE_VIEW_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

class View {
public:
    std::string getUsersInput();
    void printOutputToUser(const std::string& output);
    void printOutputVectorToUser(const std::vector<std::string>&);
private:


};
inline std::string View::getUsersInput(){
    std::cout<<"Enter your word: "<<std::endl;
    std::string input;
    getline(std::cin, input);
    input = std::regex_replace(input, std::regex("[\\W]+[\\s]*"), " ");
    return input;
}
inline void View::printOutputVectorToUser(const std::vector<std::string>& output){
    for(auto it = output.begin();it!= output.end();++it){
        std::cout<<*it<<std::endl;
    }
}
inline void View::printOutputToUser(const std::string& output){
    std::cout<<output<<std::endl;
}
#endif //GOOGLEAUTOCOMPLETE_VIEW_H
