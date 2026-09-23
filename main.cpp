#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>

#include "System.h"

void refreshStudentDetails(QListWidget *detailsList, Student *student) {
    detailsList->clear();

    if (student == nullptr) return;

    for (Subject &s : student->getSubjects()) {
        auto *subjectItem = new QListWidgetItem("Predmet: " + QString::fromStdString(s.getName()));
        QFont font = subjectItem->font();
        font.setBold(true);
        subjectItem->setFont(font);
        detailsList->addItem(subjectItem);

        for (const Grade &grade : s.getGrades()) {
            int gradeValue = static_cast<int>(grade);
            detailsList->addItem("  • Známka: " + QString::number(gradeValue));
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *system = new System();
    system->loadFromJson("Student.json");

    auto *window = new QMainWindow();
    window->resize(500, 400);
    window->setWindowTitle("Správa Študentov");

    auto *centralWidget = new QWidget(window);
    window->setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    auto *studentList = new QListWidget();

    for (Student *s : system->getStudents()) {
        if (s != nullptr) {
            auto *item = new QListWidgetItem(QString::fromStdString(s->getName()));
            item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(s)));
            studentList->addItem(item);
        }
    }

    QObject::connect(studentList, &QListWidget::itemClicked, [window, system](QListWidgetItem *item) {
        void *ptr = item->data(Qt::UserRole).value<void*>();
        auto *student = static_cast<Student*>(ptr);

        if (student == nullptr) return;

        auto *windowStudent = new QMainWindow();
        windowStudent->setAttribute(Qt::WA_DeleteOnClose);
        windowStudent->resize(500, 450);
        windowStudent->setWindowTitle("Študent: " + QString::fromStdString(student->getName()));

        auto *studentWidget = new QWidget(windowStudent);
        windowStudent->setCentralWidget(studentWidget);

        auto *studentLayout = new QVBoxLayout(studentWidget);
        studentLayout->setContentsMargins(15, 15, 15, 15);
        studentLayout->setSpacing(10);

        auto *detailsList = new QListWidget();

        auto *subjectInput = new QLineEdit();
        subjectInput->setPlaceholderText("Názov predmetu (napr. Matematika)...");

        auto *gradeInput = new QLineEdit();
        gradeInput->setPlaceholderText("Známka (1-5)...");

        auto *addGradeButton = new QPushButton("Pridať známku");

        auto *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(subjectInput);
        inputLayout->addWidget(gradeInput);
        inputLayout->addWidget(addGradeButton);

        studentLayout->addWidget(detailsList);
        studentLayout->addLayout(inputLayout);

        refreshStudentDetails(detailsList, student);

        QObject::connect(addGradeButton, &QPushButton::clicked, [windowStudent, system, student, subjectInput, gradeInput, detailsList]() {
            QString subjectText = subjectInput->text().trimmed();
            QString gradeText = gradeInput->text().trimmed();

            if (subjectText.isEmpty() || gradeText.isEmpty()) {
                QMessageBox::warning(windowStudent, "Chyba", "Musíte zadať predmet aj známku!");
                return;
            }

            bool ok = false;
            int gradeVal = gradeText.toInt(&ok);

            if (!ok || gradeVal < 1 || gradeVal > 5) {
                QMessageBox::warning(windowStudent, "Chyba", "Známka musí byť číslo 1-5!");
                return;
            }

            std::string subjectName = subjectText.toStdString();
            Grade newGrade = static_cast<Grade>(gradeVal);

            student->addGrade(subjectName, newGrade);

            bool saved = system->saveToJson("Student.json");
            if (!saved) {
                QMessageBox::critical(windowStudent, "Chyba", "Nepodarilo sa uložiť dáta do súboru!");
            }

            refreshStudentDetails(detailsList, student);

            gradeInput->clear();
            subjectInput->clear();
        });

        window->hide();
        windowStudent->show();

        QObject::connect(windowStudent, &QWidget::destroyed, window, &QWidget::show);
    });

    auto *nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Zadaj meno študenta...");

    auto *addButton = new QPushButton("Pridať študenta");

    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(nameInput);
    inputLayout->addWidget(addButton);

    mainLayout->addWidget(studentList);
    mainLayout->addLayout(inputLayout);
    centralWidget->setLayout(mainLayout);

    QObject::connect(addButton, &QPushButton::clicked, [window, system, nameInput, studentList]() {
        QString qtName = nameInput->text().trimmed();

        if (qtName.isEmpty()) {
            QMessageBox::warning(window, "Chyba", "Meno študenta nemôže byť prázdne!");
            return;
        }

        std::string cppName = qtName.toStdString();

        auto *newStudent = new Student(cppName);
        system->addStudent(newStudent);

        auto *item = new QListWidgetItem(qtName);
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(newStudent)));
        studentList->addItem(item);

        nameInput->clear();
    });

    QObject::connect(nameInput, &QLineEdit::returnPressed, addButton, &QPushButton::animateClick);

    window->show();
    return QApplication::exec();
}