//
// Created by lukas on 22. 9. 2026.
//

#include "System.h"

#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

System::System() {

}

bool System::loadFromJson(const std::string& filename) {
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;

    // 1. Vyčistíme doterajšie dáta v pamäti, aby sme nezdvojovali študentov
    for (Student* s : this->students) {
        delete s;
    }
    this->students.clear();

    QJsonArray studentsArray = doc.array();
    for (QJsonValue studentVal : studentsArray) {
        QJsonObject studentObj = studentVal.toObject();
        std::string name = studentObj["name"].toString().toStdString();

        Student* student = new Student(name);

        QJsonArray subjectsArray = studentObj["subjects"].toArray();
        for (QJsonValue subjectVal : subjectsArray) {
            QJsonObject subjectObj = subjectVal.toObject();
            std::string subjectName = subjectObj["name"].toString().toStdString();

            // Explicitne pridáme predmet študentovi (ak si vytvoril nového študenta, ešte nemá žiadne predmety!)
            Subject newSubject(subjectName);

            QJsonArray gradesArray = subjectObj["grades"].toArray();
            for (QJsonValue gradeVal : gradesArray) {
                int gradeInt = gradeVal.toInt(); // Přečítame číslo známky
                newSubject.addGrade(static_cast<Grade>(gradeInt)); // Pridáme známku do predmetu
            }

            // Pridáme kompletný predmet aj so známkami študentovi
            student->addSubject(newSubject);
        }

        this->students.push_back(student);
    }

    return true;
}

void System::addStudent(Student *student) {
    if (student == nullptr) return;
    this->students.push_back(student);
    this->saveToJson("Student.json");
}

std::vector<Student*> System::getStudents() {
    return this->students;
}

bool System::saveToJson(const std::string& filename) {
    QJsonArray studentsArray;

    for (Student* student : this->students) {
        if (student == nullptr) continue;

        QJsonObject studentObj;
        studentObj["name"] = QString::fromStdString(student->getName());

        QJsonArray subjectsArray;
        for (Subject& subject : student->getSubjects()) {
            QJsonObject subjectObj;
            subjectObj["name"] = QString::fromStdString(subject.getName());

            QJsonArray gradesArray;
            for (const Grade& grade : subject.getGrades()) {
                gradesArray.append(static_cast<int>(grade)); // Konverzia známky na číslo
            }

            subjectObj["grades"] = gradesArray;
            subjectsArray.append(subjectObj);
        }

        studentObj["subjects"] = subjectsArray;
        studentsArray.append(studentObj);
    }

    // Uloženie JSON dokumentu do súboru
    QJsonDocument doc(studentsArray);
    QFile file(QString::fromStdString(filename));

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented)); // Indented pre pekné formátovanie
    file.close();
    return true;
}


