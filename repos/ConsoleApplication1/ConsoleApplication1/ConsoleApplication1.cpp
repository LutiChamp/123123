#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

string generateLogin(int length = 8) {
    const string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> distrib(0, chars.length() - 1);

    string login;
    for (int i = 0; i < length; ++i) {
        login += chars[distrib(g)];
    }
    return login;
}


string generatePassword(int length = 12) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_-+={}[]|;:'\",.<>/?";
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> distrib(0, chars.length() - 1);

    string password;
    for (int i = 0; i < length; ++i) {
        password += chars[distrib(g)];
    }
    return password;
}

int main() {
    ofstream outfile("credentials.txt", ios_base::app); 

    if (outfile.is_open()) {
        for (int i = 0; i < 5; ++i) { 
            string login = generateLogin();
            string password = generatePassword();
            outfile << "login:" << login << endl;
            outfile << "password:" << password << endl << endl;
        }
        outfile.close();
        cout << "5 записей добавлено в credentials.txt" << endl;
    }
    else {
        cerr << "Ошибка при открытии файла credentials.txt" << endl;
    }

    return 0;
}

