#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;

struct User {
    string login;
    string password;
};
vector <User> readUsersFromFile(const string& filename) {
    vector<User> users;
    ifstream infile(filename);
    string line;

    if (infile.is_open()) {
        User user;
        while (getline(infile, line)) {
            if (line.find("login:") != string::npos) {
                user.login = line.substr(line.find(":") + 1);
            }
            else if (line.find("password:") != string::npos) {
                user.password = line.substr(line.find(":") + 1);
                users.push_back(user);
            }
        }
        infile.close();
    }
    else {
        cerr << "Ошибка при открытии файла" << filename << endl;
    }
    return users;
}


void writeLog(const string& filename, const string& message) {

    ofstream logfile(filename, ios_base::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        logfile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " - " << message << endl;
        logfile.close();
    }
    else {
        cerr << "Ошибка при открытии файла логов:" << filename << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    string enteredLogin, enteredPassword;
    string credentialsFilePath = "C:\\Users\\s.kolesnikov\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\credentials.txt";
    string logFilePath = "C:\\Users\\s.kolesnikov\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\log.txt";

    vector<User> users = readUsersFromFile(credentialsFilePath);

    cout << "Введите логин:";
    cin >> enteredLogin;
    cout << "Введите пароль:";
    cin >> enteredPassword;

    bool found = false;
    for (const auto& user : users) {
        if (user.login == enteredLogin && user.password == enteredPassword) {
            cout << "Авторизация успешна!" << endl;
            found = true;
            writeLog(logFilePath, "Пользователь" + enteredLogin + " вошел в систему.");
            break;
        }
    }

    if (!found) {
        cout << "Неверный логин или пароль." << endl;
        writeLog(logFilePath, "Неудачная попытка входа.");
    }

    return 0;
}