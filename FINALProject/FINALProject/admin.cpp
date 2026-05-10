#include "admin.h"
#include <iostream>
#include <string>
using namespace std;

Admin::Admin() {
    id = 0;
    name = " ";
    password = " ";
    contact = "00000000000";
}
Admin::Admin(int ID, const std::string n, const std::string pass, std::string cont):
    Person(ID,n,pass,cont) {
    id = ID;
    name = n;
    password = pass;
    contact = cont;
}
void Admin::displayProfile() const{
    std::cout << "Admin Name: " << name << std:: endl;
    std::cout << "Admin ID: " << id << std::endl;
    std::cout << "Doctor ContactNO: " << contact <<std::endl;
}
void Admin::displayMenu()const {
   std::cout<< "Admin Panel — MediCore"<<std::endl<<
        
       "1. Add Doctor"<<std::endl<<
       "2. Remove Doctor"<< std::endl<<
       "3. View All Patients" << std::endl<<
       "4. View All Doctors " << std::endl<<
       "5. View All Appointments"<< std::endl<<
       "6. View Unpaid Bills" << std::endl<<
       "7. Discharge Patient"<< std::endl<<
       "8. View Security Log"<< std::endl<<
       "9. Generate Daily Report" << std::endl<<
       "10. Logout"<< std::endl;
}
bool Admin::login(const std::string& pass)const {
    return password == pass;
}