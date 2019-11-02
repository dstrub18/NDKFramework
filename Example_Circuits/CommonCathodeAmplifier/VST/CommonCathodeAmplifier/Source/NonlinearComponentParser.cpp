/*
  ==============================================================================

    NonlinearComponentParser.cpp
    Created: 8 Apr 2019 2:17:58pm
    Author:  Daniel Strübig

  ==============================================================================
*/

#include "NonlinearComponentParser.h"
#include "Diode1N914.h"
#include "Npn2N2222.h"
#include "Triode12AX7.h"
#include <iostream>

NonlinearComponentParser::NonlinearComponentParser(std::vector<std::string> componentNames){
    mNonlinearComponentList = std::make_unique<std::vector<NonlinearComponent*>>();
    NonlinearComponentParser::parse(componentNames);

}



void NonlinearComponentParser::parse(std::vector<std::string> componentNames){

    for (int i = 0; i < componentNames.size(); ++i) {
        
        
        if (componentNames[i]=="1N914") {
            // TODO: Not exception safe, fix with unique_ptr
            mNonlinearComponentList->emplace_back(new Diode1N914());
        }
        if (componentNames[i]=="2N2222") {
            // TODO: Not exception safe, fix with unique_ptr
            mNonlinearComponentList->emplace_back(new Npn2N2222());
        }
        if (componentNames[i]=="12AX7") {
            // TODO: Not exception safe, fix with unique_ptr
            mNonlinearComponentList->emplace_back(new Triode12AX7());
        }
    }
}

std::unique_ptr<std::vector<NonlinearComponent*>> NonlinearComponentParser::getNonlinearComponentList() {
    return std::move(mNonlinearComponentList);
}
