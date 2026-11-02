class Person:
    def __init__(self, *, name, **kwargs):
        print(f"Entering Person.__init__, id={id(self)}")
        self.name = name
        super().__init__(**kwargs)

    def role(self):
        return "Person"

class Student(Person):
    def __init__(self, *, major, **kwargs):
        print(f"Entering Student.__init__, id={id(self)}")
        self.major = major
        super().__init__(**kwargs)

    def rename_as_student(self, name):
        self.name = name

    def role(self):
        return "Student"

class Employee(Person):
    def __init__(self, *, employee_id, **kwargs):
        print(f"Entering Employee.__init__, id={id(self)}")
        self.employee_id = employee_id
        super().__init__(**kwargs)

    def rename_as_employee(self, name):
        self.name = name

    def role(self):
        return "Employee"

class TeachingAssistant(Student, Employee):
    def __init__(self, *, course, **kwargs):
        print(f"Entering TeachingAssistant.__init__, id={id(self)}")
        self.course = course
        super().__init__(**kwargs)

class ReversedTeachingAssistant(Employee, Student):
    def __init__(self, *, course, **kwargs):
        print(f"Entering ReversedTeachingAssistant.__init__, id={id(self)}")
        self.course = course
        super().__init__(**kwargs)

class RobotAssistant:
    def role(self):
        return "RobotAssistant"

def announce(value):
    print(value.role())

def main():
    ta = TeachingAssistant(
        name="Alex Rivera",
        major="Computer Science",
        employee_id=1042,
        course="CS 240",
    )
    print("MRO:", TeachingAssistant.__mro__)
    print("State:", vars(ta))
    ta.rename_as_student("Alex Rivera (changed through Student)")
    print("After Student rename:", ta.name)
    ta.rename_as_employee("Alex Rivera (changed through Employee)")
    print("After Employee rename:", ta.name)
    print("Prediction without TA override: Student")
    print("Observed:", ta.role())

    def role(self):
        return "TeachingAssistant"

    TeachingAssistant.role = role
    print("With TA override:", ta.role())
    reversed_ta = ReversedTeachingAssistant(
        name="Alex Rivera", major="Computer Science",
        employee_id=1042, course="CS 240",
    )
    print("Reversed MRO:", ReversedTeachingAssistant.__mro__)
    print("Reversed role:", reversed_ta.role())
    announce(ta)
    announce(RobotAssistant())
    try:
        announce(42)
    except AttributeError as error:
        print(f"{type(error).__name__}: {error}")


if __name__ == "__main__":
    main()
