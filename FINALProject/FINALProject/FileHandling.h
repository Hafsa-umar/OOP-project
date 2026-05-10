
#pragma once

#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "bills.h"
#include "Prescription.h"

class FileHandling {

public:

    
    void loadPatients(Patient patients[], int& patientCount);
    void loadDoctors(Doctor doctors[], int& doctorCount);
    void loadAppointments(Appointment appointments[],int& appointmentCount);
    void loadBills(Bill bills[], int& billCount);
    void loadPrescriptions(Prescription prescriptions[],int& prescriptionCount);

    
    void updatePatients(Patient patients[],int patientCount);
    void updateDoctors(Doctor doctors[], int doctorCount);
    void updateAppointments(Appointment appointments[],int appointmentCount);
    void updateBills(Bill bills[],int billCount);
    void updatePrescriptions(Prescription prescriptions[],int prescriptionCount);

   void addAppointment(const Appointment& appointment);
   void addDischargedPatient(const Patient& patient);
   void addBill(const Bill& bill);
   void addPrescription(const Prescription& prescriptions);
   void addDoctor(const Doctor& doctor);
   void addPatient(const Patient& patient);
};