//
// Created by lukas on 22. 9. 2026.
//

#ifndef SCHOOLSYSTEM_SUBJECT_H
#define SCHOOLSYSTEM_SUBJECT_H
#include <string>
#include <vector>

#include "Grade.h"


class Subject {
    private:
        std::string name;
        std::vector<Grade> grades;

    public:
        Subject(std::string _Name);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::vector<Grade> &getGrades();
        void addGrade(Grade _Grade);
};


#endif //SCHOOLSYSTEM_SUBJECT_H
