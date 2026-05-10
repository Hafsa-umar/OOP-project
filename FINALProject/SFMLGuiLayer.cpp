#include "SFMLGuiLayer.h"

#include "Validator.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>

const unsigned int WIDTH = 1100;
const unsigned int HEIGHT = 760;

const sf::Color WHITE_BG(255, 255, 255);        // #FFFFFF
const sf::Color MINT_LIGHT(229, 247, 241);      // #E5F7F1
const sf::Color MINT_MEDIUM(158, 223, 204);     // #9EDFCC
const sf::Color SKY_BLUE(106, 169, 217);        // #6AA9D9
const sf::Color DEEP_BLUE(43, 93, 135);         // #2B5D87
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
    PATIENT_SCREEN
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

sf::FloatRect getMenuButtonBounds(int index, int total, float windowW, float windowH)
{
    float buttonW = 360.0f;
    float buttonH = 50.0f;
    float gapX = 35.0f;
    float gapY = 18.0f;

    int columns = 2;
    int rows = (total + 1) / 2;

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

const char* getScreenTitle(Screen screen)
{
    if (screen == ADMIN_SCREEN) return "ADMIN PANEL";
    if (screen == DOCTOR_SCREEN) return "DOCTOR PANEL";
    if (screen == PATIENT_SCREEN) return "PATIENT PANEL";

    return "USER TYPE";
}

const char* getWelcomeText(Screen screen)
{
    if (screen == ADMIN_SCREEN)
        return "Admin Menu";

    if (screen == DOCTOR_SCREEN)
        return "Welcome, Dr. [Name] | Specialization: [spec]";

    if (screen == PATIENT_SCREEN)
        return "Welcome, [Patient Name]";

    return "Select your user type to continue";
}

const char* getSubWelcomeText(Screen screen)
{
    if (screen == ADMIN_SCREEN)
        return "Admin Panel";

    if (screen == DOCTOR_SCREEN)
        return "Manage appointments, prescriptions and patient history";

    if (screen == PATIENT_SCREEN)
        return "Balance: PKR [balance]";

    return "Choose Admin, Doctor, Patient or Exit";
}

const char* getLoginTitle(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return "ADMIN LOGIN";
    if (screen == DOCTOR_LOGIN_SCREEN) return "DOCTOR LOGIN";
    if (screen == PATIENT_LOGIN_SCREEN) return "PATIENT LOGIN";

    return "LOGIN";
}
Screen getPanelFromLogin(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN)
    {
        return ADMIN_SCREEN;
    }

    if (screen == DOCTOR_LOGIN_SCREEN)
    {
        return DOCTOR_SCREEN;
    }

    if (screen == PATIENT_LOGIN_SCREEN)
    {
        return PATIENT_SCREEN;
    }

    return USER_TYPE_SCREEN;
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

std::string getRoleFromLogin(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return "admin";
    if (screen == DOCTOR_LOGIN_SCREEN) return "doctor";
    if (screen == PATIENT_LOGIN_SCREEN) return "patient";

    return "";
}

void attemptLogin(
    Screen& currentScreen,
    std::string& idInput,
    std::string& passwordInput,
    int& activeBox,
    std::string& message
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

void drawUserTypeScreen(sf::RenderWindow& window, sf::Font& font)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);
    float windowH = static_cast<float>(size.y);

    sf::Text title(font, "USER TYPE", 42);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 150.0f);

    sf::Text subtitle(font, "Select your role to enter the system", 20);
    subtitle.setFillColor(TEXT_DARK);
    centerText(subtitle, windowW / 2.0f, 195.0f);

    window.draw(title);
    window.draw(subtitle);

    for (int i = 0; i < 4; i++)
    {
        sf::FloatRect rect = getUserBlockBounds(i, windowW, windowH);
        bool hover = mouseInside(window, rect);

        sf::RectangleShape block({ rect.size.x, rect.size.y });
        block.setPosition(rect.position);

        if (i == 3)
            block.setFillColor(hover ? DANGER_HOVER : DANGER);
        else
            block.setFillColor(hover ? SKY_BLUE : MINT_LIGHT);

        block.setOutlineThickness(3.0f);
        block.setOutlineColor(i == 3 ? DANGER : DEEP_BLUE);

        sf::Text blockText(font, getUserBlockText(i), 27);
        blockText.setStyle(sf::Text::Bold);

        if (i == 3)
            blockText.setFillColor(sf::Color::White);
        else
            blockText.setFillColor(DEEP_BLUE);

        centerText(
            blockText,
            rect.position.x + rect.size.x / 2.0f,
            rect.position.y + rect.size.y / 2.0f
        );

        window.draw(block);
        window.draw(blockText);
    }

    sf::Text hint(font, " Click a block to continue", 17);
    hint.setFillColor(TEXT_DARK);
    centerText(hint, windowW / 2.0f, windowH - 45.0f);
    window.draw(hint);
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
    panel.setOutlineColor(MINT_MEDIUM);
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
    panel.setOutlineColor(MINT_MEDIUM);

    window.draw(panel);

    sf::Text title(font, getScreenTitle(screen), 36);
    title.setFillColor(DEEP_BLUE);
    title.setStyle(sf::Text::Bold);
    centerText(title, windowW / 2.0f, 165.0f);
    window.draw(title);

    sf::Text welcome(font, getWelcomeText(screen), 20);
    welcome.setFillColor(TEXT_DARK);
    centerText(welcome, windowW / 2.0f, 205.0f);
    window.draw(welcome);

    sf::Text subWelcome(font, getSubWelcomeText(screen), 18);
    subWelcome.setFillColor(DEEP_BLUE);
    centerText(subWelcome, windowW / 2.0f, 232.0f);
    window.draw(subWelcome);

    int count = getMenuCount(screen);

    for (int i = 0; i < count; i++)
    {
        sf::FloatRect rect = getMenuButtonBounds(i, count, windowW, static_cast<float>(size.y));
        bool hover = mouseInside(window, rect);

        bool isLogout = false;

        if (screen == ADMIN_SCREEN && i == 9) isLogout = true;
        if (screen == DOCTOR_SCREEN && i == 5) isLogout = true;
        if (screen == PATIENT_SCREEN && i == 7) isLogout = true;

        sf::RectangleShape button({ rect.size.x, rect.size.y });
        button.setPosition(rect.position);

        if (isLogout)
            button.setFillColor(hover ? DANGER_HOVER : DANGER);
        else
            button.setFillColor(hover ? SKY_BLUE : WHITE_BG);

        button.setOutlineThickness(2.0f);
        button.setOutlineColor(isLogout ? DANGER : DEEP_BLUE);

        sf::Text buttonText(font, getMenuText(screen, i), 18);
        buttonText.setStyle(sf::Text::Bold);

        if (isLogout)
            buttonText.setFillColor(sf::Color::White);
        else
            buttonText.setFillColor(DEEP_BLUE);

        centerText(
            buttonText,
            rect.position.x + rect.size.x / 2.0f,
            rect.position.y + rect.size.y / 2.0f
        );

        window.draw(button);
        window.draw(buttonText);
    }

    sf::RectangleShape messageBox({ windowW - 220.0f, 54.0f });
    messageBox.setPosition({ 110.0f, 640.0f });
    messageBox.setFillColor(WHITE_BG);
    messageBox.setOutlineThickness(2.0f);
    messageBox.setOutlineColor(MINT_MEDIUM);
    window.draw(messageBox);

    sf::Text msg(font, message, 16);
    msg.setFillColor(TEXT_DARK);
    centerText(msg, windowW / 2.0f, 667.0f);
    window.draw(msg);
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

int getClickedMenuButton(sf::RenderWindow& window, Screen screen)
{
    sf::Vector2u size = window.getSize();
    float windowW = static_cast<float>(size.x);
    float windowH = static_cast<float>(size.y);

    int count = getMenuCount(screen);

    for (int i = 0; i < count; i++)
    {
        if (mouseInside(window, getMenuButtonBounds(i, count, windowW, windowH)))
        {
            return i;
        }
    }

    return -1;
}

bool isLoginScreen(Screen screen)
{
    if (screen == ADMIN_LOGIN_SCREEN) return true;
    if (screen == DOCTOR_LOGIN_SCREEN) return true;
    if (screen == PATIENT_LOGIN_SCREEN) return true;

    return false;
}

std::string getActionMessage(Screen screen, int index)
{
    if (screen == ADMIN_SCREEN)
    {
        if (index == 0) return "Add Doctor clicked. Next step: connect with your addDoctor function.";
        if (index == 1) return "Remove Doctor clicked. Next step: connect with your removeDoctor function.";
        if (index == 2) return "View All Patients clicked. Next step: show patient file data.";
        if (index == 3) return "View All Doctors clicked. Next step: show doctor file data.";
        if (index == 4) return "View All Appointments clicked. Next step: show appointments file data.";
        if (index == 5) return "View Unpaid Bills clicked. Next step: show unpaid bills.";
        if (index == 6) return "Discharge Patient clicked. Next step: connect discharge function.";
        if (index == 7) return "View Security Log clicked. Next step: show security log.";
        if (index == 8) return "Generate Daily Report clicked. Next step: create daily report.";
    }

    if (screen == DOCTOR_SCREEN)
    {
        if (index == 0) return "View Today's Appointments clicked. Next step: show today's appointments.";
        if (index == 1) return "Mark Appointment Complete clicked.";
        if (index == 2) return "Mark Appointment No-Show clicked.";
        if (index == 3) return "Write Prescription clicked. Next step: open prescription form.";
        if (index == 4) return "View Patient Medical History clicked.";
    }

    if (screen == PATIENT_SCREEN)
    {
        if (index == 0) return "Book Appointment clicked. Next step: open appointment form.";
        if (index == 1) return "Cancel Appointment clicked.";
        if (index == 2) return "View My Appointments clicked.";
        if (index == 3) return "View My Medical Records clicked.";
        if (index == 4) return "View My Bills clicked.";
        if (index == 5) return "Pay Bill clicked.";
        if (index == 6) return "Top Up Balance clicked.";
    }

    return "Option clicked.";
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

    std::optional<sf::Cursor> arrowCursor =
        sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);

    std::optional<sf::Cursor> handCursor =
        sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);

    Screen currentScreen = USER_TYPE_SCREEN;
    std::string message = "Ready";

    std::string idInput = "";
    std::string passwordInput = "";
    int activeBox = 0;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const sf::Event::Resized* resized =
                event->getIf<sf::Event::Resized>())
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

            if (const sf::Event::KeyPressed* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    if (currentScreen == USER_TYPE_SCREEN)
                    {
                        window.close();
                    }
                    else if (isLoginScreen(currentScreen))
                    {
                        currentScreen = USER_TYPE_SCREEN;
                        idInput = "";
                        passwordInput = "";
                        activeBox = 0;
                        message = "Returned to user type screen.";
                    }
                    else
                    {
                        currentScreen = USER_TYPE_SCREEN;
                        message = "Logged out successfully.";
                    }
                }

                if (isLoginScreen(currentScreen))
                {
                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        attemptLogin(currentScreen, idInput, passwordInput, activeBox, message);
                    }
            }

            if (const sf::Event::TextEntered* textEvent =
                event->getIf<sf::Event::TextEntered>())
            {
                if (isLoginScreen(currentScreen))
                {
                    unsigned int code = textEvent->unicode;

                    if (code >= 32 && code <= 126)
                    {
                        char ch = static_cast<char>(code);

                        if (activeBox == 1)
                        {
                            if (ch >= '0' && ch <= '9')
                            {
                                if (idInput.length() < 10)
                                    idInput += ch;
                            }
                        }
                        else if (activeBox == 2)
                        {
                            if (passwordInput.length() < 20)
                                passwordInput += ch;
                        }
                    }
                }
            }

            if (const sf::Event::MouseButtonReleased* mouseEvent =
                event->getIf<sf::Event::MouseButtonReleased>())
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
                            if (idInput.length() == 0 || passwordInput.length() == 0)
                            {
                                message = "Please enter both ID and password.";
                            }
                            else
                            {
                                currentScreen = getPanelFromLogin(currentScreen);
                                idInput = "";
                                passwordInput = "";
                                activeBox = 0;
                                message = "Login successful. Real validation will be connected next.";
                            }
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
                    else
                    {
                        int clicked = getClickedMenuButton(window, currentScreen);

                        if (clicked != -1)
                        {
                            int lastIndex = getMenuCount(currentScreen) - 1;

                            if (clicked == lastIndex)
                            {
                                currentScreen = USER_TYPE_SCREEN;
                                message = "Logged out successfully.";
                            }
                            else
                            {
                                message = getActionMessage(currentScreen, clicked);
                            }
                        }
                    }
                }
            }
        }

        bool hovering = false;

        if (currentScreen == USER_TYPE_SCREEN)
        {
            if (getClickedUserBlock(window) != -1)
                hovering = true;
        }
        else if (isLoginScreen(currentScreen))
        {
            sf::Vector2u size = window.getSize();
            float windowW = static_cast<float>(size.x);

            if (mouseInside(window, getLoginIdBox(windowW)) ||
                mouseInside(window, getLoginPasswordBox(windowW)) ||
                mouseInside(window, getLoginButton(windowW)) ||
                mouseInside(window, getBackButton(windowW)))
            {
                hovering = true;
            }
        }
        else
        {
            if (getClickedMenuButton(window, currentScreen) != -1)
                hovering = true;
        }

        if (hovering && handCursor.has_value())
        {
            window.setMouseCursor(handCursor.value());
        }
        else if (arrowCursor.has_value())
        {
            window.setMouseCursor(arrowCursor.value());
        }

        if (currentScreen == USER_TYPE_SCREEN)
            window.clear(MINT_MEDIUM);
        else
            window.clear(WHITE_BG);

        sf::Vector2u size = window.getSize();
        float windowW = static_cast<float>(size.x);

        drawTopBar(window, font, windowW);

        if (currentScreen == USER_TYPE_SCREEN)
        {
            drawUserTypeScreen(window, font);
        }
        else if (isLoginScreen(currentScreen))
        {
            drawLoginScreen(window, font, currentScreen, idInput, passwordInput, activeBox, message);
        }
        else
        {
            drawRoleMenu(window, font, currentScreen, message);
        }

        window.display();
    }

    return 0;
}