//
// Created by lukas on 22. 9. 2026.
//

#ifndef SCHOOLSYSTEM_SYSTEM_H
#define SCHOOLSYSTEM_SYSTEM_H
#include <fstream>

#include "Student.h"


class System {
    private:
        std::vector<Student*> students;

    public:
        System();

        bool loadFromJson(const std::string &filename);
        void addStudent(Student* student);
        std::vector<Student*> getStudents();

        bool saveToJson(const std::string &filename);
};


#endif //SCHOOLSYSTEM_SYSTEM_H
