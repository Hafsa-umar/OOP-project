#pragma once
 
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "Validator.h"
#include "exception.h"
#include "Prescription.h"
#include "bills.h";
#include "FileHandling.h"

class Manager {
private:
    Patient patients[100];
    Doctor doctors[100];
    Appointment appointments[100];
    Prescription prescriptions[100];
    Bill bills[100];
    FileHandling filehandler;

    int patientCount;
    int doctorCount;
    int appointmentCount;
    int prescriptionCount;
    int billCount;
    
    

public:
   
    Manager();
    
    int findPatientById(int id);
    int findDoctorById(int id);
    double getDoctorFeeByAppointment(int doctorId); 
    int findAppointmentById(int id); 
    void sortAppointmentsAsc();
    int generateAppointmentId();
    bool isSlotAvailable(int doctorId, string date, string time);
    void showDoctorsBySpecialization(string spec);
    void sortRecordsDesc();
    string toLowerString(string s);

    
    void bookAppointment(int patientId);
    void CancelAppointment(int patientId);
    void ViewAppointment(int patientId);
    void ViewMedicalReports(int patientId);
    void ViewBill(int patientId);
    void Paybill(int patientId);
    void topupBalance(int patientId);
    void Logout();
   


    void TodaysAppointments(int doctorId);
    void MarkAppointmentComplete(int doctorId);
    void AppointmentCompleted(int doctorId);
    void writePrescription(int doctorId);
    void PatientsMedicalHistrory(int doctorId);
    void LOogout();

    
    
    bool hasPendingAppointments(int Id);
    bool hasUnpaidBills(int patientId);
    bool hasPendingAppointmentsForPatient(int patientId);
    std::string getTodayDate();

    void addDoctor();
    void removedoctor();
    void admitPatient(int id);
    void viewAllpatients();
    void ViewAllDoctors();
    void ViewAllAppointments();
    void viewUnpaidBills();
    void dischargePatient();
    void viewSecurityLog();
    void generateDailyReport();
    void logout();

   
};

