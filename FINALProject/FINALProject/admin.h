#pragma once
#include "person.h"
#include <string>

class Admin :public Person {
public:
	Admin();
	Admin(int ID, const std::string n, const std::string pass, std::string cont);
	void displayProfile()const override;
	void displayMenu()const override;
	bool login(const std::string& pass) const override;
};