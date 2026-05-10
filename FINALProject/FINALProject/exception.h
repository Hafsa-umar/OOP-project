#pragma once
#include <string>

class HospitalException {
public:
    std::string message;

    HospitalException(const std::string& msg);
    std::string getMessage() const;
        
};

// ---------------- Derived Exceptions ----------------

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const std::string& msg);
};

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException(const std::string& msg);
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const std::string& msg);
};

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException(const std::string& msg);
};

