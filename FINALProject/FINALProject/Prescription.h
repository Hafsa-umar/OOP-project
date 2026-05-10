#pragma once
#include <string>

class Prescription {
private:
    int Prescriptionid;
    int appointmentId;
    int patientId;
    int doctorId;
    std::string date;
    std::string medicines;
    std::string notes;

public:
    // Constructors
    Prescription();
    Prescription(int Id, int ai, int pi, int di,
        const std::string& d,
        const std::string& med,
        const std::string& n);

    // Getters
    int getPrescriptionid() const;
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    std::string getDate() const;
    std::string getMedicines() const;
    std::string getNotes() const;

    // Setters
    void setPrescriptionId(int Id);
    void setAppointmentId(int ai);
    void setPatientId(int pi);
    void setDoctorId(int di);
    void setDate(const std::string& d);
    void setMedicines(const std::string& med);
    void setNotes(const std::string& n);
};