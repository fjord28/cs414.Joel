# C++ and Python Object Models

## 1. Ordinary C++ diamond

A class is a type definition. A base-class subobject is an actual base-class portion inside an object. There is only one Person class definition here, but the teaching assistant contains two Person subobjects: one inside Student and one inside Employee. Each holds its own `name`.

Selected output:

```text
Initializing Person
Initializing Student
Initializing Person
Initializing Employee
Initializing TeachingAssistant
Student Person: 0x7fffecfea6c0, Alex Rivera
Employee Person: 0x7fffecfea700, Alex Rivera
Student name: Alex Rivera (changed through Student)
Employee name: Alex Rivera
```

The different addresses show separate Person subobjects. Changing the student's name does not change the employee's name. Two Person constructors run because each intermediate base initializes its own Person subobject. `ta.name` is ambiguous because either name could be intended. Converting `&ta` to `Person*` is also ambiguous because there are two possible Person targets. Explicitly selecting Student or Employee resolves the path.

## 2. Virtual C++ diamond

Virtual inheritance makes the two paths share one Person subobject. Both references now have the same address and observe the same changed name:

```text
Initializing Person
Initializing Student
Initializing Employee
Initializing TeachingAssistant
Student Person: 0x7ffd105f2bf8, Alex Rivera
Employee Person: 0x7ffd105f2bf8, Alex Rivera
Student name: Alex Rivera (changed through Student)
Employee name: Alex Rivera (changed through Student)
Direct name: Alex Rivera (changed through Student)
Person pointer name: Alex Rivera (changed through Student)
Role through Person&: TeachingAssistant
```

TeachingAssistant is the most-derived class, so it initializes the shared virtual Person base. Person has no default constructor, making the explicit `Person(name)` initializer necessary. Student and Employee also provide Person initializers so they can be constructed as complete objects themselves; those initializers are ignored when constructing the TA's virtual base.

Virtual inheritance controls base-subobject sharing. Virtual functions control runtime method dispatch. Calling `role()` through Person& uses TeachingAssistant's override because that is the object's dynamic type.

Sharing Person does not choose between Student's and Employee's competing overrides. Removing only TeachingAssistant's override leaves two final overriders for the same virtual function, making the class ill-formed. Restoring the TA override gives one final overrider.

## 3. Python cooperative initialization

The normal MRO is `TeachingAssistant, Student, Employee, Person, object`. The trace uses the same instance ID at every step:

```text
Entering TeachingAssistant.__init__, id=140515124636192
Entering Student.__init__, id=140515124636192
Entering Employee.__init__, id=140515124636192
Entering Person.__init__, id=140515124636192
State: {'course': 'CS 240', 'major': 'Computer Science', 'employee_id': 1042, 'name': 'Alex Rivera'}
After Student rename: Alex Rivera (changed through Student)
After Employee rename: Alex Rivera (changed through Employee)
Prediction without TA override: Student
Observed: Student
With TA override: TeachingAssistant
Reversed role: Employee
```

Python does not automatically place a separate Person subobject along each inheritance path. These methods all receive the same `self`, and this implementation stores the four attributes in that instance's dictionary. Both rename methods assign the same `name` entry.

Each initializer takes its own keyword argument and forwards the rest. Person consumes name, leaving no arguments for object.__init__. `super()` continues after the current class in the instance's MRO. For a normal TA, Student's `super()` reaches Employee, even though Employee is not Student's declared parent. With reversed bases, Employee continues to Student instead.

Without a TA override, Python finds Student.role first. Adding TeachingAssistant.role makes it the first match. Reversing the bases gives `ReversedTeachingAssistant, Employee, Student, Person, object`, so Employee.role wins. Python's C3 MRO orders this lookup; it does not apply C++'s unique-final-overrider rule. C++ requires a single final overrider for the shared virtual function instead of choosing by base declaration order.

This does not eliminate all conflicts. Two initializers can overwrite the same attribute, incompatible keyword requirements can break the chain, and omitting a cooperative super() call can skip initialization. Unexpected leftover keywords cause object.__init__ to reject the call.

## 4. Unrelated types and polymorphism

| Call | Result |
| --- | --- |
| C++ `announce_person(ta)` | Compiles; runtime virtual dispatch prints TeachingAssistant |
| C++ `announce_person(robot)` | Rejected; RobotAssistant cannot bind to Person& |
| C++ `announce_generic(ta)` | Compiles; prints TeachingAssistant |
| C++ `announce_generic(robot)` | Compiles; prints RobotAssistant |
| C++ `announce_generic(42)` | Rejected during template instantiation; int has no role() |
| Python `announce(ta)` | Prints TeachingAssistant |
| Python `announce(RobotAssistant())` | Prints RobotAssistant |
| Python `announce(42)` | Raises AttributeError, which is caught |

C++ inheritance-based runtime polymorphism requires the Person relationship and a virtual function. Templates support unrelated types by compiling the required expression for each used type. Template selection happens at compile time; a call inside a template can still use virtual dispatch when the selected method is virtual.

Python duck typing checks whether the operation works when execution reaches it. The integer test prints:

```text
AttributeError: 'int' object has no attribute 'role'
```

Integers inherit from object, but object does not define role(). Sharing that universal base therefore does not guarantee the required operation. C++ detects the missing template operation at compile time; this Python example detects it at runtime.

## 5. Design choice

I would choose composition for this personnel model. A Person could hold separate student and employee role objects, with course information attached to the teaching assignment. Someone could gain or lose a role without changing their class. It also avoids the diamond and makes shared identity easier to manage. Multiple inheritance works for this small demonstration, but composition would be easier to extend as university roles change.

## Compiler evidence

Tested with GCC 13.3.0 using C++20 and `-Wall -Wextra -pedantic`. Both submitted C++ programs compiled without warnings, and all three working programs finished normally. Each failing experiment below was compiled separately from a temporary copy; submitted files retain the working versions. Temporary paths and line numbers identify those test copies.

### Ambiguous name

```text
/tmp/oo_case_0.cpp: In function ‘int main()’:
/tmp/oo_case_0.cpp:44:21: error: request for member ‘name’ is ambiguous
   44 |     std::cout << ta.name << '\n';
      |                     ^~~~
/tmp/oo_case_0.cpp:6:17: note: candidates are: ‘std::string Person::name’
    6 |     std::string name;
      |                 ^~~~
/tmp/oo_case_0.cpp:6:17: note:                 ‘std::string Person::name’
```

### Ambiguous base conversion

```text
/tmp/oo_case_1.cpp: In function ‘int main()’:
/tmp/oo_case_1.cpp:45:22: error: ‘Person’ is an ambiguous base of ‘TeachingAssistant’
   45 |     Person* person = &ta;
      |                      ^~~
/tmp/oo_case_1.cpp:45:13: warning: unused variable ‘person’ [-Wunused-variable]
   45 |     Person* person = &ta;
      |             ^~~~~~
```

### Competing final overriders

```text
/tmp/oo_case_2.cpp:29:7: error: no unique final overrider for ‘virtual std::string Person::role() const’ in ‘TeachingAssistant’
   29 | class TeachingAssistant : public Student, public Employee {
      |       ^~~~~~~~~~~~~~~~~
/tmp/oo_case_2.cpp: In instantiation of ‘void announce_generic(const T&) [with T = TeachingAssistant]’:
/tmp/oo_case_2.cpp:68:21:   required from here
/tmp/oo_case_2.cpp:49:24: error: request for member ‘role’ is ambiguous
   49 |     std::cout << value.role() << '\n';
      |                  ~~~~~~^~~~
/tmp/oo_case_2.cpp:27:17: note: candidates are: ‘virtual std::string Employee::role() const’
   27 |     std::string role() const override { return "Employee"; }
      |                 ^~~~
/tmp/oo_case_2.cpp:18:17: note:                 ‘virtual std::string Student::role() const’
   18 |     std::string role() const override { return "Student"; }
      |                 ^~~~
```

### Unrelated RobotAssistant

```text
/tmp/oo_case_3.cpp: In function ‘int main()’:
/tmp/oo_case_3.cpp:67:21: error: invalid initialization of reference of type ‘const Person&’ from expression of type ‘RobotAssistant’
   67 |     announce_person(robot);  // RobotAssistant is unrelated to Person.
      |                     ^~~~~
/tmp/oo_case_3.cpp:44:36: note: in passing argument 1 of ‘void announce_person(const Person&)’
   44 | void announce_person(const Person& person) {
      |                      ~~~~~~~~~~~~~~^~~~~~
```

### Missing template operation

```text
/tmp/oo_case_4.cpp: In instantiation of ‘void announce_generic(const T&) [with T = int]’:
/tmp/oo_case_4.cpp:70:21:   required from here
/tmp/oo_case_4.cpp:49:24: error: request for member ‘role’ in ‘value’, which is of non-class type ‘const int’
   49 |     std::cout << value.role() << '\n';
      |                  ~~~~~~^~~~
```
