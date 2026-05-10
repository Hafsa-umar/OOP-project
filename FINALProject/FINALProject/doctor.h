#pragma once
#include <iostream>
#include<string>
#include "person.h"

class Doctor : public Person {
private:
    std::string specialization;
    float fee;

public:
    Doctor();
    Doctor(int ID, std::string n, std::string pass,std::string cont,std::string special, float FEE);

    float getFee() const;
    const std::string getSpecialization() const;

    void setspecialization(const std::string special);
    void setfee(const float f);

    bool operator==(const Doctor &obj) const;
    friend std::ostream& operator<<(std::ostream&, const Doctor&);

    void displayProfile() const override;
    void displayMenu()const override;
    bool login(const std::string& pass) const override;
};

