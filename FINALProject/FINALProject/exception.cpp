#include "exception.h"
#include <string>

HospitalException::HospitalException(const std::string& msg){
       message = msg;
}
std::string HospitalException::getMessage() const {
    return message;
}

FileNotFoundException::FileNotFoundException(const std::string& msg): HospitalException(msg) {
}

InsufficientFundsException::InsufficientFundsException(const std::string& msg)
    : HospitalException(msg) {
}

InvalidInputException::InvalidInputException(const std::string& msg)
    : HospitalException(msg) {
}

SlotUnavailableException::SlotUnavailableException(const std::string& msg)
    : HospitalException(msg) {
}