package buddyOS.access;

class Systems {
    protected String name;
    protected int number;

    public Systems(String user, int timeFrame) {
        name = user;
        number = timeFrame;
    }

    public void greetUser() {
        System.out.println("Hey " + name);
    }
}


interface UserInterface {

    int systemConf(int year);

    void greetInterface();
}


class WelcomePage extends Systems implements UserInterface {

    public WelcomePage(String name, int time) {
        super(name, time);
    }

    @Override
    public int systemConf(int year) {

        if (year != number) {
            System.out.println("year off");
            return 1;
        }

        return 0;
    }

    @Override
    public void greetInterface() {
        System.out.println("Welcome " + name);
    }
}


public class Main {

    public static void main(String[] args) {

        WelcomePage obj = new WelcomePage("Alex", 2025);

        obj.greetUser();
        obj.greetInterface();

        obj.systemConf(2024);
    }
}