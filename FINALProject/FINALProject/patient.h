#pragma once
#include <iostream>
#include "Person.h"

class Patient : public Person {
private:
    int age;
    std::string gender;
    float balance;

public:
    Patient();
    Patient(int ID, std::string n, std::string pass, std::string cont, int Age, std::string gen, float bal);

    int getage() const;
    float getBalance() const;
    std::string getGender()const;

    void setBalance(float);
    void setAge(int a);
    void setgender(std::string gen);

    void operator+=(double amount);
    void operator-=(double amount);
    bool operator>=(const Patient& obj);



    bool operator==(const Patient &obj) const;

    friend std::ostream& operator<<(std::ostream&, const Patient&);

    void displayProfile() const override;
    void displayMenu() const override;
    bool login(const std::string& pass) const override;
};

