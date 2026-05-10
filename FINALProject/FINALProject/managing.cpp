#include "managing.h"
#include<iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;



Manager::Manager(){
    patientCount=0;
    doctorCount=0;
    appointmentCount=0;
    prescriptionCount=0;
    billCount=0;

    filehandler.loadPatients(patients, patientCount);
    cout << "loading every data" << endl;
    filehandler.loadDoctors(doctors, doctorCount);
    filehandler.loadAppointments(appointments, appointmentCount);
    filehandler.loadBills(bills, billCount);
    filehandler.loadPrescriptions(prescriptions, prescriptionCount);
 }
string Manager::toLowerString(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}


void Manager::showDoctorsBySpecialization(string spec) {
    string input = toLowerString(spec);
    bool found = false;

    for (int i = 0; i < doctorCount; i++) {
        if (toLowerString(doctors[i].getSpecialization()) == input) {
            cout << "ID: " << doctors[i].getId()
                << " Name: " << doctors[i].getName()
                << " Fee: " << doctors[i].getFee() << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No doctors available for that specialization\n";
}

bool Manager::isSlotAvailable(int doctorId, string date, string time) {
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].getDoctorId() == doctorId &&
            appointments[i].getDate() == date &&
            appointments[i].getTime() == time &&
            appointments[i].getstatus() != "cancelled") {
            return false;
        }
    }
    return true;
}
int Manager::generateAppointmentId() {
    int maxId = 0;

    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].getAppointmentId() > maxId)
            maxId = appointments[i].getAppointmentId();
    }

    return 
        maxId + 1;
}

double Manager::getDoctorFeeByAppointment(int doctorId) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].getId() == doctorId)
            return doctors[i].getFee();
    }
    return 0;
}

int Manager::findPatientById(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].getId() == id) {
            return i;
        }
    }
    return -1;
}
int Manager::findDoctorById(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

int Manager::findAppointmentById(int id) {
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].getAppointmentId() == id) {
            return i;
        }
    }
    return -1;
}
void Manager::sortAppointmentsAsc() {
    for (int i = 0; i < appointmentCount - 1; i++) {
        for (int j = i + 1; j < appointmentCount; j++) {

            if (appointments[i].getDate() > appointments[j].getDate()) {
                Appointment temp = appointments[i];
                appointments[i] = appointments[j];
                appointments[j] = temp;
            }
        }
    }
}
bool Manager::hasPendingAppointments(int DoctorId) {

    for (int i = 0; i < appointmentCount; i++) {

        if (appointments[i].getDoctorId() == DoctorId &&
            appointments[i].getstatus() == "pending") {

            return true;
        }
    }

    return false;
}
bool Manager::hasPendingAppointmentsForPatient(int patientId) {

    for (int i = 0; i < appointmentCount; i++) {

        if (appointments[i].getpatientId() == patientId &&
            appointments[i].getstatus() == "pending") {

            return true;
        }
    }

    return false;
}
bool Manager::hasUnpaidBills(int patientId) {

    for (int i = 0; i < billCount; i++) {

        if (bills[i].getpatientId() == patientId &&
            bills[i].getStatus() == "unpaid") {

            return true;
        }
    }

    return false;
}

string Manager::getTodayDate() {

    time_t now = time(0);
    tm local;
    localtime_s(&local, &now);

    string day;
    string month;
    string year;

    if (local.tm_mday < 10) {
        day = "0" + to_string(local.tm_mday);
    }
    else {

        day = to_string(local.tm_mday);
    }
    if ((local.tm_mon + 1) < 10) {
        month = "0" + to_string(local.tm_mon + 1);
    }
    else
        month = to_string(local.tm_mon + 1);

    year = to_string(local.tm_year + 1900);
    return day + "-" + month + "-" + year;
}


//////////////////////////////////////////////////////////////////////////////////////////

void Manager::bookAppointment(int patientId) {

    string specialization, date, time;
    int doctorId;
    cout << "Enter specialization: ";
    cin >> specialization;

    showDoctorsBySpecialization(specialization);
    cout << "Enter Doctor ID: ";
    cin >> doctorId;

    int dIndex = findDoctorById(doctorId);

    if (dIndex == -1) {
        cout << "Doctor not found\n";
        return;
    }

    int attempts = 0;

    while (attempts < 3) {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;

        if (Validator::validateDate(date))
            break;

        cout << "Invalid date\n";
        attempts++;
    }

    if (attempts == 3) {
        cout << "attempts finished!"<<endl;
    }
        return;

    
    cout << "Available slots: ";
    string slots[8] = { "09:00","10:00","11:00","12:00",
                        "13:00","14:00","15:00","16:00" };

    for (int i = 0; i < 8; i++) {
        if (isSlotAvailable(doctorId, date, slots[i]))
            cout << slots[i] << " ";
    }
    cout << endl;

    
    cout << "Enter time slot: ";
    cin >> time;

    try {
        if (!Validator::validateTimeslot(time))
            throw SlotUnavailableException("Invalid time slot");

        if (!isSlotAvailable(doctorId, date, time))
            throw SlotUnavailableException("Time slot already taken");

    }
    catch (HospitalException& e) {
        cout << e.getMessage() << endl;
        return;
    }

   
    int pIndex = findPatientById(patientId);
    if (pIndex == -1) {
        cout << "Patient not found\n";
        return;
    }

    double fee = doctors[dIndex].getFee();

    if (patients[pIndex].getBalance() < fee) {
        cout << "Insufficient funds\n";
        return;
    }
    patients[pIndex] -= fee;

    int appId = generateAppointmentId();

    Appointment a;
    a.setAppointmentId(appId);
    a.setpatientid(patientId);
    a.setdoctorID(doctorId);
    a.setDate(date);
    a.setTime(time);
    a.setstatus("pending");

    appointments[appointmentCount++] = a;

    cout << "Appointment booked successfully. ID: " << appId << endl;

    
}




void Manager::CancelAppointment(int patientId) {
    bool found = false;

    cout << "Pending Appointments:\n";

    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].getpatientId() == patientId &&
            appointments[i].getstatus() == "pending") {

            int docId = appointments[i].getDoctorId();
            string docName = doctors[findDoctorById(docId)].getName();

            cout << "ID: " << appointments[i].getAppointmentId()
                 << " Doctor: " << docName
                 << " Date: " << appointments[i].getDate()
                 << " Time: " << appointments[i].getTime()
                 << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "You have no pending appointments.\n";
        return;
    }
    int appId;
    cout << "Enter Appointment ID to cancel: ";
    cin >> appId;

    int index = findAppointmentById(appId);

    if (index == -1 ||
        appointments[index].getpatientId() != patientId ||
        appointments[index].getstatus() != "pending") {

        cout << "Invalid appointment ID.\n";
        return;
    }
    int docId = appointments[index].getDoctorId();
    double fee = getDoctorFeeByAppointment(docId);

    
    appointments[index].setstatus("cancelled");
    int pIndex = findPatientById(patientId);
    patients[pIndex] += fee;

    cout << "Appointment cancelled. PKR "
        << fee << " refunded to your balance.\n";
}





void Manager::ViewAppointment(int patientId) {

    bool found = false;

    sortAppointmentsAsc();

    for (int i = 0; i < appointmentCount; i++) {

        if (appointments[i].getpatientId() == patientId) {

            int docId = appointments[i].getDoctorId();
            int dIndex = findDoctorById(docId);

            cout << "ID: " << appointments[i].getAppointmentId()
                << " Doctor: " << doctors[dIndex].getName()
                << " Specialization: " << doctors[dIndex].getSpecialization()
                << " Date: " << appointments[i].getDate()
                << " Time: " << appointments[i].getTime()
                << " Status: " << appointments[i].getstatus()
                << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "No appointments found\n";
    }
}


void Manager::ViewMedicalReports(int patientId) {

    bool found = false;
    for (int i = 0; i < prescriptionCount - 1; i++) {
        for (int j = i + 1; j < prescriptionCount; j++) {

            if (prescriptions[i].getPatientId() == patientId &&
                prescriptions[j].getPatientId() == patientId) {

               
                string d1 = prescriptions[i].getDate();
                string d2 = prescriptions[j].getDate();

                string c1 = d1.substr(6, 4) + d1.substr(3, 2) + d1.substr(0, 2);
                string c2 = d2.substr(6, 4) + d2.substr(3, 2) + d2.substr(0, 2);

               
                if (c1 < c2) {
                    Prescription temp = prescriptions[i];
                    prescriptions[i] = prescriptions[j];
                    prescriptions[j] = temp;
                }
            }
        }
    }
    for (int i = 0; i < prescriptionCount; i++) {

        if (prescriptions[i].getPatientId() == patientId) {

            if (!Validator::validateDate(prescriptions[i].getDate()))
                continue;

            int docId = prescriptions[i].getDoctorId();
            int dIndex = findDoctorById(docId);

            cout << "Date: " << prescriptions[i].getDate()
                << " | Doctor: " << doctors[dIndex].getName()
                << " | Medicines: " << prescriptions[i].getMedicines()
                << " | Notes: " << prescriptions[i].getNotes()
                << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "No medical records found\n";
    }
}


void Manager::ViewBill(int patientId) {

    bool found = false;
    double totalUnpaid = 0;

    for (int i = 0; i < billCount; i++) {

        if (bills[i].getpatientId() == patientId) {

            cout << "Bill ID: " << bills[i].getbillId()
                << " | Appointment ID: " << bills[i].getAppointmentId()
                << " | Amount: " << bills[i].getAmount()
                << " | Status: " << bills[i].getStatus()
                << " | Date: " << bills[i].GetDate()
                << endl;
            if (bills[i].getStatus() == "unpaid") {
                totalUnpaid += bills[i].getAmount();
            }

            found = true;
        }
    }

    if (!found) {
        cout << "No bills found\n";
        return;
    }

    cout << "Total Outstanding Unpaid Amount: PKR " << totalUnpaid << endl;
}



void Manager::Paybill(int patientId) {

    bool found = false;
    for (int i = 0; i < billCount; i++) {

        if (bills[i].getpatientId() == patientId &&
            bills[i].getStatus() == "unpaid") {

            cout << "Bill ID: " << bills[i].getbillId()
                << " | Amount: " << bills[i].getAmount()
                << " | Date: " << bills[i].GetDate()
                << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "No unpaid bills.\n";
        return;
    }

    int billId;
    cout << "Enter Bill ID to pay: ";
    cin >> billId;

    int bIndex = -1;
    for (int i = 0; i < billCount; i++) {

        if (bills[i].getbillId() == billId &&
            bills[i].getpatientId() == patientId &&
            bills[i].getStatus() == "unpaid") {

            bIndex = i;
            break;
        }
    }

    if (bIndex == -1) {
        cout << "Invalid Bill ID."<<endl;
        return;
    }

    int pIndex = findPatientById(patientId);

    double amount = bills[bIndex].getAmount();

    try {

        if (patients[pIndex].getBalance() < amount) {
            throw InsufficientFundsException("Insufficient balance.");
        }

       
        patients[pIndex] -= amount;
        bills[bIndex].setStatus("paid");

        
        filehandler.updatePatients(patients,patientCount);
        filehandler.updateBills(bills,billCount);

        cout << "Bill paid successfully."<<endl;
        cout << "Remaining balance: PKR "
            << patients[pIndex].getBalance()
            << endl;
    }

    catch (InsufficientFundsException& e) {
        cout << e.message << endl;
    }
}


void Manager::topupBalance(int patientId) {

    int attempts = 0;

    while (attempts < 3) {

        float amount;

        cout << "Enter amount to add (PKR): ";
        cin >> amount;

        try {
            if (amount <= 0) {
                throw InvalidInputException(
                    "Amount must be greater than 0."
                );
            }

            int pIndex = findPatientById(patientId);
            patients[pIndex] += amount;
            filehandler.updateBills(bills, billCount);
            

            cout << "Balance updated. New balance: PKR "
                << patients[pIndex].getBalance()
                << endl;

            return;
        }

        catch (InvalidInputException& e) {

            cout << e.message << endl;

            attempts++;
        }
    }
}
/////////////////////////////////////////////////////////////

void Manager::TodaysAppointments(int doctorId) {

    filehandler.loadAppointments(appointments, appointmentCount);
    
    time_t now = time(0);
    tm t;
    localtime_s(&t, &now);
    string today;

    if (t.tm_mday < 10) {

        today += "0" + to_string(t.tm_mday);
    }
    else {
        today += to_string(t.tm_mday);
    }
    today += "-";

    if (t.tm_mon + 1 < 10) {
        today += "0" + to_string(t.tm_mon + 1);
    }
    else {
        today += to_string(t.tm_mon + 1);
    }
    today += "-";
    today += to_string(t.tm_year + 1900);
    bool found = false;
    

    for (int i = 0; i < appointmentCount; i++) {
        

        if (appointments[i].getDoctorId() == doctorId &&
            appointments[i].getDate() == today) {

            int pIndex =findPatientById(appointments[i].getpatientId());
                cout << appointments[i].getAppointmentId()<< " | "<< patients[pIndex].getName()<< " | "
                << appointments[i].getTime()<< " | "<< appointments[i].getstatus()
                << endl;
            found = true;
            
        }
    }

    if (!found) {
        cout << "No appointments scheduled for today."<<endl;
    }
}

void Manager::MarkAppointmentComplete(int doctorId) {
    filehandler.loadAppointments(appointments, appointmentCount);
   
    int id;
    cout << "Enter Appointment ID: ";
    cin >> id;

    int Aindex = findAppointmentById(id);
    
    if (Aindex == -1) {
        cout << "Appointment not found.\n";
        return;
    }

    if (appointments[Aindex].getDoctorId() != doctorId) {
        cout << "This appointment does not belong to you.\n";
        return;
    }

    if (appointments[Aindex].getstatus() != "pending") {
        cout << "Only pending appointments can be completed.\n";
        return;
    }
    appointments[Aindex].setstatus("completed");
    filehandler.updateAppointments(appointments, appointmentCount);
    cout << "Appointment marked as completed.\n";
}

void Manager::AppointmentCompleted(int doctorId) {
    int id;
    cout << "Enter Appointment ID: ";
    cin >> id;

    int AIndex = findAppointmentById(id);

    if (AIndex == -1 ||
        appointments[AIndex].getDoctorId() != doctorId ||
        appointments[AIndex].getstatus() != "pending") {

        cout << "Invalid appointment.\n";
        return;
    }

    appointments[AIndex].setstatus("no-show");
    for (int i = 0; i < billCount; i++) {
        if (bills[i].getAppointmentId() == id) {
            bills[i].setStatus("cancelled");
        }
    }

    filehandler.updateAppointments(appointments, appointmentCount);
    filehandler.updateBills(bills, billCount);

    cout << "Appointment marked as no-show."<<endl;
}

void Manager::writePrescription(int doctorId) {

    int id;
    cout << "Enter Appointment ID: ";
    cin >> id;

    int aIndex = findAppointmentById(id);

    if (aIndex == -1 ||
        appointments[aIndex].getDoctorId() != doctorId ||
        appointments[aIndex].getstatus() != "completed") {

        cout << "Invalid appointment.\n";
        return;
    }
    for (int i = 0; i < prescriptionCount; i++) {

        if (prescriptions[i].getAppointmentId() == id) {
            cout << "Prescription already written for this appointment."<<endl;
            return;
        }
    }
   cin.ignore();
   string medicines;
   string notes;

    cout << "Enter medicines: ";
    getline(cin,medicines);

    cout << "enter Notes: ";
    getline(cin, notes);

    Prescription p;

    p.setPrescriptionId(prescriptionCount + 1);
    p.setAppointmentId(id);
    p.setPatientId(appointments[aIndex].getpatientId());
    p.setDoctorId(doctorId);
    p.setDate(appointments[aIndex].getDate());
    p.setMedicines(medicines);
    p.setNotes(notes);

    prescriptions[prescriptionCount++] = p;
    filehandler.addPrescription(p);
    cout << "Prescription added successfully. "<<endl;
}

void Manager::PatientsMedicalHistrory(int doctorId) {

    int patientId;

    cout << "Enter Patient ID: ";
    cin >> patientId;
    bool hasAccess = false;

    for (int i = 0; i < appointmentCount; i++) {

        if (appointments[i].getDoctorId() == doctorId &&
            appointments[i].getpatientId() == patientId &&
            appointments[i].getstatus() == "completed") {

            hasAccess = true;
            break;
        }
    }

    if (!hasAccess) {
        cout << "Access denied. You can only view records of your own patients."<<endl;
        return;
    }
    for (int i = 0; i < prescriptionCount - 1; i++) {
        for (int j = i + 1; j < prescriptionCount; j++) {

            if (prescriptions[i].getDate() < prescriptions[j].getDate()) {

                Prescription temp = prescriptions[i];
                prescriptions[i] = prescriptions[j];
                prescriptions[j] = temp;
            }
        }
    }
    bool found = false;

    for (int i = 0; i < prescriptionCount; i++) {

        if (prescriptions[i].getPatientId() == patientId &&
            prescriptions[i].getDoctorId() == doctorId) {

                cout << "Date: " << prescriptions[i].getDate()
                << " | Medicines: " << prescriptions[i].getMedicines()
                << " | Notes: " << prescriptions[i].getNotes()
                << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "No medical history found."<<endl;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void Manager::addDoctor() {
    Doctor d;
    int id;
    string name, specialization, contact, password;
    float fee;
    

    cout << "Enter ID" << endl;
    cin >> id;
    if (!Validator::validateId(id)) {
        cout << "Enter valid ID" << endl;
        return;
    }
    cin.ignore();
    cout << "Enter name : ";
    getline(cin, name);

    if (name.length() > 50) {
        cout << "Name too long."<<endl;
        return;
    }

    cout << "Enter specialization : ";
    getline(cin, specialization);

    if (specialization.length() > 50) {
        cout << "Specialization too long."<<endl;
        return;
    }
    
    cout << "Enter contact (11 digits): ";
    getline(cin, contact);
    
    if (!Validator::validateContactNumber(contact)) {
        cout << "Invalid contact number."<<endl;
        return;
    }

    cout << "Enter password : ";
    cout << "Requirements for the Password:" <<
        "minimum 8 characters with Upper and lower both" << endl <<
        "Numbers are Also required" << endl <<
        "not completeing the requirement will cause invalidation" << endl;
    getline(cin, password);

    if (!Validator::validatePassword(password)) {
        cout << "Invalid password."<<endl;
        return;
    }

    cout << "Enter consultation fee: ";
    cin >> fee;

    if (fee <= 0) {
        cout << "Invalid fee."<<endl;
        return;
    }
    id = doctorCount + 1;
    d.setID(id);
    d.setname(name);
    d.setspecialization(specialization);
    d.setpassword(password);
    d.setfee(fee);
    d.setContact(contact);

    doctors[doctorCount++] = d;
    cout << doctorCount;
    cout << endl;
    filehandler.addDoctor(d);

    cout << "Doctor added successfully. ID: " << id << endl;
}

void Manager::removedoctor() {

    int id;
    cout << endl<<"Doctors List:"<<endl;
    for (int i = 0; i < doctorCount; i++) {

        cout << doctors[i].getId() << " | "
            << doctors[i].getName() << " | "
            << doctors[i].getSpecialization() << " | "
            << doctors[i].getFee() << endl;
    }

    cout << endl<<"Enter Doctor ID to remove: "<<endl;

    cin >> id;

    int index = findDoctorById(id);

    if (index == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    if (hasPendingAppointments(id)) {
        cout << "Cannot remove doctor with pending appointments."<<endl;
        return;
    }
    for (int i = index; i < doctorCount - 1; i++) {
        doctors[i] = doctors[i + 1];
    }
   doctorCount--;
   filehandler.updateDoctors(doctors, doctorCount);

    cout << "Doctor removed successfully.\n";
}
void Manager::dischargePatient() {

    int id;
    cout << "Enter Patient ID: ";
    cin >> id;

    int index = findPatientById(id);

    if (index == -1) {
        cout << "Patient not found."<<endl;
        return;
    }
    if (hasUnpaidBills(id)) {
        cout << "Cannot discharge patient with unpaid bills.\n";
        return;
    }
    if (hasPendingAppointmentsForPatient(id)) {
        cout << "Cannot discharge patient with pending appointments.\n";
        return;
    }
    Patient p;
    filehandler.addDischargedPatient(p);
    

    patientCount--;

    filehandler.updatePatients(patients, patientCount);
    filehandler.updateAppointments(appointments, appointmentCount);
    filehandler.updateBills(bills, billCount);
    filehandler.updatePrescriptions(prescriptions, prescriptionCount);

    cout << "Patient discharged successfully."<<endl;
}

void Manager::viewAllpatients() {

    if (patientCount == 0) {
        cout << "No patients found.\n";
        return;
    }

    for (int i = 0; i < patientCount; i++) {

        int unpaidCount = 0;

        for (int j = 0; j < billCount; j++) {
            if (bills[j].getpatientId() == patients[i].getId() &&
                bills[j].getStatus() == "unpaid") {
                unpaidCount++;
            }
        }

        cout << patients[i].getId() << " | "
            << patients[i].getName() << " | "
            << patients[i].getage() << " | "
            << patients[i].getGender() << " | "
            << patients[i].getContact() << " | "
            << patients[i].getBalance() << " | "
            << unpaidCount << endl;
    }
}

void Manager::ViewAllDoctors() {

    if (doctorCount == 0) {
        cout << "No doctors found.\n";
        return;
    }

    for (int i = 0; i < doctorCount; i++) {

        cout << doctors[i].getId() << " | "
            << doctors[i].getName() << " | "
            << doctors[i].getSpecialization() << " | "
            << doctors[i].getContact() << " | "
            << doctors[i].getFee()
            << endl;
    }
}

void Manager :: ViewAllAppointments() {

    if (appointmentCount == 0) {
        cout << "No appointments found."<<endl;
        return;
    }
    for (int i = 0; i < appointmentCount - 1; i++) {
        for (int j = i + 1; j < appointmentCount; j++) {

            if (appointments[i].getDate() < appointments[j].getDate()) {

                Appointment temp = appointments[i];
                appointments[i] = appointments[j];
                appointments[j] = temp;
            }
        }
    }
    for (int i = 0; i < appointmentCount; i++) {

        int Pid = appointments[i].getpatientId();
        int Did = appointments[i].getDoctorId();

        string patientName = "Unknown";
        string doctorName = "Unknown";

        int pIndex = findPatientById(Pid);

        if (pIndex != -1) {
            patientName = patients[pIndex].getName();
        }

        int dIndex = findDoctorById(Did);

        if (dIndex != -1) {
            doctorName = doctors[dIndex].getName();
        }

         cout << appointments[i].getAppointmentId() << " | "
            << patientName << " | "
            << doctorName << " | "
            << appointments[i].getDate() << " | "
            << appointments[i].getTime() << " | "
            << appointments[i].getstatus()
            << endl;
    }
}

void Manager::viewUnpaidBills() {

    bool found = false;

    for (int i = 0; i < billCount; i++) {

        if (bills[i].getStatus() == "unpaid") {

            found = true;

            int pIndex =
                findPatientById(bills[i].getpatientId());

            cout << bills[i].getbillId() << " | "
                << patients[pIndex].getName() << " | "
                << bills[i].getAmount() << " | "
                << bills[i].GetDate()
                << endl;
        }
    }

    if (!found) {
        cout << "No unpaid bills found."<<endl;
    }
}

void Manager::viewSecurityLog() {

    ifstream fin("security_log.txt");

    string line;

    bool empty = true;

    while (getline(fin, line)) {

        cout << line << endl;
        empty = false;
    }

    if (empty) {
        cout << "No security events logged."<<endl;
    }

    fin.close();
}

void Manager::generateDailyReport() {

    string today = getTodayDate();

    int total = 0;
    int pending = 0;
    int completed = 0;
    int noShow = 0;

    float revenue = 0;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].getDate() == today) {
            total++;
            if (appointments[i].getstatus() == "pending")
                pending++;
            else if (appointments[i].getstatus() == "completed")
                completed++;
            else if (appointments[i].getstatus() == "no-show")
                noShow++;
        }
    }
    for (int i = 0; i < billCount; i++) {

        if (bills[i].getStatus() == "paid" &&
            bills[i].GetDate() == today) {

            revenue += bills[i].getAmount();
        }
    }

    cout <<endl<< "DAILY REPORT"<<endl;

    cout << "Total appointments today: "
        << total << endl;
    cout << "Pending: "
        << pending << endl;
    cout << "Completed: "
        << completed << endl;
    cout << "No-show: "
        << noShow << endl;
    cout << "Revenue collected today: PKR "
        << revenue << endl;
}
