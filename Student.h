//
// Created by lukas on 22. 9. 2026.
//

#ifndef SCHOOLSYSTEM_STUDENT_H
#define SCHOOLSYSTEM_STUDENT_H
#include <string>
#include <vector>

#include "Grade.h"
#include "Subject.h"


class Student {
    private:
        std::string name;
        std::vector<Subject> subjects;


    public:
        Student(std::string _Name);
        void addSubject(Subject& _Subject);
        void addGrade(std::string _SubjectName, Grade _Grade);
        [[nodiscard]] std::string getName() const;
        void printGrades() const;
        std::vector<Subject>& getSubjects();
};


#endif //SCHOOLSYSTEM_STUDENT_H
