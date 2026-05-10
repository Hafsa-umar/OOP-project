#include "menu.h"
#include <iostream>
#include <fstream>

using namespace std;

void Menu::start() {

    int choice;

    while (true) {

        cout << endl<<"===== HEALING HORIZON SYSTEM ====="<<endl;
        cout << "SELECT USER TYPE" << endl;
        cout << "1. Admin"<<endl;
        cout << "2. Doctor"<<endl;
        cout << "3. Patient"<<endl;
        cout << "4. Exit"<<endl;

        cout << "Enter choice: ";
        choice = validator.getValidMenuChoice(1, 4);

        if (choice == 1) {
            adminMenu();
        }
        else if (choice == 2) {

            doctorMenu();
        }
        else if (choice == 3) {
            patientMenu();
        }
        else {
            cout << "Exiting system..."<<endl;
            break;
        }
    }
}
void Menu::adminMenu() {

    int adminId;
    string password;

    cout << endl<<"===== ADMIN LOGIN ====="<<endl;

    cout << "Enter YOUR id!" << endl;
    adminId = validator.getValidMenuChoice(1, 99999);

    cout << "Enter YOUR Password: "<<endl;
    cin >> password;

    if (!validator.login("admin", adminId, password)) {
        ofstream fout("security_log.txt", ios::app);

        fout << "Admin with ID: " << adminId << " login failed" << endl;

        fout.close();
        cout << "Invalid credentials!" << endl;
        return;
    }
    else {
        ofstream fout("security_log.txt", ios::app);

        fout << "Admin with ID: " << adminId << " login successful" << endl;

        fout.close();

    }

    int choice;

    while (true) {

        cout << endl<<"================================="<<endl;
        cout <<    "   ADMIN PANEL - HEALING HORIZON   "<<endl;
        cout <<    "================================="<<endl;

        cout << "1. Add Doctor"<<endl;
        cout << "2. Remove Doctor"<<endl;
        cout << "3. View All Patients"<<endl;
        cout << "4. View All Doctors"<<endl;
        cout << "5. View All Appointments"<<endl;
        cout << "6. View Unpaid Bills"<<endl;
        cout << "7. Discharge Patient" << endl;
        cout << "8. View Security Log"<<endl;
        cout << "9. Logout"<<endl;

        cout << "Enter choice: ";
        choice = validator.getValidMenuChoice(1, 9);

        if (choice == 9) {
            cout << "Logging out..."<<endl;
            break;
        }

        switch (choice) {

        case 1:
            sys.addDoctor();
            break;

        case 2:
            sys.removedoctor();
            break;

        case 3:
            sys.viewAllpatients();
            break;

        case 4:
            sys.ViewAllDoctors();
            break;

        case 5:
            sys.ViewAllAppointments();
            break;

        case 6:
            sys.viewUnpaidBills();
            break;

        case 7:
            sys.dischargePatient();
            break;

        case 8:
            sys.viewSecurityLog();
            break;
        }
    }
}
void Menu::patientMenu() {

    int patientId;
    string password;

    cout << endl<<"===== PATIENT LOGIN ====="<<endl;

    cout << "Enter YOUR Id!"<<endl;
    patientId = validator.getValidMenuChoice(1, 99999);

    cout << "Enter YOUR Password: "<<endl;
    cin >> password;

  
    if (!validator.login("patient", patientId, password)) {

        ofstream fout("security_log.txt", ios::app);

        fout << "Patient with ID: " << patientId << " login was not successful" << endl;

        fout.close();
        cout << "Invalid credentials!"<<endl;
        return;
    }
    else {
        ofstream fout("security_log.txt", ios::app);

        fout << "Patient with ID: " << patientId << " login successful" << endl;

        fout.close();
        
    }
    int choice;

    while (true) {

        cout << endl<<"================================"<<endl;
        cout << endl<<" PATIENT PANEL - HEALING HORIZON "<<endl;
        cout <<       "================================="<<endl;

        cout << "1. Book Appointment"<<endl;
        cout << "2. Cancel Appointment"<<endl;
        cout << "3. View My Appointments"<<endl;
        cout << "4. View My Medical Records"<<endl;
        cout << "5. View My Bills"<<endl;
        cout << "6. Pay Bill"<<endl;
        cout << "7. Top Up Balance"<<endl;
        cout << "8. Logout"<<endl;

        cout << "Enter choice: ";
        choice = validator.getValidMenuChoice(1, 8);

        if (choice == 8) {
            cout << "Logging out..."<<endl;
            
            break;
        }

        switch (choice) {

        case 1:
            sys.bookAppointment(patientId);
            break;

        case 2:
            sys.CancelAppointment(patientId);
            break;

        case 3:
            sys.ViewAppointment(patientId);
            break;

        case 4:
            sys.ViewMedicalReports(patientId);
            break;

        case 5:
            sys.ViewBill(patientId);
            break;

        case 6:
            sys.Paybill(patientId);
            break;

        case 7:
            sys.topupBalance(patientId);
            break;
        }
    }
}
void Menu::doctorMenu() {

    int doctorId;
    string password;

    cout << endl<<"===== DOCTOR LOGIN ====="<<endl;

    
    doctorId = validator.getValidMenuChoice(1, 99999);

    cout << "Enter Password: ";
    cin >> password;

    
    if (!validator.login("doctor", doctorId, password)) {
        ofstream fout("security_log.txt", ios::app);

        fout << "Doctor" << doctorId << " login was not successful" << endl;

        fout.close();
        cout << "Invalid credentials!"<<endl;
        return;
    }
    else{
        ofstream fout("security_log.txt", ios::app);

        fout << "Doctor "<<doctorId<<" login successful" << endl;

        fout.close();
        
    }
    
        int choice;

        while (true) {

            cout << endl << "=================================" << endl;
            cout << " DOCTOR PANEL - HEALING HORIZON  " << endl;
            cout << " =================================" << endl;

            cout << "1. View Today's Appointments" << endl;
            cout << "2. Mark Appointment Complete" << endl;
            cout << "3. Mark No-Show" << endl;
            cout << "4. Write Prescription" << endl;
            cout << "5. View Patient Medical History" << endl;
            cout << "6. Logout" << endl;

            cout << "Enter choice: ";
            choice = validator.getValidMenuChoice(1, 6);

            if (choice == 6) {
                cout << "Logging out..." << endl;
                break;
            }

            switch (choice) {

            case 1:
                sys.TodaysAppointments(doctorId);
                break;

            case 2:
                sys.MarkAppointmentComplete(doctorId);
                break;

            case 3:
                sys.AppointmentCompleted(doctorId);
                break;

            case 4:
                sys.writePrescription(doctorId);
                break;

            case 5:
                sys.PatientsMedicalHistrory(doctorId);
                break;
            }
        }
    }
