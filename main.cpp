#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <iomanip>
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
            if(newPrice >= 0) pricePerSlot = newPrice;
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
        if (cin >> choiceUser && choiceUser >= 0 && choiceUser <= 2) return choiceUser;
        
        cout << "Nieprawidlowe dane. Sprobuj ponownie." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

//Testowe - AI
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

void adminPanel(const User &admin, const Resource &res){
     cout << "==================================================\n"
         << "   PANEL ADMINA | Zalogowany: " << admin.getFullNameUser() << " | ID: " << admin.getIdUser() << '\n'
         << "==================================================" << "\n\n";

    cout << "   --- ZARZĄDZANIE ZASOBAMI ---\n";
    cout << "   [1] Lista wszystkich zasobów\n";
    cout << "   [2] Dodaj nowy zasób\n";
    cout << "   [3] Zmień cenę / dostępność zasobu\n\n";

    cout << "   --- PRZEGLĄD SYSTEMU ---\n";
    cout << "   [4] Pełny harmonogram rezerwacji (Wszystkie)\n";
    cout << "   [5] Raport przychodów i obłożenia\n\n";

    cout << "   [0] Wyloguj / Powrot do ekranu startowego\n\n";
    cout << "==================================================\n";
}

void clientPanel(const User &client, const Resource &res){
    cout << "==================================================\n"
         << "   PANEL KLIENTA | Zalogowany: " << client.getFullNameUser() << " | ID: " << client.getIdUser() << '\n'
         << "==================================================" << "\n\n";

    cout << "   --- STREFA REZERWACJI ---\n";
    cout << "   [1] Przegladaj dostepne zasoby i cennik\n";
    cout << "   [2] Zloz nowa rezeracje\n\n";

    cout << "   --- MOJE KONTO ---\n";
    cout << "   [3] Moje akutalne rezeracje\n";
    cout << "   [4] Anuluj rezeracje\n\n";

    cout << "   [0] Wyloguj / Powrot do ekranu startowego\n\n";
    cout << "==================================================\n";

}

int clientChoiceLog(){
    int clientChoiceUser;
    while (true) {
        cout << "wybierz opcje > ";
        if (cin >> clientChoiceUser && clientChoiceUser >= 0 && clientChoiceUser <= 4) return clientChoiceUser;
        
        cout << "Nieprawidlowe dane. Sprobuj ponownie" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void displayResourcesAndPricing(const vector<Resource>& resources, bool isActive = false){
    cout << "==============================================================================\n"
         << "                           LISTA ZASOBOW I CENNIK                             \n"
         << "==============================================================================\n";

    if(resources.empty()){
        cout << "  Brak zarejestrowanych zasobów w systemie.\n";
        cout << "==============================================================================\n";
        return;
    }

    cout << left
        << setw(7)  << "ID"
        << setw(30) << "Nazwa Zasobow"
        << setw(14) << "Pojemnosc"
        << setw(15) << "Cena/godz."
        << "Status" << "\n";  

    cout << string(78, '-') << "\n";
    for(const auto& res : resources){
        if(isActive && !res.getIsActive())
            continue;
        
    

        string capStr = to_string(res.getCapacity()) + " os.";
        cout << left
            <<setw(7)  << res.getIdResource()
            <<setw(30) << res.getNameResource()
            <<setw(14) << capStr
            <<setw(15) << res.getPricePerSlot();
        
        cout << left
            << (res.getIsActive() ? "[Dostepny]" : "[Niedotepny]") << "\n"; 
    }
    cout << "==============================================================================\n";
}

//AI
time_t makeTimestamp(int year, int month, int day, int hour) {
    struct tm timeInfo = {0};
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;

    time_t timestamp = mktime(&timeInfo);
    if (timestamp == -1 ||
        timeInfo.tm_year != year - 1900 ||
        timeInfo.tm_mon != month - 1 ||
        timeInfo.tm_mday != day ||
        timeInfo.tm_hour != hour) {
        return -1;
    }
    return timestamp;
}
//AI
string formatTimestamp(time_t timestamp) {
    struct tm *timeInfo = localtime(&timestamp);
    if (!timeInfo) return "n/a";
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeInfo);
    return string(buffer);
}

string reservationStatusToString(ReservationStatus status) {
    switch(status) {
        case ReservationStatus::PENDING: return "PENDING";
        case ReservationStatus::CONFIRMED: return "CONFIRMED";
        case ReservationStatus::CANCELLED: return "CANCELLED";
        case ReservationStatus::COMPLETED: return "COMPLETED";
        default: return "UNKNOWN";
    }
}

//AI
const Resource* findResourceById(const vector<Resource>& resources, unsigned int resourceId) {
    for (const auto& resource : resources) {
        if (resource.getIdResource() == resourceId) {
            return &resource;
        }
    }
    return nullptr;
}

void displayUserReservations(const vector<Reservation>& reservations, const vector<Resource>& resources, unsigned int userId) {
    clearScreen();
    cout << "==================================================\n"
         << "                TWOJE REZERWACJE                  \n"
         << "==================================================\n";

    bool hasReservation = false;
    for(const auto& res : reservations){
        if(res.getUserId() != userId) continue;
        hasReservation = true;
        const Resource* resource = findResourceById(resources, res.getResourceId());
        cout << "   ID rezerwacji: " << res.getId() << "\n"
             << "   Zasob: " << (resource ? resource->getNameResource() : string("Nieznany")) << "\n"
             << "   Data pocz.: " << formatTimestamp(res.getStartTimestamp()) << "\n"
             << "   Data zak.: " << formatTimestamp(res.getEndTimestamp()) << "\n"
             << "   Miejsce: " << res.getReservedSeats() << "\n"
             << "   Status: " << reservationStatusToString(res.getStatus()) << "\n" 
             << "   Cena: " << res.getTotalPrice() <<  "\n" 
            << "--------------------------------------------------\n";
    }
    if(!hasReservation){
        cout << "Brak aktywnych rezerwacji dla Twojego konta.\n";
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    
    cout << "Nacisnij Enter, aby kontynuowac...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void newReservation(const User& user, const vector<Resource>& resources, vector<Reservation>& reservations){
    unsigned int resID;
    cout << "==================================================\n"
         << "           TWORZENIE NOWEJ REZERWACJI             \n"
         << "==================================================\n";
    
    cout << "Podaj id > ";
    cin >> resID;
    clearScreen();
    cout << string(78, '-') << "\n";
    const Resource* targetResource  = nullptr;
    for(const auto& r : resources){
        if(r.getIdResource() == resID && r.getIsActive()){
            targetResource = &r;
            break;
        }
    }
    if(!targetResource){
        clearScreen();
        cout << "Blad nie ma lub nie jest dostepny obecnie: " << resID << endl;
        return;
    }

    int Year, Month, Day, startHour, endHour;
    unsigned int seat;
    clearScreen();
    cout << "Podaj date od-do Dzien - Miesiac - Rok> \n";
    cout << string(78, '-') << "\n";
    cin >> Day >> Month >> Year;
    if(Day > 31 && Day < 1 || Month > 12 && Month < 1 || Year < 2026) return;

    clearScreen();
    cout << "Podaj Godzine poczatkowa > ";  cin >> startHour;
    cout << "Podaj godzine zakonczenia > "; cin >> endHour;
        cout << string(78, '-') << "\n";
    if (endHour <= startHour || startHour < 0 && startHour > 24 || endHour < 0 && endHour > 24) {
        clearScreen();
        cout << "\n[!] Błąd: Godzina zakończenia musi być późniejsza niż rozpoczęcia.\n";
        return;
    }

    clearScreen();
    cout << "Podaj liczbe miejsc do zarezerwowania (maksylanie: " << targetResource->getCapacity() << ") : ";
    cin >> seat;
    cout << string(50, '-') << "\n";
    if(seat == 0 || seat > targetResource->getCapacity()){
        clearScreen();
        cout << "Niepoprawna ilosc miejsc\n";
        return;
    }

    time_t startTs = makeTimestamp(Year, Month, Day, startHour);
    time_t endTs   = makeTimestamp(Year, Month, Day, endHour);
    if(startTs == -1 || endTs == -1){
        clearScreen();
        cout << "Niepoprawna data\n";
        return;
    }

    Reservation newRes(user.getIdUser(), targetResource->getIdResource(),
                 startTs, endTs, seat, targetResource->getPricePerSlot());

    clearScreen();
    cout << "==================================================\n"
         << "           TWORZENIE NOWEJ REZERWACJI             \n"
         << "==================================================\n";
    cout << "   Zasób:        Sala Konferencyjna A\n"
         << "   Termin:      " << Day << " "  << Month << " " << Year << "\n"
         << "   Miejsca:     " << seat << "\n"
         << "   Stawka:      50.00zl\n";
    cout << string(50, '-') << "\n";
    cout << "   LACZNY KOSZT:   " << newRes.getTotalPrice() << "\n";

    reservations.push_back(newRes);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

}

void cancelReservationForClient(const User& user, vector<Reservation>& reservation){
    clearScreen();
     cout << "==================================================\n"
          << "               ANULOWANIE REZERWACJI              \n"
          << "==================================================\n";

    bool isActiveReservation = false;
    for(const auto& res : reservation){

        if(res.getUserId() == user.getIdUser() && res.getStatus() != ReservationStatus::CANCELLED){
            isActiveReservation = true;
            cout << "ID: " << res.getId() << "\n"
                 << "ID Zasobu: " << res.getResourceId() << "\n"
                 << "ID Miejsca: " << res.getReservedSeats() << "\n"
                 << "Koszt: " << res.getTotalPrice() << "\n";
        }
    }
    if(!isActiveReservation){
        clearScreen();
        cout << "Nie posiadasz zadnej rejestacji\n";
        return;
    }
    unsigned int cancelResID;
    cout << "Podaj id rezerwacji do anulowania > ";
    cin >> cancelResID;
    if(cancelResID == 0) return;
    for(auto& res : reservation){
        if(res.getId() == cancelResID && res.getUserId() == user.getIdUser()){
            if(res.setCancel()){
                cout << "Udalo sie pomyslnie anulowac rezeracje o ID: " << cancelResID << "\n";
            }else{
                cout << "Nie mozna anulowac rezeracji!\n";
            }
            return;
        }
    }
    cout << "Nie znaleziono rezerwacji\n";
}

int main(){
    vector<User> users;
    vector<Resource> resources;
    vector<Reservation> reservations;
    users.push_back(User("Jan Kowalski", "+48 111 222 333", Role::ADMIN));
    users.push_back(User("Anna Nowak", "+48 999 888 777", Role::CLIENT));
    resources.push_back(Resource("Sala Konferencyjna A", 20, 50.0));
    while(true){
        clearScreen();
        header();
        logOption();
        int choiceUser = userChoiceLog();
        if(choiceUser == 1){
            while(true){
                clearScreen();
                clientPanel(users[1], resources[0]);
                int clientChoiceUser = clientChoiceLog();
                if(clientChoiceUser == 0){
                    break;
                }
                switch(clientChoiceUser){
                    case 1:
                        int backToClientPanel;
                        clearScreen();
                        displayResourcesAndPricing(resources, false);
                        cout << "[0] Powrot > "; cin >> backToClientPanel;
                        if(backToClientPanel == 0){
                            clientPanel(users[1], resources[0]);
                        }
                        break;
                    case 2:
                        clearScreen();
                        newReservation(users[1], resources, reservations);
                        break;
                    case 3:
                        displayUserReservations(reservations, resources, users[1].getIdUser());
                        break;
                    case 4:
                        cancelReservationForClient(users[1], reservations); 
                    break;
                }
            }
        }
        else if(choiceUser == 2){
            clearScreen();
            // daneAdmin(users[0], users[1], resources[0]);
            // cout << "\nNacisnij Enter, aby wrocic do menu";
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // cin.get();
            while(true){
                adminPanel(users[0], resources[0]);
            }
        }
        else{
            clearScreen();
            return 0;
        }
    }
}