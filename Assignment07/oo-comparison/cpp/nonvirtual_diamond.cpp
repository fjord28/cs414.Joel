#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    explicit Person(const std::string& name) : name(name) {
        std::cout << "Initializing Person\n";
    }
};
class Student : public Person {
public:
    std::string major;
    Student(const std::string& name, const std::string& major)
        : Person(name), major(major) { std::cout << "Initializing Student\n"; }
};
class Employee : public Person {
public:
    int employee_id;
    Employee(const std::string& name, int employee_id)
        : Person(name), employee_id(employee_id) {
        std::cout << "Initializing Employee\n";
    }
};
class TeachingAssistant : public Student, public Employee {
public:
    std::string course;
    TeachingAssistant(const std::string& name, const std::string& major,
                      int employee_id, const std::string& course)
        : Student(name, major), Employee(name, employee_id), course(course) {
        std::cout << "Initializing TeachingAssistant\n";
    }
};
int main() {
    TeachingAssistant ta("Alex Rivera", "Computer Science", 1042, "CS 240");
    Person& student_person = static_cast<Student&>(ta);
    Person& employee_person = static_cast<Employee&>(ta);
    std::cout << "Student Person: " << &student_person << ", " << student_person.name << '\n';
    std::cout << "Employee Person: " << &employee_person << ", " << employee_person.name << '\n';
    student_person.name = "Alex Rivera (changed through Student)";
    std::cout << "Student name: " << student_person.name << '\n';
    std::cout << "Employee name: " << employee_person.name << '\n';
}
