#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <iomanip>
#include <ctime>
using namespace std;

// Wybor opcji menu
enum class Menu{
    logInClient = 1,
    logInAdmin  = 2,
    logOut      = 3
};

// Role uzytkownikow 
enum class Role{
    CLIENT,
    ADMIN
};

//Przypisanie statusu rezeracji
enum class ReservationStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

class Reservation{
    private:
        static unsigned int nextAutoIncrementId; // static unsigned int. Widoczna tylko dla class Reservation. unsigned sprawa zmienną tylko dodatnią 
        unsigned int id; // ID rezerwacji
        unsigned int userId; // ID uzytkownia w rezerwacji
        unsigned int resourceId; // ID zasobu
        time_t startTimestamp; // Data początkowa
        time_t endTimestamp; // Data zakonczenia
        unsigned int reservedSeats; // Stawka za osobe
        ReservationStatus status; // Status rezerwacji (enum class ReservationStatus)
        double totalPrice; // Cena calkowita

        // Obliczanie ceny rezeracji
        void calculateTotalPrice(double pricePerSlot){
            // Jeśli data zakonczenia jest taka sama lub wczesniejsza niz data startowa cena calkowita jest 0 
            if(endTimestamp <= startTimestamp){
                totalPrice = 0.0;
                return;
            }
            // Obliczanie ceny rezerwacji w godzinach przez czas trwania i stawke 
            double durationHours = difftime(endTimestamp, startTimestamp) / 3600;
            totalPrice = durationHours * reservedSeats * pricePerSlot;
        }
    public:
        //Tworzenie nowej rezerwacji
        //Przypisanie zmiennych do informacji o nowej rezerwacjii
        Reservation(unsigned int uId, unsigned rId, time_t start, time_t end, unsigned int seats, double pricePerSlot)
                // nextAutoIncrementId++ (Za kazdym razem o + 1)
                // ID Uzytkownika 
                // ID Rezerwacji
                // Data początkowa
                // Data zakonczenia
                // Stawka za osobe
                // Ustawienie statusu rezerwacji na PENDING
            : id(nextAutoIncrementId++), userId(uId), resourceId(rId), startTimestamp(start), endTimestamp(end),
                 reservedSeats(seats), status(ReservationStatus::PENDING){
                // Obliczenie ceny calkowitej
                calculateTotalPrice(pricePerSlot);
            }

        // Tylko do odczytu danych (const)
        unsigned int getId()            const {return id;}
        unsigned int getUserId()        const {return userId;}
        unsigned int getResourceId()    const {return resourceId;}
        time_t getStartTimestamp()      const {return startTimestamp;}
        time_t getEndTimestamp()        const {return endTimestamp;}
        unsigned int getReservedSeats() const {return reservedSeats;}
        ReservationStatus getStatus()   const {return status;}
        double getTotalPrice()          const {return totalPrice;}
        
        // Ustawienie nowego statusu dla rezerwacji
        void setStatus(ReservationStatus newStatus){status = newStatus;}
        // sprawdzenia czy status rezerwacji jest COMPLETED jesli tak -> Ustawienie statusu rezerwacji na CANCELLED 
        bool setCancel(){
            if(status == ReservationStatus::COMPLETED) return false;
            status = ReservationStatus::CANCELLED;
            return true;
        }
};

class User{
    private:
        static unsigned int nextAutoIncrementId; // static unsigned int. Widoczna tylko dla class Reservation. unsigned sprawa zmienną tylko dodatnią
        unsigned int id; // ID uzytkownika
        string fullName; // Imie i Nazwisko uzytkownika
        string phoneNumber; // Number telefonu uzytkownika
        Role role; // Przypisana rola (Admin lub Client) [enum class Role]
    public:
        // Tworzenie nowego uzytkownika 
        // Podanie imie i nazwisko
        // Number telfonu
        // Przypisana rola 
        User(string name, string phone, Role r) 
            // nextAutoIncrementId++ (Za kazdym razem o + 1)
            // Imie i nazwisko
            // Numer telefonu
            // Rola 
            : id(nextAutoIncrementId++), fullName(name), phoneNumber(phone), role(r){}

        // Tylko do odczytu danych (const)
        unsigned getIdUser()        const {return id;}
        string getFullNameUser()    const {return fullName;}
        string getPhoneNumberUser() const {return phoneNumber;}
        Role getRole()              const {return role;}
};

class Resource{
    private:
        static unsigned int nextAutoIncrementId; // static unsigned int. Widoczna tylko dla class Reservation. unsigned sprawa zmienną tylko dodatnią 
        unsigned int id; // ID zasobu
        string name; // Nazwa zasobu
        unsigned int capacity; // Ilosc miejsc 
        double pricePerSlot; // cena za miejsce
        bool isActive; // Status aktywnosci (Dostepne / Niedostepne)
    public:
        // Podanie nazwy zasobu
        // Ilosc miejsc 
        // cena za osobe
        Resource(string n, unsigned int cap, double price)
            // nextAutoIncrementId++ (Za kazdym razem o + 1)
            // Nazwa zasobu
            // Ilosc miejsc 
            // Cena za miejsce
            // Danie statusu Dosetpny (isActive(true))
            : id(nextAutoIncrementId++), name(n), capacity(cap), pricePerSlot(price), isActive(true) {} 
        unsigned int getIdResource() const {return id;}
        string getNameResource()     const {return name;}
        unsigned int getCapacity()   const {return capacity;}
        double getPricePerSlot()     const {return pricePerSlot;}
        bool getIsActive()           const {return isActive;}
        
       
        void setIsActive(bool active){isActive = active;}  // Zmana aktywnosci na Dostepny
        void setNewName(string newName){name = newName;}   // Ustawienie nowej nazwy dla zaosbu
        void setPricePerSlot(double newPrice){
            if(newPrice >= 0) pricePerSlot = newPrice; // Ustawienie nowej ceny za osobe
        }
};

// Przypisanie dla kazdej class zmiennej nextAutoIncrementId liczbe 1
unsigned int Reservation::nextAutoIncrementId = 1;
unsigned int User::nextAutoIncrementId        = 1;
unsigned int Resource::nextAutoIncrementId    = 1;

//Czyszczenie terminala 
void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

// Baner początkowy
void header(){
    cout << "==================================================\n"
         << "           SYSTEM REZERWACJI ZASOBOW\n"
         << "==================================================\n";
}

// Opcje logowania 
void logOption(){
    cout << endl;
    cout << "   [1] Zaloguj jako Klient\n";
    cout << "   [2] Zaloguj jako Administrator\n";
    cout << "   [0] Wyjscie z programu\n\n";

    cout << "==================================================\n";
}

// Wybor uzytkownika kto sie loguje (Admin / Klient / Wyjscie)
int userChoiceLog(){
    int choiceUser;
    // Pentla wraz z sprawdzeniem czy uzytkownik uzył podpowiedniej opcji jesli tak zwraca choiceUser 
    while (true) {
        cout << "wybierz opcje > ";
        if (cin >> choiceUser && choiceUser >= 0 && choiceUser <= 2) return choiceUser;
        
        cout << "Nieprawidlowe dane. Sprobuj ponownie." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Funkcje admina
// Panel administatora 
void adminPanel(const User &admin, const Resource &res){
     cout << "==================================================\n"
         << "   PANEL ADMINA | Zalogowany: " << admin.getFullNameUser() << " | ID: " << admin.getIdUser() << '\n'
         << "==================================================" << "\n\n";

    cout << "   --- ZARZĄDZANIE ZASOBAMI ---\n";
    cout << "   [1] Lista wszystkich zasobów\n";
    cout << "   [2] Dodaj nowy zasób\n";
    cout << "   [3] Zmień cenę / dostępność zasobu\n";
    cout << "   [4] Zmien nazwe zasobu\n\n";

    cout << "   --- PRZEGLĄD SYSTEMU Podczas robienia! ---\n";
    cout << "   [5] Pełny harmonogram rezerwacji (Wszystkie)\n";
    cout << "   [6] Wszyscy uzytkownicy\n\n";

    cout << "   [0] Wyloguj / Powrot do ekranu startowego\n\n";
    cout << "==================================================\n";
}

// Wybor pocji admina do adminPanel
int adminChoiceLog(){
    int adminChoice;
    // Pentla spawdzajaca czy admin wyboral odpowiednią opcje z mozliwych 
    while(true){
        cout << "wybierz opcje > ";
        if(cin >> adminChoice && adminChoice >= 0 && adminChoice <=6) return adminChoice; // Jesli poda poprawna odsyl adminChoice

        cout << "Nieprawidlowe dane. Sprobuj ponownie." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Funkcja dodawania noweych zasobow 
void addNewResourcesAndPricing(vector<Resource>& resources){
    clearScreen();
    cout << "==================================================\n"
         << "               NOWE WOLNE REZERWACJE              \n"
         << "==================================================\n\n";
    
    // podanie nowej nazwy z zmiennej newResourcesName
    string newResourcesName;
    cout << "Podaj nazwe zasobu > ";        cin.ignore(); getline(cin, newResourcesName); // cin.ignore() getline(cin, newResourcesName) Pozwala na dodanie spacji w nazwy nowego zasobu
    cout << string(50, '-') << "\n\n";

    unsigned int newResourcesCapacity;
    cout << "Podaj pojemnosc zasobu > ";    cin >> newResourcesCapacity; // Podanie ilosci miejsc
    cout << string(50, '-') << "\n\n";

    double newResourcesPrice;
    cout << "Podaj cene za godzine > ";     cin >> newResourcesPrice; // Podanie ceny za osobe
    clearScreen();

    cout << "==================================================\n"
         << "       TWORZENIE NOWEJ REZERWACJI (Krok 2/2)      \n"
         << "==================================================\n";

    // Sprawdzenie dokładnosci podanych danych 
    cout << left
        << setw(30) << "Nazwa Zasobu" 
        << setw(14) << "Pojemnosc"   
        << setw(15) << "Cena/godz.\n";
    cout << string(59, '-') << "\n";

    cout << left
        << setw(30) <<  newResourcesName  
        << setw(14) <<  newResourcesCapacity  << " os."
        << setw(15) <<  newResourcesPrice << "\n";
    cout << string(59, '-') << "\n\n";

    // Potwierdzenie dodania nowej rezerwacji do vector<Resources> resources
    string checkAddNewResources;
    cout << "Czy chcesz utworzyc nowa rezerwacje? [T]/[N] > ";
    cin.ignore();
    getline(cin, checkAddNewResources);
    // Jesli T
    if(checkAddNewResources == "T" || checkAddNewResources == "t"){
        // Tworzenie nowej struktury Resource newRes 
        // Podanie Nazwy, ilosc miejsc, ceny za miejsce
        Resource newRes(newResourcesName, newResourcesCapacity, newResourcesPrice);
        // Przeslanie danych do vecotr
        resources.push_back(newRes);
    }
    else{
        return;
    }
}

// Funkcja zmiany ceny i statusu zasobu
void changePriceAndStatus(vector<Resource>& resources, bool isActive = false){
    // Wybor ID do zmian
    int choiceIDToChange;
    cout << "Podaj id zasobu do zmiany > "; cin >> choiceIDToChange;

    // Ustawienie target jak nullptr
    Resource* target = nullptr;
    for(auto& res : resources){
        // Sprawdzanie i szukanie ID zasobu takiego samego jak podal uzytkownik
        if(static_cast<int>(res.getIdResource()) == choiceIDToChange) // static_cast<int> -> ustawienie zmiennych na tylko int
        {
            // Jesli jest aktywne target na miec to samo ID co res
            if(isActive && !res.getIsActive())
                continue;
            target = &res;
            break;
        }
    }

    // Sprawdzenie jeśli nie znaleziono danego ID
    if(!target){
        clearScreen();
        cout << "Nie znaleziono zasobu o ID: " << choiceIDToChange << "\n";
        return;
    }

    clearScreen();
    cout << "==================================================\n"
         << "               ZMIANA ZASOBU PO ID " << choiceIDToChange << "\n"
         << "==================================================\n";

    cout << left
         << setw(7)  << "ID"
         << setw(30) << "Nazwa Zasobow"
         << setw(14) << "Pojemnosc"
         << setw(15) << "Cena/godz."
         << "Status" << "\n";
    cout << string(78, '-') << "\n";

    string capStr = to_string(target->getCapacity()) + " os.";
    cout << left
         << setw(7)  << target->getIdResource()   // ID wyszukanego zasobu
         << setw(30) << target->getNameResource() // Nazwa wyszukanego zasobu
         << setw(14) << capStr // Ilosc miejsc 
         << setw(15) << target->getPricePerSlot() // Cena za miejsce 
         << (target->getIsActive() ? "[Dostepny]" : "[Niedostepny]") << "\n"; // Status zasobu
    cout << "==============================================================================\n\n";

    // Wybor co do zmiany
    int choiceWithChange;
    cout << "   [1] Zmien cene\n";
    cout << "   [2] Zmien status\n\n";
    cout << string(50, '-') << "\n";
    cout << "Wybierz opcje zmiany > ";  cin >> choiceWithChange;

    switch(choiceWithChange){
        case 1:
            double newPrice;
            cout << "Podaj nowa cene za godzine > ";    cin >> newPrice;
                if(newPrice >= 0){
                    // Przypisanie nowej ceny 
                    target->setPricePerSlot(newPrice);
                    cout << "Cena zostala zmieniona.\n";
                } 
                else {
                    cout << "Cena nie moze byc ujemna.\n";
                }
            break;
        case 2:
            char statusChoice;
            cout << "Ustaw status [D]ostepny / [N]iedostepny > ";   cin >> statusChoice;
                if(statusChoice == 'D' || statusChoice == 'd'){
                    // Przypisanie statusu dostepne
                    target->setIsActive(true);
                    cout << "Zasob ustawiony jako dostepny.\n";
                } 
                else if(statusChoice == 'N' || statusChoice == 'n'){
                    // Przypisanie statusu niedostepne
                    target->setIsActive(false);
                    cout << "Zasob ustawiony jako niedostepny.\n";
                } 
                else {
                    cout << "Nieprawidlowy wybor.\n";
                }
            break;
        default:
            cout << "Nieprawidlowa opcja.\n";
        break;
    }
}

// Funkcja zmiany nazwy zasobu
void changeNameInResources(vector<Resource>& resources,  bool isActive = false){
    int choiceIDToChangeName;
    cout << "Podaj id do zmiany nazwy > ";
    cin >> choiceIDToChangeName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Stworzenie ptr
    Resource* target = nullptr;
    // Pentla z if czy ID zasobu jest taki sam jak podany przez uzytkownika
    for(auto& res : resources){
        if(static_cast<int>(res.getIdResource()) == choiceIDToChangeName){
            if(isActive && !res.getIsActive())
                continue;
            target = &res;
            break;
        }
    }

    // Jesli nie ma takiego ID powrot do menu
    if(!target){
        clearScreen();
        cout << "Nie ma takiego ID jak: " << choiceIDToChangeName << '\n';
        cout << string(50, '=');
        return;
    }

    clearScreen();
    cout << "==================================================\n"
         << "               ZMIANA nazwy w ID " << choiceIDToChangeName << "\n"
         << "==================================================\n";

    cout << left   
         << setw(7)  << "ID"
         << setw(30) << "Nazwa Zasobu"
         << setw(14) << "Pojemnosc"
         << setw(15) << "Cena/godz."
         << "status\n";
    cout << string(78, '-') << "\n";

    // Wyswitelenie wszystkich danych o podanym zasobie z ID
    string capStr = to_string(target->getCapacity()) + " os.";
    cout << left    
         << setw(7)  << target->getIdResource()
         << setw(30) << target->getNameResource()
         << setw(14) << capStr
         << setw(15) << target->getCapacity()
         << (target->getIsActive() ? "[Dostepny]" : "Niedostepny") << "\n";
    cout << string(78, '+');

    // Podanie nowej nazwy
    string newName;
    cout << "\nPodaj nowa nazwe zasobu > ";
    getline(cin, newName); // String z spacją
    if(newName.empty()){
        cout << "Nazwa nie moze byc pusta.\n";
        return;
    }
    // Podmiana nazwy
    target->setNewName(newName);

    clearScreen();
    cout << "Zmiana sie powiadla\n";
    return;   
}

void displayUsers(vector<User>& users){
    clearScreen();
    cout << "==================================================\n"
         << "                   Uzytkownicy                    \n"
         << "==================================================\n\n";
    if(users.empty()){
        cout << "Nie ma zadnego uzytwkonika\n";
        return;
    }

    cout << left    
         << setw(7)  << "ID"
         << setw(20) << "Imie i Nazwisko"
         << setw(14) << "Number"
         << setw(15) << "Przypisana Rola\n";
    cout << string(58, '=') << "\n\n";

    for(auto& usr : users){
        cout << string(58, '-') << "\n\n";
        cout << left    
             << setw(7)  << usr.getIdUser()
             << setw(20) << usr.getFullNameUser()
             << setw(14) << usr.getPhoneNumberUser()
             << setw(15) << (usr.getRole() == Role::ADMIN ? "   ADMIN" : "  CLIENT") << "\n\n";
        cout << string(58, '-') << "\n\n";
    }

    cout << "Nacisnij enter aby wyjsc" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string formatTimestamp(time_t timestamp);
string reservationStatusToString(ReservationStatus status);
const Resource* findResourceById(const vector<Resource>& resources, unsigned int resourceId);

void bookingSchedule(const vector<Reservation>& reservations, const vector<Resource>& resources){
    clearScreen();
    int numberOfReservations = 0;
    cout << "==================================================\n"
        << "               Harmonogtam Rezerwacji             \n"
        << "==================================================\n\n";
    // Pentla do wyswietlenie wszystkich rezerwacji z statusem PEDING
    for(const auto& res : reservations){
        // Wyswitelenie wszystkich rezerwacji z statusuem PEDING
        if(reservationStatusToString(res.getStatus()) == "PENDING"){
            numberOfReservations++;
            const Resource* resource = findResourceById(resources, res.getResourceId());
              cout << string(50, '-') << "\n\n"
                  << "   Pozycja rezerwacji: " << numberOfReservations << "\n"
                  << "   ID rezerwacji: " << res.getId() << "\n"
                 << "   Zasob: " << (resource ? resource->getNameResource() : string("Nieznany")) << "\n"
                 << "   Data pocz.: " << formatTimestamp(res.getStartTimestamp()) << "\n"
                 << "   Data zak.: " << formatTimestamp(res.getEndTimestamp()) << "\n"
                 << "   Miejsce: " << res.getReservedSeats() << "\n"
                 << "   Status: " << reservationStatusToString(res.getStatus()) << "\n"
                  << "   Cena: " << res.getTotalPrice() << "\n"
                  << string(50, '-') << "\n\n";
        }
        // Sprawdzenie jeśli nie ma zasobu
        if(numberOfReservations == 0){
            cout << "Nie ma zadnej rezerwacji \n";
            cout << string(50, '=') << "\n\n";
            return;
        }
    }
}

// Funkcje Klienta
// Panel klienta
void clientPanel(const User &client, const Resource &res){
    // Wyświetlenie mozliwych opcji dla klienta
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

// Wybor pocji klienta do clientPanel
int clientChoiceLog(){
    int clientChoiceUser;
    // Pentla czy klient wybral dobra opcje w zakresie 0 - 4
    while (true) {
        cout << "wybierz opcje > ";
        if (cin >> clientChoiceUser && clientChoiceUser >= 0 && clientChoiceUser <= 4) return clientChoiceUser;
        
        cout << "Nieprawidlowe dane. Sprobuj ponownie" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Wyswietlenie wszywkich zasobow 
void displayResourcesAndPricing(const vector<Resource>& resources, bool isActive = false){ 
    cout << "==============================================================================\n"
         << "                           LISTA ZASOBOW I CENNIK                             \n"
         << "==============================================================================\n";

    // Komunikat o braku zasobow
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
    // Pentla z sprawdzeniem czy dany zasob jest dostepny
    for(const auto& res : resources){
        if(isActive && !res.getIsActive())
            continue;
        string capStr = to_string(res.getCapacity()) + " os.";
        // Wyswietlenie wszystkich danych
        cout << left
            <<setw(7)  << res.getIdResource()
            <<setw(30) << res.getNameResource()
            <<setw(14) << capStr
            <<setw(15) << res.getPricePerSlot();
        
        cout << left
            << (res.getIsActive() ? "[Dostepny]" : "[Niedotepny]") << "\n"; 
    }
    cout << "==============================================================================\n\n";
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
// AI
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

// Wyswietlenie rezerwacji uzytkownika
void displayUserReservations(const vector<Reservation>& reservations, const vector<Resource>& resources, unsigned int userId) {
    clearScreen();
    cout << "==================================================\n"
         << "                TWOJE REZERWACJE                  \n"
         << "==================================================\n";

    bool hasReservation = false;
    // Pentla wraz z sprawdzeniem czy ID uzytkownika jest taki sam jak ID przy rezerwacji
    for(const auto& res : reservations){
        // Dodatkowe sprawdzenie czy status jest PENDING
        if(res.getUserId() != userId && reservationStatusToString(res.getStatus()) == "PENDING") continue;
            hasReservation = true;
            const Resource* resource = findResourceById(resources, res.getResourceId());
            // Wyswietlenie wszystkich danych o rezerwacji
            cout << "   ID rezerwacji: " << res.getId() << "\n"
                 << "   Zasob: " << (resource ? resource->getNameResource() : string("Nieznany")) << "\n"
                 << "   Data pocz.: " << formatTimestamp(res.getStartTimestamp()) << "\n"
                 << "   Data zak.: " << formatTimestamp(res.getEndTimestamp()) << "\n"
                 << "   Miejsce: " << res.getReservedSeats() << "\n"
                 << "   Status: " << reservationStatusToString(res.getStatus()) << "\n" 
                 << "   Cena: " << res.getTotalPrice() <<  "\n" 
                 << "--------------------------------------------------\n";
    }
    // Komunkat gdyby nie bylo zadnej rezerwacji
    if(!hasReservation){
        cout << "Brak aktywnych rezerwacji dla Twojego konta.\n";
        cout << "Nacisnij Enter, aby kontynuowac";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

// Tworzenie nowej rezerwacji
void newReservation(const User& user, const vector<Resource>& resources, vector<Reservation>& reservations){
    unsigned int resID; 
    cout << "==================================================\n"
         << "           TWORZENIE NOWEJ REZERWACJI             \n"
         << "==================================================\n";
    
    cout << "Podaj id > ";  cin >> resID;
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
          << "==================================================\n\n";

    bool isActiveReservation = false;
    for(const auto& res : reservation){

        if(res.getUserId() == user.getIdUser() && res.getStatus() != ReservationStatus::CANCELLED){
            isActiveReservation = true;
            cout << string(50, '-') << "\n\n";
            cout << "   ID: " << res.getId() << "\n"
                 << "   ID Zasobu: " << res.getResourceId() << "\n"
                 << "   ID Miejsca: " << res.getReservedSeats() << "\n"
                 << "   Koszt: " << res.getTotalPrice() << "\n\n";
            cout << string(50, '-') << "\n\n";
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
    clearScreen();
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
                if(clientChoiceUser == 0)   break;
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
                        displayResourcesAndPricing(resources, false);
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
            while(true){
                clearScreen();
                adminPanel(users[0], resources[0]);
                int adminChoice =  adminChoiceLog();
                if(adminChoice == 0)    break;
                switch (adminChoice){
                    case 1:
                        int backToAdminPanel;
                        clearScreen();
                        displayResourcesAndPricing(resources, false);
                        cout << "[0] Powrot > "; cin >> backToAdminPanel;
                        if(backToAdminPanel == 0){
                            adminPanel(users[0], resources[0]);
                        }
                        break;
                    case 2:
                        addNewResourcesAndPricing(resources);
                        break;
                    case 3:
                        clearScreen();
                        displayResourcesAndPricing(resources, false);
                        changePriceAndStatus(resources, false);
                        break;
                    case 4:
                        clearScreen();
                        displayResourcesAndPricing(resources, false);
                        changeNameInResources(resources, false);
                        break;
                
                    case 5:
                        clearScreen();
                        bookingSchedule(reservations, resources);
                        cout << "[0] Powrot > "; cin >> backToAdminPanel;
                        if(backToAdminPanel == 0){
                            adminPanel(users[0], resources[0]);
                        }
                        break;
                    case 6: 
                        clearScreen();
                        displayUsers(users);
                    break;
                }
            }
        }
        else{
            clearScreen();     
            return 0;
        }
    }
}