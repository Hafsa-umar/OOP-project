#pragma once

#include <string>
#include <iostream>

class Validator {
public:
	
	static bool validateId(int id);
	static bool validateDate(const std::string& date);
	static bool validateTimeslot(const std::string& Time);
	static std::string toLowerString( std::string s);
		
	static bool validateContactNumber(const std::string& number);
	
	static bool login(std::string role, int id, std::string password);
    static bool validatePassword(const std::string& password);
	static bool validatePositiveFloat(const std::string& value);
	static int getValidMenuChoice( int min, int max);
};
