//
// Created by lukas on 22. 9. 2026.
//

#include "Student.h"

#include <iostream>
#include <ostream>

Student::Student(std::string _Name) {
    this->name = _Name;
}

void Student::addSubject(Subject& _Subject) {
    this->subjects.push_back(_Subject);
}

void Student::addGrade(std::string _SubjectName, Grade _Grade) {
    for (Subject& subject : subjects) {
        if (subject.getName() == _SubjectName) {
            subject.addGrade(_Grade);
            return;
        }
    }

    Subject newSubject(_SubjectName);
    newSubject.addGrade(_Grade);
    this->subjects.push_back(newSubject);
}

std::string Student::getName() const{
    return this->name;
}

void Student::printGrades() const{
    for (Subject subject : subjects) {
        std::cout << subject.getName() << std::endl;
        for (Grade grade : subject.getGrades()) {
            std::cout << static_cast<int>(grade) <<  " ,";
        }
    }
}

std::vector<Subject>& Student::getSubjects() {
    return this->subjects;
}

