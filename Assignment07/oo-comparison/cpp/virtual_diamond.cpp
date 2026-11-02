#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    explicit Person(const std::string& name) : name(name) {
        std::cout << "Initializing Person\n";
    }
    virtual std::string role() const { return "Person"; }
    virtual ~Person() = default;
};
class Student : virtual public Person {
public:
    std::string major;
    Student(const std::string& name, const std::string& major)
        : Person(name), major(major) { std::cout << "Initializing Student\n"; }
    std::string role() const override { return "Student"; }
};
class Employee : virtual public Person {
public:
    int employee_id;
    Employee(const std::string& name, int employee_id)
        : Person(name), employee_id(employee_id) {
        std::cout << "Initializing Employee\n";
    }
    std::string role() const override { return "Employee"; }
};
class TeachingAssistant : public Student, public Employee {
public:
    std::string course;
    TeachingAssistant(const std::string& name, const std::string& major,
                      int employee_id, const std::string& course)
        : Person(name), Student(name, major), Employee(name, employee_id), course(course) {
        std::cout << "Initializing TeachingAssistant\n";
    }
    std::string role() const override { return "TeachingAssistant"; }
};
class RobotAssistant {
public:
    std::string role() const { return "RobotAssistant"; }
};
void announce_person(const Person& person) {
    std::cout << person.role() << '\n';
}
template<typename T>
void announce_generic(const T& value) {
    std::cout << value.role() << '\n';
}
int main() {
    TeachingAssistant ta("Alex Rivera", "Computer Science", 1042, "CS 240");
    Person& student_person = static_cast<Student&>(ta);
    Person& employee_person = static_cast<Employee&>(ta);
    std::cout << "Student Person: " << &student_person << ", " << student_person.name << '\n';
    std::cout << "Employee Person: " << &employee_person << ", " << employee_person.name << '\n';
    student_person.name = "Alex Rivera (changed through Student)";
    std::cout << "Student name: " << student_person.name << '\n';
    std::cout << "Employee name: " << employee_person.name << '\n';
    std::cout << "Direct name: " << ta.name << '\n';
    Person* person = &ta;
    std::cout << "Person pointer name: " << person->name << '\n';
    Person& base = ta;
    std::cout << "Role through Person&: " << base.role() << '\n';
    RobotAssistant robot;
    announce_person(ta);
    announce_generic(ta);
    announce_generic(robot);
}
