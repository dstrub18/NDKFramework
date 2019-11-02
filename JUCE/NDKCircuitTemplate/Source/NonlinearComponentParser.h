//
// Created by Joakim Borg on 2019-04-08.
//

#pragma once

#include "NonlinearComponent.h"

class NonlinearComponentParser {
public:
    NonlinearComponentParser(std::vector <std::string> componentNames);

    std::unique_ptr<std::vector<NonlinearComponent*>> getNonlinearComponentList();

private:
    void parse(std::vector<std::string> componentNames);

    std::unique_ptr<std::vector<NonlinearComponent*>> mNonlinearComponentList;

};

