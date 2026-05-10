#include "SFMLGuiLayer.h"
#include "Validator.h"
#include "managing.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>
#include <sstream>
#include <iostream>

const unsigned int WIDTH = 1100;
const unsigned int HEIGHT = 760;

// Color scheme
const sf::Color WHITE_BG(255, 255, 255);
const sf::Color MINT_LIGHT(229, 247, 241);
const sf::Color MINT_MEDIUM(158, 223, 204);
const sf::Color SKY_BLUE(106, 169, 217);
const sf::Color DEEP_BLUE(43, 93, 135);
const sf::Color TEXT_DARK(27, 27, 27);
const sf::Color DANGER(190, 65, 65);
const sf::Color DANGER_HOVER(220, 80, 80);

enum Screen
{
    USER_TYPE_SCREEN,

    ADMIN_LOGIN_SCREEN,
    DOCTOR_LOGIN_SCREEN,
    PATIENT_LOGIN_SCREEN,

    ADMIN_SCREEN,
    DOCTOR_SCREEN,
    PATIENT_SCREEN,

    OUTPUT_SCREEN,
    FORM_SCREEN
};

enum Action
{
    ACTION_NONE,

    ACTION_ADD_DOCTOR,
    ACTION_REMOVE_DOCTOR,
    ACTION_VIEW_ALL_PATIENTS,
    ACTION_VIEW_ALL_DOCTORS,
    ACTION_VIEW_ALL_APPOINTMENTS,
    ACTION_VIEW_UNPAID_BILLS,
    ACTION_DISCHARGE_PATIENT,
    ACTION_VIEW_SECURITY_LOG,
    ACTION_GENERATE_DAILY_REPORT,

    ACTION_DOCTOR_TODAY_APPOINTMENTS,
    ACTION_MARK_COMPLETE,
    ACTION_MARK_NOSHOW,
    ACTION_WRITE_PRESCRIPTION,
    ACTION_PATIENT_HISTORY,

    ACTION_BOOK_APPOINTMENT,
    ACTION_CANCEL_APPOINTMENT,
    ACTION_VIEW_MY_APPOINTMENTS,
    ACTION_VIEW_MY_RECORDS,
    ACTION_VIEW_MY_BILLS,
    ACTION_PAY_BILL,
    ACTION_TOPUP_BALANCE
};

void centerText(sf::Text& text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
        });

    text.setPosition({ x, y });
}

bool isLoginScreen(Screen screen)
{
    return screen == ADMIN_LOGIN_SCREEN ||
        screen == DOCTOR_LOGIN_SCREEN ||
        screen == PATIENT_LOGIN_SCREEN;
}

Screen getPanelFromLogin(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return ADMIN_SCREEN;
    if (screen == DOCTOR_LOGIN_SCREEN) return DOCTOR_SCREEN;
    if (screen == PATIENT_LOGIN_SCREEN) return PATIENT_SCREEN;

    return USER_TYPE_SCREEN;
}

std::string getRoleFromLogin(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return "admin";
    if (screen == DOCTOR_LOGIN_SCREEN) return "doctor";
    if (screen == PATIENT_LOGIN_SCREEN) return "patient";

    return "";
}

int stringToInt(const std::string& text)
{
    int number = 0;

    for (int i = 0; i < static_cast<int>(text.length()); i++)
    {
        number = number * 10 + (text[i] - '0');
    }

    return number;
}

sf::FloatRect getUserBlockBounds(int index, float windowW, float windowH)
{
    float blockW = 230.0f;
    float blockH = 155.0f;
    float gap = 25.0f;

    float totalW = blockW * 4.0f + gap * 3.0f;
    float startX = windowW / 2.0f - totalW / 2.0f;
    float y = windowH / 2.0f - 55.0f;
    float x = startX + index * (blockW + gap);

    return sf::FloatRect({ x, y }, { blockW, blockH });
}

sf::FloatRect getMenuButtonBounds(int index, int total, float windowW)
{
    float buttonW = 360.0f;
    float buttonH = 50.0f;
    float gapX = 35.0f;
    float gapY = 18.0f;

    int columns = 2;

    float totalW = buttonW * 2.0f + gapX;
    float startX = windowW / 2.0f - totalW / 2.0f;
    float startY = 245.0f;

    int col = index % columns;
    int row = index / columns;

    float x = startX + col * (buttonW + gapX);
    float y = startY + row * (buttonH + gapY);

    return sf::FloatRect({ x, y }, { buttonW, buttonH });
}

sf::FloatRect getLoginIdBox(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f - 180.0f, 295.0f }, { 360.0f, 48.0f });
}

sf::FloatRect getLoginPasswordBox(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f - 180.0f, 385.0f }, { 360.0f, 48.0f });
}

sf::FloatRect getLoginButton(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f - 180.0f, 465.0f }, { 170.0f, 50.0f });
}

sf::FloatRect getBackButton(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f + 10.0f, 465.0f }, { 170.0f, 50.0f });
}

sf::FloatRect getOutputBackButton(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f - 100.0f, 665.0f }, { 200.0f, 50.0f });
}

sf::FloatRect getFormInputBox(int index, float windowW)
{
    float x = windowW / 2.0f - 190.0f;
    float y = 210.0f + index * 58.0f;

    return sf::FloatRect({ x, y }, { 380.0f, 38.0f });
}

sf::FloatRect getFormSubmitButton(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f - 190.0f, 600.0f }, { 180.0f, 50.0f });
}

sf::FloatRect getFormCancelButton(float windowW)
{
    return sf::FloatRect({ windowW / 2.0f + 10.0f, 600.0f }, { 180.0f, 50.0f });
}

bool mouseInside(sf::RenderWindow& window, const sf::FloatRect& rect)
{
    sf::Vector2i pixelMouse = sf::Mouse::getPosition(window);
    sf::Vector2f worldMouse = window.mapPixelToCoords(pixelMouse);

    return rect.contains(worldMouse);
}

const char* getUserBlockText(int index)
{
    if (index == 0) return "ADMIN";
    if (index == 1) return "DOCTOR";
    if (index == 2) return "PATIENT";
    if (index == 3) return "EXIT";

    return "";
}

int getClickedUserBlock(sf::RenderWindow& window)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);
    float windowH = static_cast<float>(size.y);

    for (int i = 0; i < 4; i++)
    {
        if (mouseInside(window, getUserBlockBounds(i, windowW, windowH)))
        {
            return i;
        }
    }

    return -1;
}

int getMenuCount(Screen screen)
{
    if (screen == ADMIN_SCREEN) return 10;
    if (screen == DOCTOR_SCREEN) return 6;
    if (screen == PATIENT_SCREEN) return 8;

    return 0;
}

const char* getMenuText(Screen screen, int index)
{
    if (screen == ADMIN_SCREEN)
    {
        if (index == 0) return "1. Add Doctor";
        if (index == 1) return "2. Remove Doctor";
        if (index == 2) return "3. View All Patients";
        if (index == 3) return "4. View All Doctors";
        if (index == 4) return "5. View All Appointments";
        if (index == 5) return "6. View Unpaid Bills";
        if (index == 6) return "7. Discharge Patient";
        if (index == 7) return "8. View Security Log";
        if (index == 8) return "9. Generate Daily Report";
        if (index == 9) return "10. Logout";
    }

    if (screen == DOCTOR_SCREEN)
    {
        if (index == 0) return "1. View Today's Appointments";
        if (index == 1) return "2. Mark Appointment Complete";
        if (index == 2) return "3. Mark Appointment No-Show";
        if (index == 3) return "4. Write Prescription";
        if (index == 4) return "5. View Patient Medical History";
        if (index == 5) return "6. Logout";
    }

    if (screen == PATIENT_SCREEN)
    {
        if (index == 0) return "1. Book Appointment";
        if (index == 1) return "2. Cancel Appointment";
        if (index == 2) return "3. View My Appointments";
        if (index == 3) return "4. View My Medical Records";
        if (index == 4) return "5. View My Bills";
        if (index == 5) return "6. Pay Bill";
        if (index == 6) return "7. Top Up Balance";
        if (index == 7) return "8. Logout";
    }

    return "";
}

const char* getPanelTitle(Screen screen)
{
    if (screen == ADMIN_SCREEN) return "ADMIN PANEL";
    if (screen == DOCTOR_SCREEN) return "DOCTOR PANEL";
    if (screen == PATIENT_SCREEN) return "PATIENT PANEL";

    return "USER TYPE";
}

const char* getWelcomeText(Screen screen)
{
    if (screen == ADMIN_SCREEN)
    {
        return "Admin Menu";
    }

    if (screen == DOCTOR_SCREEN)
    {
        return "Welcome, Dr. [Name] | Specialization: [spec]";
    }

    if (screen == PATIENT_SCREEN)
    {
        return "Welcome, [Patient Name] | Balance: PKR [balance]";
    }

    return "";
}

int getClickedMenuButton(sf::RenderWindow& window, Screen screen)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);
    int count = getMenuCount(screen);

    for (int i = 0; i < count; i++)
    {
        if (mouseInside(window, getMenuButtonBounds(i, count, windowW)))
        {
            return i;
        }
    }

    return -1;
}

Action getActionFromMenu(Screen screen, int clicked)
{
    if (screen == ADMIN_SCREEN)
    {
        if (clicked == 0) return ACTION_ADD_DOCTOR;
        if (clicked == 1) return ACTION_REMOVE_DOCTOR;
        if (clicked == 2) return ACTION_VIEW_ALL_PATIENTS;
        if (clicked == 3) return ACTION_VIEW_ALL_DOCTORS;
        if (clicked == 4) return ACTION_VIEW_ALL_APPOINTMENTS;
        if (clicked == 5) return ACTION_VIEW_UNPAID_BILLS;
        if (clicked == 6) return ACTION_DISCHARGE_PATIENT;
        if (clicked == 7) return ACTION_VIEW_SECURITY_LOG;
        if (clicked == 8) return ACTION_GENERATE_DAILY_REPORT;
    }

    if (screen == DOCTOR_SCREEN)
    {
        if (clicked == 0) return ACTION_DOCTOR_TODAY_APPOINTMENTS;
        if (clicked == 1) return ACTION_MARK_COMPLETE;
        if (clicked == 2) return ACTION_MARK_NOSHOW;
        if (clicked == 3) return ACTION_WRITE_PRESCRIPTION;
        if (clicked == 4) return ACTION_PATIENT_HISTORY;
    }

    if (screen == PATIENT_SCREEN)
    {
        if (clicked == 0) return ACTION_BOOK_APPOINTMENT;
        if (clicked == 1) return ACTION_CANCEL_APPOINTMENT;
        if (clicked == 2) return ACTION_VIEW_MY_APPOINTMENTS;
        if (clicked == 3) return ACTION_VIEW_MY_RECORDS;
        if (clicked == 4) return ACTION_VIEW_MY_BILLS;
        if (clicked == 5) return ACTION_PAY_BILL;
        if (clicked == 6) return ACTION_TOPUP_BALANCE;
    }

    return ACTION_NONE;
}

bool actionNeedsForm(Action action)
{
    if (action == ACTION_ADD_DOCTOR) return true;
    if (action == ACTION_REMOVE_DOCTOR) return true;
    if (action == ACTION_DISCHARGE_PATIENT) return true;
    if (action == ACTION_MARK_COMPLETE) return true;
    if (action == ACTION_MARK_NOSHOW) return true;
    if (action == ACTION_WRITE_PRESCRIPTION) return true;
    if (action == ACTION_PATIENT_HISTORY) return true;
    if (action == ACTION_BOOK_APPOINTMENT) return true;
    if (action == ACTION_CANCEL_APPOINTMENT) return true;
    if (action == ACTION_PAY_BILL) return true;
    if (action == ACTION_TOPUP_BALANCE) return true;

    return false;
}

std::string captureManagerOutput(Manager& manager, void (Manager::* func)())
{
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    (manager.*func)();

    std::cout.rdbuf(oldCout);

    std::string result = output.str();

    if (result.length() == 0)
    {
        result = "Operation completed.";
    }

    return result;
}

std::string captureManagerOutputWithId(Manager& manager, void (Manager::* func)(int), int id)
{
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    (manager.*func)(id);

    std::cout.rdbuf(oldCout);

    std::string result = output.str();

    if (result.length() == 0)
    {
        result = "Operation completed.";
    }

    return result;
}

std::string runManagerInputNoArg(Manager& manager, void (Manager::* func)(), const std::string& inputData)
{
    std::istringstream input(inputData);
    std::ostringstream output;

    std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    (manager.*func)();

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    std::string result = output.str();

    if (result.length() == 0)
    {
        result = "Operation completed.";
    }

    return result;
}

std::string runManagerInputWithId(Manager& manager, void (Manager::* func)(int), int userId, const std::string& inputData)
{
    std::istringstream input(inputData);
    std::ostringstream output;

    std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    (manager.*func)(userId);

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    std::string result = output.str();

    if (result.length() == 0)
    {
        result = "Operation completed.";
    }

    return result;
}

std::string executeDirectAction(Manager& manager, Action action, int loggedInId)
{
    if (action == ACTION_VIEW_ALL_PATIENTS) return captureManagerOutput(manager, &Manager::viewAllpatients);
    if (action == ACTION_VIEW_ALL_DOCTORS) return captureManagerOutput(manager, &Manager::ViewAllDoctors);
    if (action == ACTION_VIEW_ALL_APPOINTMENTS) return captureManagerOutput(manager, &Manager::ViewAllAppointments);
    if (action == ACTION_VIEW_UNPAID_BILLS) return captureManagerOutput(manager, &Manager::viewUnpaidBills);
    if (action == ACTION_VIEW_SECURITY_LOG) return captureManagerOutput(manager, &Manager::viewSecurityLog);
    if (action == ACTION_GENERATE_DAILY_REPORT) return captureManagerOutput(manager, &Manager::generateDailyReport);

    if (action == ACTION_DOCTOR_TODAY_APPOINTMENTS) return captureManagerOutputWithId(manager, &Manager::TodaysAppointments, loggedInId);

    if (action == ACTION_VIEW_MY_APPOINTMENTS) return captureManagerOutputWithId(manager, &Manager::ViewAppointment, loggedInId);
    if (action == ACTION_VIEW_MY_RECORDS) return captureManagerOutputWithId(manager, &Manager::ViewMedicalReports, loggedInId);
    if (action == ACTION_VIEW_MY_BILLS) return captureManagerOutputWithId(manager, &Manager::ViewBill, loggedInId);

    return "Option selected.";
}

std::string executeFormAction(Manager& manager, Action action, int loggedInId, std::string values[])
{
    std::string inputData = "";

    if (action == ACTION_ADD_DOCTOR)
    {
        inputData =
            values[0] + "\n" +
            values[1] + "\n" +
            values[2] + "\n" +
            values[3] + "\n" +
            values[4] + "\n" +
            values[5] + "\n";

        return runManagerInputNoArg(manager, &Manager::addDoctor, inputData);
    }

    if (action == ACTION_REMOVE_DOCTOR)
    {
        inputData = values[0] + "\n";
        return runManagerInputNoArg(manager, &Manager::removedoctor, inputData);
    }

    if (action == ACTION_DISCHARGE_PATIENT)
    {
        inputData = values[0] + "\n";
        return runManagerInputNoArg(manager, &Manager::dischargePatient, inputData);
    }

    if (action == ACTION_MARK_COMPLETE)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::MarkAppointmentComplete, loggedInId, inputData);
    }

    if (action == ACTION_MARK_NOSHOW)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::AppointmentCompleted, loggedInId, inputData);
    }

    if (action == ACTION_WRITE_PRESCRIPTION)
    {
        inputData =
            values[0] + "\n" +
            values[1] + "\n" +
            values[2] + "\n";

        return runManagerInputWithId(manager, &Manager::writePrescription, loggedInId, inputData);
    }

    if (action == ACTION_PATIENT_HISTORY)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::PatientsMedicalHistrory, loggedInId, inputData);
    }

    if (action == ACTION_BOOK_APPOINTMENT)
    {
        inputData =
            values[0] + "\n" +
            values[1] + "\n" +
            values[2] + "\n" +
            values[3] + "\n";

        return runManagerInputWithId(manager, &Manager::bookAppointment, loggedInId, inputData);
    }

    if (action == ACTION_CANCEL_APPOINTMENT)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::CancelAppointment, loggedInId, inputData);
    }

    if (action == ACTION_PAY_BILL)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::Paybill, loggedInId, inputData);
    }

    if (action == ACTION_TOPUP_BALANCE)
    {
        inputData = values[0] + "\n";
        return runManagerInputWithId(manager, &Manager::topupBalance, loggedInId, inputData);
    }

    return "Action not handled.";
}

void setupForm(Action action, std::string& formTitle, std::string labels[], std::string values[], int& fieldCount, int& activeField)
{
    for (int i = 0; i < 6; i++)
    {
        labels[i] = "";
        values[i] = "";
    }

    fieldCount = 0;
    activeField = 0;

    if (action == ACTION_ADD_DOCTOR)
    {
        formTitle = "ADD DOCTOR";
        fieldCount = 6;
        labels[0] = "Temporary ID";
        labels[1] = "Name";
        labels[2] = "Specialization";
        labels[3] = "Contact";
        labels[4] = "Password";
        labels[5] = "Fee";
        values[0] = "1";
        return;
    }

    if (action == ACTION_REMOVE_DOCTOR)
    {
        formTitle = "REMOVE DOCTOR";
        fieldCount = 1;
        labels[0] = "Doctor ID";
        return;
    }

    if (action == ACTION_DISCHARGE_PATIENT)
    {
        formTitle = "DISCHARGE PATIENT";
        fieldCount = 1;
        labels[0] = "Patient ID";
        return;
    }

    if (action == ACTION_MARK_COMPLETE)
    {
        formTitle = "MARK APPOINTMENT COMPLETE";
        fieldCount = 1;
        labels[0] = "Appointment ID";
        return;
    }

    if (action == ACTION_MARK_NOSHOW)
    {
        formTitle = "MARK APPOINTMENT NO-SHOW";
        fieldCount = 1;
        labels[0] = "Appointment ID";
        return;
    }

    if (action == ACTION_WRITE_PRESCRIPTION)
    {
        formTitle = "WRITE PRESCRIPTION";
        fieldCount = 3;
        labels[0] = "Appointment ID";
        labels[1] = "Medicines";
        labels[2] = "Notes";
        return;
    }

    if (action == ACTION_PATIENT_HISTORY)
    {
        formTitle = "PATIENT MEDICAL HISTORY";
        fieldCount = 1;
        labels[0] = "Patient ID";
        return;
    }

    if (action == ACTION_BOOK_APPOINTMENT)
    {
        formTitle = "BOOK APPOINTMENT";
        fieldCount = 4;
        labels[0] = "Specialization";
        labels[1] = "Doctor ID";
        labels[2] = "Date DD-MM-YYYY";
        labels[3] = "Time HH:MM";
        return;
    }

    if (action == ACTION_CANCEL_APPOINTMENT)
    {
        formTitle = "CANCEL APPOINTMENT";
        fieldCount = 1;
        labels[0] = "Appointment ID";
        return;
    }

    if (action == ACTION_PAY_BILL)
    {
        formTitle = "PAY BILL";
        fieldCount = 1;
        labels[0] = "Bill ID";
        return;
    }

    if (action == ACTION_TOPUP_BALANCE)
    {
        formTitle = "TOP UP BALANCE";
        fieldCount = 1;
        labels[0] = "Amount";
        return;
    }

    formTitle = "FORM";
}

bool formIsComplete(std::string values[], int fieldCount)
{
    for (int i = 0; i < fieldCount; i++)
    {
        if (values[i].length() == 0)
        {
            return false;
        }
    }

    return true;
}

void attemptLogin(
    Screen& currentScreen,
    std::string& idInput,
    std::string& passwordInput,
    int& activeBox,
    std::string& message,
    int& loggedInId
)
{
    if (idInput.length() == 0 || passwordInput.length() == 0)
    {
        message = "Please enter both ID and password.";
        return;
    }

    int id = stringToInt(idInput);
    std::string role = getRoleFromLogin(currentScreen);

    if (Validator::login(role, id, passwordInput))
    {
        loggedInId = id;
        currentScreen = getPanelFromLogin(currentScreen);
        idInput = "";
        passwordInput = "";
        activeBox = 0;
        message = "Login successful.";
    }
    else
    {
        message = "Invalid ID or password.";
    }
}

std::string maskPassword(const std::string& password)
{
    std::string masked = "";

    for (int i = 0; i < static_cast<int>(password.length()); i++)
    {
        masked += "*";
    }

    return masked;
}

const char* getLoginTitle(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return "ADMIN LOGIN";
    if (screen == DOCTOR_LOGIN_SCREEN) return "DOCTOR LOGIN";
    if (screen == PATIENT_LOGIN_SCREEN) return "PATIENT LOGIN";

    return "LOGIN";
}

void drawTopBar(sf::RenderWindow& window, sf::Font& font, float windowW)
{
    sf::RectangleShape topBar({ windowW, 92.0f });
    topBar.setPosition({ 0.0f, 0.0f });
    topBar.setFillColor(DEEP_BLUE);

    sf::Text title(font, "HEALING HORIZON HOSPITAL", 34);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 46.0f);

    window.draw(topBar);
    window.draw(title);
}

void drawUserTypeScreen(sf::RenderWindow& window, sf::Font& font, const std::string& message)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);
    float windowH = static_cast<float>(size.y);

    sf::Text title(font, "USER TYPE", 42);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 150.0f);
    window.draw(title);

    sf::Text subtitle(font, "Select your role to enter the system", 20);
    subtitle.setFillColor(TEXT_DARK);
    centerText(subtitle, windowW / 2.0f, 195.0f);
    window.draw(subtitle);

    for (int i = 0; i < 4; i++)
    {
        sf::FloatRect rect = getUserBlockBounds(i, windowW, windowH);
        bool hover = mouseInside(window, rect);

        sf::RectangleShape block({ rect.size.x, rect.size.y });
        block.setPosition(rect.position);
        block.setOutlineThickness(3.0f);

        if (i == 3)
        {
            block.setFillColor(hover ? DANGER_HOVER : DANGER);
            block.setOutlineColor(DANGER);
        }
        else
        {
            block.setFillColor(hover ? SKY_BLUE : MINT_LIGHT);
            block.setOutlineColor(DEEP_BLUE);
        }

        sf::Text blockText(font, getUserBlockText(i), 27);
        blockText.setStyle(sf::Text::Bold);

        if (i == 3)
        {
            blockText.setFillColor(sf::Color::White);
        }
        else
        {
            blockText.setFillColor(DEEP_BLUE);
        }

        centerText(
            blockText,
            rect.position.x + rect.size.x / 2.0f,
            rect.position.y + rect.size.y / 2.0f
        );

        window.draw(block);
        window.draw(blockText);
    }

    sf::Text msg(font, message, 18);
    msg.setFillColor(TEXT_DARK);
    centerText(msg, windowW / 2.0f, windowH - 55.0f);
    window.draw(msg);
}

void drawLoginScreen(
    sf::RenderWindow& window,
    sf::Font& font,
    Screen screen,
    const std::string& idInput,
    const std::string& passwordInput,
    int activeBox,
    const std::string& message
)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);

    sf::RectangleShape panel({ 520.0f, 440.0f });
    panel.setPosition({ windowW / 2.0f - 260.0f, 160.0f });
    panel.setFillColor(MINT_LIGHT);
    panel.setOutlineThickness(3.0f);
    panel.setOutlineColor(DEEP_BLUE);
    window.draw(panel);

    sf::Text title(font, getLoginTitle(screen), 36);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 205.0f);
    window.draw(title);

    sf::Text subtitle(font, "Enter your ID and password", 18);
    subtitle.setFillColor(TEXT_DARK);
    centerText(subtitle, windowW / 2.0f, 242.0f);
    window.draw(subtitle);

    sf::Text idLabel(font, "ID", 18);
    idLabel.setFillColor(DEEP_BLUE);
    idLabel.setStyle(sf::Text::Bold);
    idLabel.setPosition({ windowW / 2.0f - 180.0f, 268.0f });
    window.draw(idLabel);

    sf::Text passLabel(font, "Password", 18);
    passLabel.setFillColor(DEEP_BLUE);
    passLabel.setStyle(sf::Text::Bold);
    passLabel.setPosition({ windowW / 2.0f - 180.0f, 358.0f });
    window.draw(passLabel);

    sf::FloatRect idRect = getLoginIdBox(windowW);
    sf::FloatRect passRect = getLoginPasswordBox(windowW);
    sf::FloatRect loginRect = getLoginButton(windowW);
    sf::FloatRect backRect = getBackButton(windowW);

    sf::RectangleShape idBox({ idRect.size.x, idRect.size.y });
    idBox.setPosition(idRect.position);
    idBox.setFillColor(WHITE_BG);
    idBox.setOutlineThickness(2.5f);
    idBox.setOutlineColor(activeBox == 1 ? SKY_BLUE : MINT_MEDIUM);
    window.draw(idBox);

    sf::RectangleShape passBox({ passRect.size.x, passRect.size.y });
    passBox.setPosition(passRect.position);
    passBox.setFillColor(WHITE_BG);
    passBox.setOutlineThickness(2.5f);
    passBox.setOutlineColor(activeBox == 2 ? SKY_BLUE : MINT_MEDIUM);
    window.draw(passBox);

    sf::Text idText(font, idInput, 20);
    idText.setFillColor(TEXT_DARK);
    idText.setPosition({ idRect.position.x + 12.0f, idRect.position.y + 11.0f });
    window.draw(idText);

    sf::Text passText(font, maskPassword(passwordInput), 20);
    passText.setFillColor(TEXT_DARK);
    passText.setPosition({ passRect.position.x + 12.0f, passRect.position.y + 11.0f });
    window.draw(passText);

    bool loginHover = mouseInside(window, loginRect);
    bool backHover = mouseInside(window, backRect);

    sf::RectangleShape loginButton({ loginRect.size.x, loginRect.size.y });
    loginButton.setPosition(loginRect.position);
    loginButton.setFillColor(loginHover ? SKY_BLUE : DEEP_BLUE);
    loginButton.setOutlineThickness(2.0f);
    loginButton.setOutlineColor(DEEP_BLUE);
    window.draw(loginButton);

    sf::RectangleShape backButton({ backRect.size.x, backRect.size.y });
    backButton.setPosition(backRect.position);
    backButton.setFillColor(backHover ? SKY_BLUE : WHITE_BG);
    backButton.setOutlineThickness(2.0f);
    backButton.setOutlineColor(DEEP_BLUE);
    window.draw(backButton);

    sf::Text loginText(font, "LOGIN", 19);
    loginText.setFillColor(sf::Color::White);
    loginText.setStyle(sf::Text::Bold);
    centerText(loginText, loginRect.position.x + loginRect.size.x / 2.0f, loginRect.position.y + loginRect.size.y / 2.0f);
    window.draw(loginText);

    sf::Text backText(font, "BACK", 19);
    backText.setFillColor(DEEP_BLUE);
    backText.setStyle(sf::Text::Bold);
    centerText(backText, backRect.position.x + backRect.size.x / 2.0f, backRect.position.y + backRect.size.y / 2.0f);
    window.draw(backText);

    sf::Text msg(font, message, 16);
    msg.setFillColor(TEXT_DARK);
    centerText(msg, windowW / 2.0f, 560.0f);
    window.draw(msg);
}

void drawRoleMenu(sf::RenderWindow& window, sf::Font& font, Screen screen, const std::string& message)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);

    sf::RectangleShape panel({ windowW - 140.0f, 560.0f });
    panel.setPosition({ 70.0f, 130.0f });
    panel.setFillColor(MINT_LIGHT);
    panel.setOutlineThickness(3.0f);
    panel.setOutlineColor(DEEP_BLUE);
    window.draw(panel);

    sf::Text title(font, getPanelTitle(screen), 36);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 165.0f);
    window.draw(title);

    sf::Text welcome(font, getWelcomeText(screen), 20);
    welcome.setFillColor(TEXT_DARK);
    centerText(welcome, windowW / 2.0f, 205.0f);
    window.draw(welcome);

    int count = getMenuCount(screen);

    for (int i = 0; i < count; i++)
    {
        sf::FloatRect rect = getMenuButtonBounds(i, count, windowW);
        bool hover = mouseInside(window, rect);

        bool isLogout = false;

        if (screen == ADMIN_SCREEN && i == 9) isLogout = true;
        if (screen == DOCTOR_SCREEN && i == 5) isLogout = true;
        if (screen == PATIENT_SCREEN && i == 7) isLogout = true;

        sf::RectangleShape button({ rect.size.x, rect.size.y });
        button.setPosition(rect.position);
        button.setOutlineThickness(2.0f);

        if (isLogout)
        {
            button.setFillColor(hover ? DANGER_HOVER : DANGER);
            button.setOutlineColor(DANGER);
        }
        else
        {
            button.setFillColor(hover ? SKY_BLUE : WHITE_BG);
            button.setOutlineColor(DEEP_BLUE);
        }

        sf::Text buttonText(font, getMenuText(screen, i), 18);
        buttonText.setStyle(sf::Text::Bold);

        if (isLogout)
        {
            buttonText.setFillColor(sf::Color::White);
        }
        else
        {
            buttonText.setFillColor(DEEP_BLUE);
        }

        centerText(buttonText, rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f);

        window.draw(button);
        window.draw(buttonText);
    }

    sf::RectangleShape messageBox({ windowW - 220.0f, 54.0f });
    messageBox.setPosition({ 110.0f, 640.0f });
    messageBox.setFillColor(WHITE_BG);
    messageBox.setOutlineThickness(2.0f);
    messageBox.setOutlineColor(DEEP_BLUE);
    window.draw(messageBox);

    sf::Text msg(font, message, 16);
    msg.setFillColor(TEXT_DARK);
    centerText(msg, windowW / 2.0f, 667.0f);
    window.draw(msg);
}

void drawOutputScreen(sf::RenderWindow& window, sf::Font& font, const std::string& outputText)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);

    sf::RectangleShape panel({ windowW - 140.0f, 520.0f });
    panel.setPosition({ 70.0f, 130.0f });
    panel.setFillColor(MINT_LIGHT);
    panel.setOutlineThickness(3.0f);
    panel.setOutlineColor(DEEP_BLUE);
    window.draw(panel);

    sf::Text title(font, "OUTPUT", 34);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 165.0f);
    window.draw(title);

    sf::Text output(font, outputText, 16);
    output.setFillColor(TEXT_DARK);
    output.setPosition({ 105.0f, 215.0f });
    window.draw(output);

    sf::FloatRect backRect = getOutputBackButton(windowW);
    bool hover = mouseInside(window, backRect);

    sf::RectangleShape backButton({ backRect.size.x, backRect.size.y });
    backButton.setPosition(backRect.position);
    backButton.setFillColor(hover ? SKY_BLUE : DEEP_BLUE);
    backButton.setOutlineThickness(2.0f);
    backButton.setOutlineColor(DEEP_BLUE);
    window.draw(backButton);

    sf::Text backText(font, "BACK", 20);
    backText.setFillColor(sf::Color::White);
    backText.setStyle(sf::Text::Bold);
    centerText(backText, backRect.position.x + backRect.size.x / 2.0f, backRect.position.y + backRect.size.y / 2.0f);
    window.draw(backText);
}

void drawFormScreen(
    sf::RenderWindow& window,
    sf::Font& font,
    const std::string& formTitle,
    std::string labels[],
    std::string values[],
    int fieldCount,
    int activeField,
    const std::string& message
)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);

    sf::RectangleShape panel({ 560.0f, 560.0f });
    panel.setPosition({ windowW / 2.0f - 280.0f, 125.0f });
    panel.setFillColor(MINT_LIGHT);
    panel.setOutlineThickness(3.0f);
    panel.setOutlineColor(DEEP_BLUE);
    window.draw(panel);

    sf::Text title(font, formTitle, 30);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 160.0f);
    window.draw(title);

    for (int i = 0; i < fieldCount; i++)
    {
        sf::FloatRect rect = getFormInputBox(i, windowW);

        sf::Text label(font, labels[i], 16);
        label.setFillColor(DEEP_BLUE);
        label.setStyle(sf::Text::Bold);
        label.setPosition({ rect.position.x, rect.position.y - 23.0f });
        window.draw(label);

        sf::RectangleShape inputBox({ rect.size.x, rect.size.y });
        inputBox.setPosition(rect.position);
        inputBox.setFillColor(WHITE_BG);
        inputBox.setOutlineThickness(2.0f);
        inputBox.setOutlineColor(activeField == i ? SKY_BLUE : MINT_MEDIUM);
        window.draw(inputBox);

        sf::Text valueText(font, values[i], 18);
        valueText.setFillColor(TEXT_DARK);
        valueText.setPosition({ rect.position.x + 10.0f, rect.position.y + 8.0f });
        window.draw(valueText);
    }

    sf::FloatRect submitRect = getFormSubmitButton(windowW);
    sf::FloatRect cancelRect = getFormCancelButton(windowW);

    bool submitHover = mouseInside(window, submitRect);
    bool cancelHover = mouseInside(window, cancelRect);

    sf::RectangleShape submitButton({ submitRect.size.x, submitRect.size.y });
    submitButton.setPosition(submitRect.position);
    submitButton.setFillColor(submitHover ? SKY_BLUE : DEEP_BLUE);
    submitButton.setOutlineThickness(2.0f);
    submitButton.setOutlineColor(DEEP_BLUE);
    window.draw(submitButton);

    sf::Text submitText(font, "SUBMIT", 19);
    submitText.setFillColor(sf::Color::White);
    submitText.setStyle(sf::Text::Bold);
    centerText(submitText, submitRect.position.x + submitRect.size.x / 2.0f, submitRect.position.y + submitRect.size.y / 2.0f);
    window.draw(submitText);

    sf::RectangleShape cancelButton({ cancelRect.size.x, cancelRect.size.y });
    cancelButton.setPosition(cancelRect.position);
    cancelButton.setFillColor(cancelHover ? SKY_BLUE : WHITE_BG);
    cancelButton.setOutlineThickness(2.0f);
    cancelButton.setOutlineColor(DEEP_BLUE);
    window.draw(cancelButton);

    sf::Text cancelText(font, "CANCEL", 19);
    cancelText.setFillColor(DEEP_BLUE);
    cancelText.setStyle(sf::Text::Bold);
    centerText(cancelText, cancelRect.position.x + cancelRect.size.x / 2.0f, cancelRect.position.y + cancelRect.size.y / 2.0f);
    window.draw(cancelText);

    sf::Text msg(font, message, 15);
    msg.setFillColor(TEXT_DARK);
    centerText(msg, windowW / 2.0f, 670.0f);
    window.draw(msg);
}

int runSfmlGui()
{
    sf::RenderWindow window(
        sf::VideoMode({ WIDTH, HEIGHT }),
        "Healing Horizon Hospital",
        sf::Style::Default
    );

    window.setFramerateLimit(60);

    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return -1;
    }

    std::optional<sf::Cursor> arrowCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
    std::optional<sf::Cursor> handCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);

    Manager manager;

    Screen currentScreen = USER_TYPE_SCREEN;
    Screen previousScreen = USER_TYPE_SCREEN;

    Action currentAction = ACTION_NONE;

    std::string message = "Click Admin, Doctor, Patient, or Exit.";
    std::string outputText = "";

    std::string idInput = "";
    std::string passwordInput = "";
    int activeBox = 0;
    int loggedInId = 0;

    std::string formTitle = "";
    std::string formLabels[6];
    std::string formValues[6];
    int formFieldCount = 0;
    int activeFormField = 0;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const sf::Event::Resized* resized = event->getIf<sf::Event::Resized>())
            {
                sf::View view(
                    sf::FloatRect(
                        { 0.0f, 0.0f },
                        {
                            static_cast<float>(resized->size.x),
                            static_cast<float>(resized->size.y)
                        }
                    )
                );

                window.setView(view);
            }

            if (const sf::Event::KeyPressed* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    if (currentScreen == USER_TYPE_SCREEN)
                    {
                        window.close();
                    }
                    else if (currentScreen == OUTPUT_SCREEN || currentScreen == FORM_SCREEN)
                    {
                        currentScreen = previousScreen;
                        message = "Returned to menu.";
                    }
                    else
                    {
                        currentScreen = USER_TYPE_SCREEN;
                        idInput = "";
                        passwordInput = "";
                        activeBox = 0;
                        message = "Returned to user type screen.";
                    }
                }

                if (isLoginScreen(currentScreen))
                {
                    if (key->code == sf::Keyboard::Key::Backspace)
                    {
                        if (activeBox == 1 && idInput.length() > 0)
                        {
                            idInput.erase(idInput.length() - 1, 1);
                        }
                        else if (activeBox == 2 && passwordInput.length() > 0)
                        {
                            passwordInput.erase(passwordInput.length() - 1, 1);
                        }
                    }

                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        attemptLogin(currentScreen, idInput, passwordInput, activeBox, message, loggedInId);
                    }
                }

                if (currentScreen == FORM_SCREEN)
                {
                    if (key->code == sf::Keyboard::Key::Backspace)
                    {
                        if (activeFormField >= 0 &&
                            activeFormField < formFieldCount &&
                            formValues[activeFormField].length() > 0)
                        {
                            formValues[activeFormField].erase(formValues[activeFormField].length() - 1, 1);
                        }
                    }

                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        if (activeFormField < formFieldCount - 1)
                        {
                            activeFormField++;
                        }
                        else
                        {
                            if (!formIsComplete(formValues, formFieldCount))
                            {
                                message = "Please fill all fields.";
                            }
                            else
                            {
                                outputText = executeFormAction(manager, currentAction, loggedInId, formValues);
                                currentScreen = OUTPUT_SCREEN;
                            }
                        }
                    }
                }
            }

            if (const sf::Event::TextEntered* textEvent = event->getIf<sf::Event::TextEntered>())
            {
                unsigned int code = textEvent->unicode;

                if (code >= 32 && code <= 126)
                {
                    char ch = static_cast<char>(code);

                    if (isLoginScreen(currentScreen))
                    {
                        if (activeBox == 1)
                        {
                            if (ch >= '0' && ch <= '9')
                            {
                                if (idInput.length() < 10)
                                {
                                    idInput += ch;
                                }
                            }
                        }
                        else if (activeBox == 2)
                        {
                            if (passwordInput.length() < 25)
                            {
                                passwordInput += ch;
                            }
                        }
                    }
                    else if (currentScreen == FORM_SCREEN)
                    {
                        if (activeFormField >= 0 && activeFormField < formFieldCount)
                        {
                            if (formValues[activeFormField].length() < 60)
                            {
                                formValues[activeFormField] += ch;
                            }
                        }
                    }
                }
            }

            if (const sf::Event::MouseButtonReleased* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (currentScreen == USER_TYPE_SCREEN)
                    {
                        int clicked = getClickedUserBlock(window);

                        if (clicked == 0)
                        {
                            currentScreen = ADMIN_LOGIN_SCREEN;
                            idInput = "";
                            passwordInput = "";
                            activeBox = 1;
                            message = "Enter admin ID and password.";
                        }
                        else if (clicked == 1)
                        {
                            currentScreen = DOCTOR_LOGIN_SCREEN;
                            idInput = "";
                            passwordInput = "";
                            activeBox = 1;
                            message = "Enter doctor ID and password.";
                        }
                        else if (clicked == 2)
                        {
                            currentScreen = PATIENT_LOGIN_SCREEN;
                            idInput = "";
                            passwordInput = "";
                            activeBox = 1;
                            message = "Enter patient ID and password.";
                        }
                        else if (clicked == 3)
                        {
                            window.close();
                        }
                    }
                    else if (isLoginScreen(currentScreen))
                    {
                        sf::Vector2u size = window.getSize();
                        float windowW = static_cast<float>(size.x);

                        if (mouseInside(window, getLoginIdBox(windowW)))
                        {
                            activeBox = 1;
                        }
                        else if (mouseInside(window, getLoginPasswordBox(windowW)))
                        {
                            activeBox = 2;
                        }
                        else if (mouseInside(window, getLoginButton(windowW)))
                        {
                            attemptLogin(currentScreen, idInput, passwordInput, activeBox, message, loggedInId);
                        }
                        else if (mouseInside(window, getBackButton(windowW)))
                        {
                            currentScreen = USER_TYPE_SCREEN;
                            idInput = "";
                            passwordInput = "";
                            activeBox = 0;
                            message = "Returned to user type screen.";
                        }
                    }
                    else if (currentScreen == OUTPUT_SCREEN)
                    {
                        sf::Vector2u size = window.getSize();
                        float windowW = static_cast<float>(size.x);

                        if (mouseInside(window, getOutputBackButton(windowW)))
                        {
                            currentScreen = previousScreen;
                            message = "Returned to menu.";
                        }
                    }
                    else if (currentScreen == FORM_SCREEN)
                    {
                        sf::Vector2u size = window.getSize();
                        float windowW = static_cast<float>(size.x);

                        for (int i = 0; i < formFieldCount; i++)
                        {
                            if (mouseInside(window, getFormInputBox(i, windowW)))
                            {
                                activeFormField = i;
                            }
                        }

                        if (mouseInside(window, getFormSubmitButton(windowW)))
                        {
                            if (!formIsComplete(formValues, formFieldCount))
                            {
                                message = "Please fill all fields.";
                            }
                            else
                            {
                                outputText = executeFormAction(manager, currentAction, loggedInId, formValues);
                                currentScreen = OUTPUT_SCREEN;
                            }
                        }
                        else if (mouseInside(window, getFormCancelButton(windowW)))
                        {
                            currentScreen = previousScreen;
                            message = "Returned to menu.";
                        }
                    }
                    else
                    {
                        int clicked = getClickedMenuButton(window, currentScreen);

                        if (clicked != -1)
                        {
                            int lastIndex = getMenuCount(currentScreen) - 1;

                            if (clicked == lastIndex)
                            {
                                currentScreen = USER_TYPE_SCREEN;
                                idInput = "";
                                passwordInput = "";
                                activeBox = 0;
                                loggedInId = 0;
                                message = "Logged out successfully.";
                            }
                            else
                            {
                                Action action = getActionFromMenu(currentScreen, clicked);

                                previousScreen = currentScreen;

                                if (actionNeedsForm(action))
                                {
                                    currentAction = action;
                                    setupForm(currentAction, formTitle, formLabels, formValues, formFieldCount, activeFormField);
                                    currentScreen = FORM_SCREEN;
                                    message = "Fill the form and press Submit.";
                                }
                                else
                                {
                                    outputText = executeDirectAction(manager, action, loggedInId);
                                    currentScreen = OUTPUT_SCREEN;
                                }
                            }
                        }
                    }
                }
            }
        }

        bool hovering = false;

        if (currentScreen == USER_TYPE_SCREEN)
        {
            hovering = getClickedUserBlock(window) != -1;
        }
        else if (isLoginScreen(currentScreen))
        {
            sf::Vector2u size = window.getSize();
            float windowW = static_cast<float>(size.x);

            hovering =
                mouseInside(window, getLoginIdBox(windowW)) ||
                mouseInside(window, getLoginPasswordBox(windowW)) ||
                mouseInside(window, getLoginButton(windowW)) ||
                mouseInside(window, getBackButton(windowW));
        }
        else if (currentScreen == OUTPUT_SCREEN)
        {
            sf::Vector2u size = window.getSize();
            float windowW = static_cast<float>(size.x);

            hovering = mouseInside(window, getOutputBackButton(windowW));
        }
        else if (currentScreen == FORM_SCREEN)
        {
            sf::Vector2u size = window.getSize();
            float windowW = static_cast<float>(size.x);

            hovering =
                mouseInside(window, getFormSubmitButton(windowW)) ||
                mouseInside(window, getFormCancelButton(windowW));

            for (int i = 0; i < formFieldCount; i++)
            {
                if (mouseInside(window, getFormInputBox(i, windowW)))
                {
                    hovering = true;
                }
            }
        }
        else
        {
            hovering = getClickedMenuButton(window, currentScreen) != -1;
        }

        if (hovering && handCursor.has_value())
        {
            window.setMouseCursor(handCursor.value());
        }
        else if (arrowCursor.has_value())
        {
            window.setMouseCursor(arrowCursor.value());
        }

        window.clear(MINT_MEDIUM);

        sf::Vector2u size = window.getSize();
        float windowW = static_cast<float>(size.x);

        drawTopBar(window, font, windowW);

        if (currentScreen == USER_TYPE_SCREEN)
        {
            drawUserTypeScreen(window, font, message);
        }
        else if (isLoginScreen(currentScreen))
        {
            drawLoginScreen(window, font, currentScreen, idInput, passwordInput, activeBox, message);
        }
        else if (currentScreen == OUTPUT_SCREEN)
        {
            drawOutputScreen(window, font, outputText);
        }
        else if (currentScreen == FORM_SCREEN)
        {
            drawFormScreen(window, font, formTitle, formLabels, formValues, formFieldCount, activeFormField, message);
        }
        else
        {
            drawRoleMenu(window, font, currentScreen, message);
        }

        window.display();
    }

    return 0;
}