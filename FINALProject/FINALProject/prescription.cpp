#include "prescription.h"

// ================= Constructors =================

Prescription::Prescription() {
    Prescriptionid = 0;
    appointmentId = 0;
    patientId = 0;
    doctorId = 0;
    date = "";
    medicines = "";
    notes = "";
}

Prescription::Prescription(int Id, int ai, int pi, int di,
    const std::string& d,
    const std::string& med,
    const std::string& n) {
    Prescriptionid = Id;
    appointmentId = ai;
    patientId = pi;
    doctorId = di;
    date = d;
    medicines = med;
    notes = n;
}

// ================= Getters =================

int Prescription::getPrescriptionid() const {
    return Prescriptionid;
}

int Prescription::getAppointmentId() const {
    return appointmentId;
}

int Prescription::getPatientId() const {
    return patientId;
}

int Prescription::getDoctorId() const {
    return doctorId;
}

std::string Prescription::getDate() const {
    return date;
}

std::string Prescription::getMedicines() const {
    return medicines;
}

std::string Prescription::getNotes() const {
    return notes;
}

// ================= Setters =================

void Prescription::setPrescriptionId(int Id) {
    Prescriptionid = Id;
}

void Prescription::setAppointmentId(int ai) {
    appointmentId = ai;
}

void Prescription::setPatientId(int pi) {
    patientId = pi;
}

void Prescription::setDoctorId(int di) {
    doctorId = di;
}

void Prescription::setDate(const std::string& d) {
    date = d;
}

void Prescription::setMedicines(const std::string& med) {
    medicines = med;
}

void Prescription::setNotes(const std::string& n) {
    notes = n;
}