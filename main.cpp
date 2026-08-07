#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <time.h>
using namespace std;

enum class Menu{
    logInClient = 1,
    logInAdmin  = 2,
    logOut      = 3
};

enum class Role{
    CLIENT,
    ADMIN
};

enum class ReservationStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

class Reservation{
    private:
        static unsigned int nextAutoIncrementId;
        unsigned int id;
        unsigned int userId;
        unsigned int resourceId;
        time_t startTimestamp;
        time_t endTimestamp;
        unsigned int reservedSeats;
        ReservationStatus status;
        double totalPrice;

        void calculateTotalPrice(double pricePerSlot){
            if(endTimestamp <= startTimestamp){
                totalPrice = 0.0;
                return;
            }
            double durationHours = difftime(endTimestamp, startTimestamp) / 3600;
            totalPrice = durationHours * reservedSeats * pricePerSlot;
        }
    public:
        Reservation(unsigned int uId, unsigned rId, time_t start, time_t end, unsigned int seats, double pricePerSlot)
            : id(nextAutoIncrementId++), userId(uId), resourceId(rId), startTimestamp(start), endTimestamp(end),
                 reservedSeats(seats), status(ReservationStatus::PENDING){
                calculateTotalPrice(pricePerSlot);
            }

        // Tylko do odczytu danych
        unsigned int getId()            const {return id;}
        unsigned int getUserId()        const {return userId;}
        unsigned int getResourceId()    const {return resourceId;}
        time_t getStartTimestamp()      const {return startTimestamp;}
        time_t getEndTimestamp()        const {return endTimestamp;}
        unsigned int getReservedSeats() const {return reservedSeats;}
        ReservationStatus getStatus()   const {return status;}
        double getTotalPrice()          const {return totalPrice;}
        
        void setStatus(ReservationStatus newStatus){status = newStatus;}
        bool setCancel(){
            if(status == ReservationStatus::COMPLETED) return false;
            status = ReservationStatus::CANCELLED;
            return true;
        }
};

class User{
    private:
        static unsigned int nextAutoIncrementId;
        unsigned int id;
        string fullName;
        string phoneNumber;
        Role role;
    public:
        User(string name, string phone, Role r) 
            : id(nextAutoIncrementId++), fullName(name), phoneNumber(phone), role(r){}
        // Tylko do odczytu danych
        unsigned getIdUser() const          {return id;}
        string getFullNameUser() const      {return fullName;}
        string getPhoneNumberUser() const   {return phoneNumber;}
        Role getRole() const                {return role;}
};

class Resource{
    private:
        static unsigned int nextAutoIncrementId;
        unsigned int id;
        string name;
        unsigned int capacity;
        double pricePerSlot;
        bool isActive;
    public:
        Resource(string n, unsigned int cap, double price) 
            : id(nextAutoIncrementId++), name(n), capacity(cap), pricePerSlot(price), isActive(true) {} 
        unsigned int getIdResource() const {return id;}
        string getNameResource()     const {return name;}
        unsigned int getCapacity()   const {return capacity;}
        double getPricePerSlot()     const {return pricePerSlot;}
        bool getIsActive()           const {return isActive;}
        
        void setIsActive(bool active){isActive = active;}
        void setPricePerSlot(double newPrice){
            if(pricePerSlot >= 0) pricePerSlot = newPrice;
        }
};

unsigned int Reservation::nextAutoIncrementId = 1;
unsigned int User::nextAutoIncrementId        = 1;
unsigned int Resource::nextAutoIncrementId    = 1;

void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void header(){
    cout << "==================================================\n"
         << "           SYSTEM REZERWACJI ZASOBOW\n"
         << "==================================================\n";
}

void logOption(){
    cout << endl;
    cout << "   [1] Zaloguj jako Klient\n";
    cout << "   [2] Zaloguj jako Administrator\n";
    cout << "   [0] Wyjscie z programu\n\n";

    cout << "==================================================\n";
}

int userChoiceLog(){
    int choiceUser;
    while (true) {
        cout << "wybierz opcje > ";
        if (cin >> choiceUser && choiceUser >= 1 && choiceUser <= 3) {
            return choiceUser;
        }
        cout << "Nieprawidlowe dane. Sprobuj ponownie." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void daneAdmin(const User &admin, const User &client, const Resource &res){
    std::cout << "=== KONTO ADMINA ===" << std::endl;
    std::cout << "ID: " << admin.getIdUser() << std::endl;
    std::cout << "Nazwa: " << admin.getFullNameUser() << std::endl;
    std::cout << "Rola: " << (admin.getRole() == Role::ADMIN ? "ADMIN" : "CLIENT") << std::endl;

    std::cout << "\n=== KONTO KLIENTA ===" << std::endl;
    std::cout << "ID: " << client.getIdUser() << std::endl;
    std::cout << "Nazwa: " << client.getFullNameUser() << std::endl;
    std::cout << "Rola: " << (client.getRole() == Role::ADMIN ? "ADMIN" : "CLIENT") << std::endl;

    std::cout << "\n=== ZASÓB ===" << std::endl;
    std::cout << "ID Zasobu: " << res.getIdResource() << std::endl;
    std::cout << "Nazwa: " << res.getNameResource() << std::endl;
    std::cout << "Pojemność: " << res.getCapacity() << " osób" << std::endl;
    std::cout << "Cena/h: " << res.getPricePerSlot() << " PLN" << std::endl;
}

void clientPanel(const User &client, const Resource &res){
    cout << "==================================================\n"
         << "   PANEL KLIENTA | Zalogowany: " << client.getFullNameUser() << " | ID: " << client.getIdUser() << '\n'
         << "==================================================" << "\n\n";
    
    cout << "   --- MOJE KONTO ---\n\n";
    cout << "   [3] Moje akutalne rezeracje\n";
    cout << "   [4] Anuluj rezeracje\n\n";
    cout << "   [0] Wyloguj / Powrot do ekranu startowego\n\n";

    //Wybierz opcję >
}

int clientChoiceLog(){
    int choiceUser;
    while (true) {
        cout << "wybierz opcje > ";
        if (cin >> choiceUser && choiceUser >= 3 && choiceUser <= 4) {
            return choiceUser;
        }
        cout << "Nieprawidlowe dane. Sprobuj ponownie." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main(){
    vector<User> users;
    vector<Resource> resources;
    users.push_back(User("Jan Kowalski", "+48 111 222 333", Role::ADMIN));
    users.push_back(User("Anna Nowak", "+48 999 888 777", Role::CLIENT));
    resources.push_back(Resource("Sala Konferencyjna A", 20, 50.0));

    while(true){
        clearScreen();
        header();
        logOption();
        int choiceUser = userChoiceLog();
        if(choiceUser == 1){
            clearScreen();
            clientPanel(users[1], resources[0]);
            clientChoiceLog();
            int choiceUser = clientChoiceLog();
            
        }
        else if(choiceUser == 2){
            clearScreen();
            daneAdmin(users[0], users[1], resources[0]);
        }
        else{
            clearScreen();
            return 0;
        }
    }
}