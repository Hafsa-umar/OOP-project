#pragma once
#include <string>

class Person {
protected:
    int id;
    std::string name;
    std::string password;
    std::string contact;

public:
    Person();
    Person(int ID, const std::string n, const std::string pass,std::string cont);

    int getId() const;
    const std::string getName() const;
    const std::string getpassword() const;
    const std::string getContact() const;

    void setname(std::string n);
    void setpassword(std::string pass);
    void setContact(std::string cont);
    void setID(int id);

    virtual void displayProfile() const = 0;
    virtual void displayMenu() const = 0;
    virtual bool login(const std::string& pass) const = 0;

    virtual ~Person();
};

