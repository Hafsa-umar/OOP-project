#pragma once
#include <string>
class Appointment {
private:
    int Appointmentid;
    int patientId;
    int doctorId;
    std::string date;
    std::string time;
    std::string status;

public:
    Appointment();

    int getAppointmentId() const;
    int getDoctorId() const;
    int getpatientId()const;
    const std::string getDate() const;
    const std::string getTime() const;
    const std::string getstatus() const;

    void setDate(const std::string d);
    void setTime(const std::string t);
    void setpatientid(int pi);
    void setdoctorID(int di);
    void setAppointmentId(int Ad);
    void setstatus(const std::string s);

    bool operator==(const Appointment& other) const;
   
};
