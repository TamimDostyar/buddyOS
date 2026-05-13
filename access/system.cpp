#include <iostream>
#include <string>

using namespace std;

class Systems {
    protected:
        string name;
        int number;

    public:
        Systems(string user, int timeFrame) {
            name = user;
            number = timeFrame;
        }

        void greetUser() {
            cout << "Hey " << name << endl;
        }
};

class UserInteface {
    protected:
        string name;

    public:
        UserInteface(string user) {
            name = user;
        }

        int systemConf(int year) {
            if (year != 2025) {
                cout << "year off" << endl;
                return 1;
            }
            return 0;
        }

        void greetInterface() {
            cout << "Welcome " << name << endl;
        }
};

class WelcomePage : public Systems, public UserInteface {
    public:
        WelcomePage(string name, int time)
            : Systems(name, time), UserInteface(name) {}
};

int main() {
    WelcomePage obj("Alex", 2025);

    obj.greetUser();
    obj.greetInterface();

    return 0;
}