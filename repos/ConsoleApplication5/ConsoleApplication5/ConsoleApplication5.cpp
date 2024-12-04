#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// Структура для хранения данных о пользователе
struct User {
    string login;
    string password;
    string access_key;
};


struct Data {
    string name;
    string value;
};

// Функция для получения текущей метки времени
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto now_time_t = chrono::system_clock::to_time_t(now);

    tm localTime;
    if (localtime_s(&localTime, &now_time_t) != 0) {
        return "Неизвестное время";
    }

    ostringstream timestampStream;
    timestampStream << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return timestampStream.str();
}


vector<Data> readDataFromFile(const string& filename) {
    vector<Data> data;
    setlocale(LC_ALL, "ru");
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Ошибка открытия файла:" << filename << endl;
        return data;
    }

    string line;
    while (getline(inputFile, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            Data newData;
            newData.name = line.substr(0, pos);
            newData.value = line.substr(pos + 1);
            data.push_back(newData);
        }
        else {
            cerr << "Некорректная строка в файле:" << line << endl;
        }
    }

    inputFile.close();
    return data;
}


bool writeDataToFile(const vector<Data>& data, const string& filename) {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cerr << "Ошибка открытия файла для записи:" << filename << endl;
        return false;
    }

    for (const auto& item : data) {
        outputFile << item.name << " " << item.value << endl;
    }

    outputFile.close();
    return true;
}

// Новая функция для фильтрации данных по имени
void filterDataByName(const vector<Data>& data) {
    string filter;
    cout << "Введите часть имени для фильтрации:";
    cin.ignore();
    getline(cin, filter);

    cout << "Результаты фильтрации:" << endl;
    bool found = false;
    for (const auto& item : data) {
        if (item.name.find(filter) != string::npos) {
            cout << item.name << " " << item.value << endl;
            found = true;
        }

    }

    if (!found) {
        cout << "Записи не найдены." << endl;
    }
}

// Функция импорта данных из файла
bool importDataFromFile(vector<Data>& data, const string& filename) {
    vector<Data> importedData = readDataFromFile(filename);
    if (importedData.empty()) {
        return false; // Импорт не удался
    }
    data.insert(data.end(), importedData.begin(), importedData.end());
    return true;
}

// Функция экспорта данных в файл
bool exportDataToFile(const vector<Data>& data, const string& filename) {
    return writeDataToFile(data, filename);
}

int main() {
    setlocale(LC_ALL, "ru");
    string login, password;
    cout << "Введите логин:";
    cin >> login;
    cout << "Введите пароль:";
    cin >> password;

    string userFilePath = "C:\\Users\\s.kolesnikov\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\credentials.txt";
    ifstream userFile(userFilePath);
    if (!userFile.is_open()) {
        cerr << "Ошибка открытия файла" << userFilePath << endl;
        return 1;
    }

    string logFilePath = "C:\\Users\\s.kolesnikov\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\log.log";
    ofstream logFile(logFilePath, ios::app);
    if (!logFile.is_open()) {
        cerr << "Ошибка открытия файла" << logFilePath << endl;
        return 1;
    }

    vector<User> users;
    string line;
    while (getline(userFile, line)) {
        if (line.empty()) continue;
        User user;
        user.login = line;
        getline(userFile, user.password);
        getline(userFile, user.access_key);
        if (userFile.fail()) break;
        users.push_back(user);
    }
    userFile.close();
    bool authorized = false;
    for (const auto& user : users) {
        if (login == user.login && password == user.password) {
            authorized = true;
            cout << "Код разрешен. Ключ доступа:" << user.access_key << endl;
            logFile << getCurrentTimestamp() << " Пользователь " << login << " вошел в систему" << endl;
            break;
        }
    }

    if (!authorized) {
        cout << "Неверный логин или пароль." << endl;
        logFile << getCurrentTimestamp() << " Неудачная попытка входа пользователя " << login << endl;
        logFile.close();
        return 1;
    }

    string dataFilename = "C:\\Users\\s.kolesnikov\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\student.txt";
    vector<Data> data = readDataFromFile(dataFilename);

    int choice = -1;
    do {
        cout << "\nМеню:" << endl;
        cout << "1. Добавить запись" << endl;
        cout << "2. Удалить запись" << endl;
        cout << "3. Редактировать запись" << endl;
        cout << "4. Вывести все записи" << endl;
        cout << "5. Фильтровать записи" << endl; // Новый пункт меню
        cout << "6. Импортировать данные" << endl;
        cout << "7. Экспортировать данные" << endl;
        cout << "0. Выход" << endl;
        cout << "Введите ваш выбор:";

        if (!(cin >> choice)) {
            cout << "Неверный ввод. Пожалуйста, введите число." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            Data newData;
            cout << "Введите имя:";
            cin.ignore();
            getline(cin, newData.name);
            cout << "Введите значение:";
            getline(cin, newData.value);
            data.push_back(newData);
            break;
        }
        case 2: {
            string nameToDelete;
            cout << "Введите имя записи для удаления:";
            cin.ignore();
            getline(cin, nameToDelete);
            data.erase(remove_if(data.begin(), data.end(),
                [&](const Data& item) { return item.name == nameToDelete; }), data.end());
            break;
        }
        case 3: {
            string nameToEdit;
            cout << "Введите имя записи для редактирования:";
            cin.ignore();
            getline(cin, nameToEdit);
            auto it = find_if(data.begin(), data.end(), [&](const Data& item) { return item.name == nameToEdit; });
            if (it != data.end()) {
                cout << "Введите новое значение:";
                string newValue;
                getline(cin, newValue);
                it->value = newValue;
            }
            else {
                cout << "Запись с именем \"" << nameToEdit << "\" не найдена." << endl;
            }
            break;
        }
        case 4: {
            if (data.empty()) {
                cout << "Список записей пуст." << endl;
            }
            else {
                for (const auto& item : data) {
                    cout << item.name << ": " << item.value << endl;
                }
            }
            break;
        }
        case 5: {
            filterDataByName(data); // Вызов функции фильтрации
            break;
        }

        case 6: { // Импортировать данные
            string importFilename;
            cout << "Введите имя файла для импорта:";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, importFilename);
            if (importDataFromFile(data, importFilename)) {
                cout << "Данные успешно импортированы." << endl;
            }
            else {
                cout << "Ошибка импорта данных." << endl;
            }
            break;
        }
        case 7: { // Экспортировать данные
            string exportFilename;
            cout << "Введите имя файла для экспорта:";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, exportFilename);
            if (exportDataToFile(data, exportFilename)) {
                cout << "Данные успешно экспортированы." << endl;
            }
            else {
                cout << "Ошибка экспорта данных." << endl;
            }
            break;
        }

        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }

        if (!writeDataToFile(data, dataFilename)) {
            cerr << "Ошибка записи данных в файл." << endl;
        }
    } while (choice != 0);

    logFile << getCurrentTimestamp() << " Пользователь " << login << " вышел из системы" << endl;
    logFile.close();
    return 0;
}