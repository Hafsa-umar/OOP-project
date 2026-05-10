//
#include "FileHandling.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void FileHandling::loadPatients(Patient patients[], int& patientCount) {
    ifstream fin("patients.txt");
    patientCount = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, password, balance;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, password, '|');
        getline(ss, balance, '|');
        patients[patientCount].setID(stoi(id));
        patients[patientCount].setname(name);
        patients[patientCount].setpassword(password);
        patients[patientCount].setBalance(stof(balance));
        patientCount++;
    }
    fin.close();
}

void FileHandling::loadDoctors(Doctor doctors[], int& doctorCount) {
    ifstream fin("doctors.txt");
    doctorCount = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, password, specialization, fee, contact;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, password, '|');
        getline(ss, specialization, '|');
        getline(ss, fee, '|');
        getline(ss, contact, '|');
        doctors[doctorCount].setID(stoi(id));
        doctors[doctorCount].setname(name);
        doctors[doctorCount].setpassword(password);
        doctors[doctorCount].setspecialization(specialization);
        doctors[doctorCount].setfee(stof(fee));
        doctors[doctorCount].setContact(contact);
        doctorCount++;
    }
    fin.close();
}

void FileHandling::loadAppointments(Appointment appointments[], int& appointmentCount) {
    ifstream fin("appointments.txt");
    appointmentCount = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, patientId, doctorId, date, timeSlot, status;
        getline(ss, id, '|');
        getline(ss, patientId, '|');
        getline(ss, doctorId, '|');
        getline(ss, date, '|');
        getline(ss, timeSlot, '|');
        getline(ss, status, '|');
        appointments[appointmentCount].setAppointmentId(stoi(id));
        appointments[appointmentCount].setpatientid(stoi(patientId));
        appointments[appointmentCount].setdoctorID(stoi(doctorId));
        appointments[appointmentCount].setDate(date);
        appointments[appointmentCount].setTime(timeSlot);
        appointments[appointmentCount].setstatus(status);
        appointmentCount++;
    }
    fin.close();
}

void FileHandling::loadBills(Bill bills[], int& billCount) {
    ifstream fin("bills.txt");
    billCount = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, patientId, appointmentId, amount, status, date;
        getline(ss, id, '|');
        getline(ss, patientId, '|');
        getline(ss, appointmentId, '|');
        getline(ss, amount, '|');
        getline(ss, status, '|');
        getline(ss, date, '|');
        bills[billCount].setbillid(stoi(id));
        bills[billCount].setpatientid(stoi(patientId));
        bills[billCount].setAppointmentId(stoi(appointmentId));
        bills[billCount].SetAmount(stof(amount));
        bills[billCount].setStatus(status);
        bills[billCount].setdate(date);
        billCount++;
    }
    fin.close();
}

void FileHandling::loadPrescriptions(Prescription prescriptions[], int& prescriptionCount) {
    ifstream fin("prescriptions.txt");
    prescriptionCount = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, appointmentId, patientId, doctorId, date, medicines, notes;
        getline(ss, id, '|');
        getline(ss, appointmentId, '|');
        getline(ss, patientId, '|');
        getline(ss, doctorId, '|');
        getline(ss, date, '|');
        getline(ss, medicines, '|');
        getline(ss, notes, '|');
        prescriptions[prescriptionCount].setPrescriptionId(stoi(id));
        prescriptions[prescriptionCount].setAppointmentId(stoi(appointmentId));
        prescriptions[prescriptionCount].setPatientId(stoi(patientId));
        prescriptions[prescriptionCount].setDoctorId(stoi(doctorId));
        prescriptions[prescriptionCount].setDate(date);
        prescriptions[prescriptionCount].setMedicines(medicines);
        prescriptions[prescriptionCount].setNotes(notes);
        prescriptionCount++;
    }
    fin.close();
}

///////////////////////////////////////////////////////

void FileHandling::addAppointment(const Appointment& appointment) {
    ofstream fout("appointments.txt", ios::app);
    fout << appointment.getAppointmentId() << "|"
        << appointment.getpatientId() << "|"
        << appointment.getDoctorId() << "|"
        << appointment.getDate() << "|"
        << appointment.getTime() << "|"
        << appointment.getstatus()
        << endl;
    fout.close();
}

void FileHandling::addBill(const Bill& bill) {
    ofstream fout("bills.txt", ios::app);
    fout << bill.getbillId() << "|"
        << bill.getpatientId() << "|"
        << bill.getAppointmentId() << "|"
        << bill.getAmount() << "|"
        << bill.getStatus() << "|"
        << bill.GetDate()
        << endl;
    fout.close();
}

void FileHandling::addPrescription(const Prescription& prescriptions) {
    ofstream fout("prescriptions.txt", ios::app);
    fout << prescriptions.getPrescriptionid() << "|"
        << prescriptions.getAppointmentId() << "|"
        << prescriptions.getPatientId() << "|"
        << prescriptions.getDoctorId() << "|"
        << prescriptions.getDate() << "|"
        << prescriptions.getMedicines() << "|"
        << prescriptions.getNotes()
        << endl;
    fout.close();
}

void FileHandling::addDoctor(const Doctor& doctor) {
    ofstream fout("doctors.txt", ios::app);
    fout << doctor.getId() << "|"
        << doctor.getName() << "|"
        << doctor.getpassword() << "|"
        << doctor.getSpecialization() << "|"
        << doctor.getFee() << "|"
        << doctor.getContact()
        << endl;
    fout.close();
}

void FileHandling::addPatient(const Patient& patient) {
    ofstream fout("patients.txt", ios::app);
    fout << patient.getId() << "|"
        << patient.getName() << "|"
        << patient.getpassword() << "|"
        << patient.getBalance()
        << endl;
    fout.close();
}

void FileHandling::addDischargedPatient(const Patient& patient) {
    ofstream fout("discharged.txt", ios::app);
    fout << patient.getId() << "|"
        << patient.getName() << "|"
        << patient.getBalance()
        << endl;
    fout.close();
}

/////////////////////////////////////////////////////////

void FileHandling::updateAppointments(Appointment appointments[], int appointmentCount) {
    ofstream fout("appointments.txt");
    for (int i = 0; i < appointmentCount; i++) {
        fout << appointments[i].getAppointmentId() << "|"
            << appointments[i].getpatientId() << "|"
            << appointments[i].getDoctorId() << "|"
            << appointments[i].getDate() << "|"
            << appointments[i].getTime() << "|"
            << appointments[i].getstatus()
            << endl;
    }
    fout.close();
}

void FileHandling::updateBills(Bill bills[], int billCount) {
    ofstream fout("bills.txt");
    for (int i = 0; i < billCount; i++) {
        fout << bills[i].getbillId() << "|"
            << bills[i].getpatientId() << "|"
            << bills[i].getAppointmentId() << "|"
            << bills[i].getAmount() << "|"
            << bills[i].getStatus() << "|"
            << bills[i].GetDate()
            << endl;
    }
    fout.close();
}

void FileHandling::updatePatients(Patient patients[], int patientCount) {
    ofstream fout("patients.txt");
    for (int i = 0; i < patientCount; i++) {
        fout << patients[i].getId() << "|"
            << patients[i].getName() << "|"
            << patients[i].getpassword() << "|"
            << patients[i].getBalance()
            << endl;
    }
    fout.close();
}

void FileHandling::updatePrescriptions(Prescription prescriptions[], int prescriptionCount) {
    ofstream fout("prescriptions.txt");
    for (int i = 0; i < prescriptionCount; i++) {
        fout << prescriptions[i].getPrescriptionid() << "|"
            << prescriptions[i].getAppointmentId() << "|"
            << prescriptions[i].getPatientId() << "|"
            << prescriptions[i].getDoctorId() << "|"
            << prescriptions[i].getDate() << "|"
            << prescriptions[i].getMedicines() << "|"
            << prescriptions[i].getNotes()
            << endl;
    }
    fout.close();
}

void FileHandling::updateDoctors(Doctor doctors[], int doctorCount) {
    ofstream fout("doctors.txt");
    for (int i = 0; i < doctorCount; i++) {
        fout << doctors[i].getId() << "|"
            << doctors[i].getName() << "|"
            << doctors[i].getpassword() << "|"
            << doctors[i].getSpecialization() << "|"
            << doctors[i].getFee() << "|"
            << doctors[i].getContact()
            << endl;
    }
    fout.close();
}