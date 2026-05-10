#pragma once
#include "managing.h"
#include "Validator.h"

class Menu {

private:
    Manager sys;
    Validator validator;

public:
    void start();
    void adminMenu();
    void doctorMenu();
    void patientMenu();
};