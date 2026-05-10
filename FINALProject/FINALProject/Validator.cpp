#include "Validator.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;


int getLength(const std::string& str) {
	int i = 0;
	int count = 0;
	while (str[i] != '\0') {
		count++;
        i++;
	}
	return count;

}

bool Validator::validateId(int id) {
	return id > 0;
}
bool Validator::validateDate(const std::string& date) {
    if (getLength(date) != 10)
        return false;

    if (date[2] != '-' || date[5] != '-')
        return false;
    for (int i = 0; i < 10; i++) {

        if (i == 2 || i == 5)
            continue;

        if (date[i] < '0' || date[i] > '9')
            return false;
    }
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 +
        (date[7] - '0') * 100 +
        (date[8] - '0') * 10 +
        (date[9] - '0');
    if (day < 1 || day > 31)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (year < 2026)
        return false;

    return true;
}

bool Validator::validateTimeslot(const std::string& time) {

	int len = getLength(time);
	if (len != 11)
		return false;

	for (int i = 0;i < len;i++) {
		if (time[2] != ':' || time[5] != '-' || time[8]!=':')
			return false;

		for (int i = 0;i < len;i++) {
			if (i == 2 || i == 5 || i == 8) {
				continue;
			}
	}
		if(time[i] < '0' || time[i] > '9')
			return false;
	}

	return true;
}

bool Validator::validateContactNumber(const std::string& number) {

    int len = getLength(number);
    cout << "taking length" << endl;
    if (len < 10 || len > 15) {
        cout << "Invalid contact number length" << endl;
        return false;
    }

    for (int i = 0; i < len; i++) {

        if (number[i] < '0' || number[i] > '9') {

            cout << "Contact number must contain digits only" << endl;
            return false;
        }
    }

    return true;
}
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool Validator::login(string role, int id, string password) {

    ifstream fin;

    if (role == "patient")
        fin.open("patients.txt");

    else if (role == "doctor")
        fin.open("doctors.txt");

    else if (role == "admin")
        fin.open("admin.txt");

    else {
        cout << "Invalid role!" << endl;
        return false;
    }

    if (!fin) {
        cout << "File not found!" << endl;
        return false;
    }

    string line;

    while (getline(fin, line)) {

        stringstream ss(line);

        int fileId;
        char line;

        ss >> fileId >> line;   
        if (role == "admin") {

            string filePassword;
            getline(ss, filePassword);

            if (fileId == id && filePassword == password) {

                cout << endl<<"Login Successful"<<endl;
                cout << "WELCOME ADMIN"<<endl;
                cout << "Admin ID: " << fileId << endl;

                return true;
            }
        }

        
        else if (role == "patient") {

            string name, filePassword, contact, balance;

            

            getline(ss, name, '|');
            getline(ss, filePassword, '|');
            getline(ss, contact, '|');
            getline(ss, balance, '|');

            if (fileId == id && filePassword == password) {

                cout << endl<<"Login Successful" << endl;;
                cout << "WELCOME PATIENT\n";
                cout << "Name: " << name << endl;
                cout << "ID: " << fileId << endl;
                cout << "Balance: " << balance << endl;

                return true;
            }
        }

        // ---------------- DOCTOR ----------------
        else if (role == "doctor") {

            string name, filePassword, specialization, fee, contact;
            

            getline(ss, name, '|');
            getline(ss, filePassword, '|');
            getline(ss, specialization, '|');
            getline(ss, fee, '|');
            getline(ss, contact, '|');

            if (fileId == id && filePassword == password) {

                cout << endl<<"Login Successful"<<endl;
                cout << "WELCOME DOCTOR"<<endl;
                cout << "Name: " << name << endl;
                cout << "Specialization: " << specialization << endl;
                cout << "Consultation Fee: " << fee << endl;
                cout << "Contact: " << contact << endl;

                return true;
            }
        }
    }

    cout << "Invalid credentials!" << endl;
    return false;
}


bool Validator::validatePassword(const std::string& password) {
    
    int len = getLength(password);

    if (len < 8)
        return false;

    bool hasDigit = false;
    bool hasUpper = false;

    for (int i = 0; i < len; i++) {
        char c = password[i];

        if (c >= '0' && c <= '9')
            hasDigit = true;

        if (c >= 'A' && c <= 'Z')
            hasUpper = true;
    }

    return 
        hasDigit && hasUpper;
}

bool Validator::validatePositiveFloat(const std::string& value) {
    int len = getLength(value);
    if (len == 0) 
        return false;

    bool hasDigit = false;
    int dotCount = 0;

    for (int i = 0; i < len; i++) {
        char c = value[i];

        if (c == '.') {
            dotCount++;
            if (dotCount > 1) 
                return false;
        }
        else if 
            (c >= '0' && c <= '9') {
            hasDigit = true;
        }
        else {
            return false;
        }
    }

    if (!hasDigit) 
        return false;


    double num = 0;
    double decimalPlace = 1;
    bool afterDot = false;

    for (int i = 0; i < len; i++) {
        char c = value[i];

        if (c == '.') {
            afterDot = true;
            continue;
        }

        if (!afterDot) {
            num = num * 10 + (c - '0');
        }
        else {
            decimalPlace *= 10;
            num = num + (c - '0') / decimalPlace;
        }
    }

    return 
        num > 0;
}
std::string Validator :: toLowerString(std::string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

int Validator::getValidMenuChoice(int min, int max) {

    string input;

    while (true) {

        cin >> input;

        bool isNumber = true;
        int value = 0;

       
        for (char c : input) {

            if (c < '0' || c > '9') {
                isNumber = false;
                break;
            }

           
            value = value * 10 + (c - '0');
        }

        if (!isNumber) {
            cout << "Invalid input! Enter numbers only: ";
            continue;
        }

        if (value < min || value > max) {
            cout << "Out of range! Try again: ";
            continue;
        }

        return value;
    }
}





