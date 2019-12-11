//
// Created by hadasswittow on 12/11/19.
//

#ifndef GOOGLEAUTOCOMPLETE_CONTROLLER_H
#define GOOGLEAUTOCOMPLETE_CONTROLLER_H

#include "View.h"
#include "Model.h"
class Controller {

public:
    Controller();
    ~Controller();
    void run();

private:
    View* view;
    Model* model;

};
inline Controller::Controller(){
    view = new View;
    model = new Model;
}
inline Controller::~Controller(){
    delete view;
    delete model;
}
inline void Controller::run(){
    model->initFilesDataTable(); // put the files with data to a table in the model
    model->parseDataFileToTable(); // put the offline model to a file
    string input = view->getUsersInput();
    while(!input.empty()){
        vector<string> completions = model->getCompletions(input);
        view->printOutputVectorToUser(completions);
        input = view->getUsersInput();
    }
}

#endif //GOOGLEAUTOCOMPLETE_CONTROLLER_H
