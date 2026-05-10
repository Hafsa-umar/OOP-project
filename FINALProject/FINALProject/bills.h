#pragma once
#include <string>
class Bill {
private:
    int billid;
    int patientId;
    int AppointmentId;
    float amount;
    std::string status;
    std::string date;

public:
    Bill();
    Bill(int ID, int PI, int Ai, float a, std::string s, std::string d);

    int getbillId() const;
    int getpatientId()const;
    float getAmount() const;
    int getAppointmentId()const;
    const std::string getStatus() const;
    const std::string GetDate()const;
    
    void setdate(const std::string d);
    void setpatientid(int pi);
    void setStatus(const std::string s);
    void setbillid(int Id);
    void SetAmount(float a);
    void setAppointmentId(int id);
};
