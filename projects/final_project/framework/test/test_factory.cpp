#include <iostream>
#include <memory>
#include <string>

#include "factory.hpp"

class Shape
{
  public:
    virtual ~Shape() = default;
    virtual void Draw() = 0;
};

class Circle : public Shape
{
    int m_radius;

  public:
    Circle(int radius) : m_radius(radius) {}
    void Draw() override
    {
        std::cout << "Drawing Circle of radius " << m_radius << "\n";
    }
};

class Square : public Shape
{
    int m_side;

  public:
    Square(int side) : m_side(side) {}
    void Draw() override
    {
        std::cout << "Drawing Square of side " << m_side << "\n";
    }
};

void Test1arg(void)
{
    ilrd::Factory<std::string, Shape, int> shapeFactory;
    std::string name1 = "Circle";
    std::string name2 = "Square";

    shapeFactory.Add(name1,
                     [](int size) { return std::make_shared<Circle>(size); });

    shapeFactory.Add(name2,
                     [](int size) { return std::make_shared<Square>(size); });

    auto myCircle = shapeFactory.Create(name1, 10);
    myCircle->Draw();

    auto mySquare = shapeFactory.Create(name2, 5);
    mySquare->Draw();
}

class Employee
{
  public:
    virtual ~Employee() = default;
    virtual void Work() = 0;
};

class Developer : public Employee
{
    std::string m_name;
    int m_experience_years;

  public:
    Developer(std::string name, int experience_years)
        : m_name(name), m_experience_years(experience_years)
    {
    }

    void Work() override
    {
        std::cout << "Developer " << m_name
                  << " is writing code. Experience: " << m_experience_years
                  << " years.\n";
    }
};

class Manager : public Employee
{
    std::string m_name;
    int m_team_size;

  public:
    Manager(std::string name, int team_size)
        : m_name(name), m_team_size(team_size)
    {
    }

    void Work() override
    {
        std::cout << "Manager " << m_name << " is managing a team of "
                  << m_team_size << " people\n";
    }
};

void Test2args(void)
{
    ilrd::Factory<std::string, Employee, std::string, int> employeeFactory;
    std::string name1 = "developer";
    std::string name2 = "manager";

    employeeFactory.Add(name1, [](std::string name, int exp)
                        { return std::make_shared<Developer>(name, exp); });

    employeeFactory.Add(name2, [](std::string name, int size)
                        { return std::make_shared<Manager>(name, size); });

    auto dev = employeeFactory.Create(name1, "Robi", 5);
    dev->Work();

    auto mgr = employeeFactory.Create(name2, "Bar", 12);
    mgr->Work();
}

int main()
{
    Test1arg();
    Test2args();

    return 0;
}