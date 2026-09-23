//
// Created by lukas on 22. 9. 2026.
//

#include "Subject.h"

#include <utility>

Subject::Subject(std::string _Name) {
    this->name = std::move(_Name);
}

std::string Subject::getName() const {
    return this->name;
}

std::vector<Grade>& Subject::getGrades()  {
    return grades;
}

void Subject::addGrade(Grade _Grade) {
    this->grades.push_back(_Grade);
}
