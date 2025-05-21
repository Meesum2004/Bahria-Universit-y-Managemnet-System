#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

// ----- Structs (No Functions) -----

struct Event {
    int id;
    string name;
    string location;
    string date;
    string time;
    float price;
    int totalSeats;
    int bookedSeats;
};

struct Customer {
    string name;
    string cnic;
    string contact;
    string bookedEvent;
    int eventId;
    int seatsBooked;
    bool paymentDone;
    string bookingDate;
};

// ----- Global Vectors -----
vector<Event> events;
vector<Customer> customers;

// ----- Utility Functions -----

int getRemainingSeats(Event e) {
    return e.totalSeats - e.bookedSeats;
}

void bookSeats(Event &e, int seats) {
    e.bookedSeats += seats;
}

// ----- File Handling -----

void saveEvents() {
    ofstream out("event_data.txt");
    for (auto& e : events) {
        out << e.id << '\n' << e.name << '\n' << e.location << '\n' << e.date << '\n' << e.time << '\n'
            << e.price << '\n' << e.totalSeats << '\n' << e.bookedSeats << '\n';
    }
    out.close();
}

void loadEvents() {
    ifstream in("event_data.txt");
    events.clear();
    Event e;
    while (in >> e.id) {
        in.ignore();
        getline(in, e.name);
        getline(in, e.location);
        getline(in, e.date);
        getline(in, e.time);
        in >> e.price >> e.totalSeats >> e.bookedSeats;
        in.ignore();
        events.push_back(e);
    }
    in.close();
}

void saveCustomers() {
    ofstream out("customer_data.txt");
    for (auto& c : customers) {
        out << c.name << '\n' << c.cnic << '\n' << c.contact << '\n' << c.bookedEvent << '\n'
            << c.eventId << '\n' << c.seatsBooked << '\n' << c.paymentDone << '\n' << c.bookingDate << '\n';
    }
    out.close();
}

void loadCustomers() {
    ifstream in("customer_data.txt");
    customers.clear();
    Customer c;
    while (getline(in, c.name)) {
        getline(in, c.cnic);
        getline(in, c.contact);
        getline(in, c.bookedEvent);
        in >> c.eventId >> c.seatsBooked >> c.paymentDone;
        in.ignore();
        getline(in, c.bookingDate);
        customers.push_back(c);
    }
    in.close();
}

// ----- Display Functions -----

void displayAllEvents() {
    cout << "\n--- All Events ---\n";
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }
    for (auto& e : events) {
        cout << "ID: " << e.id << ", Name: " << e.name << ", Venue: " << e.location
             << ", Date: " << e.date << ", Time: " << e.time << ", Price: " << e.price
             << ", Booked: " << e.bookedSeats << ", Available: " << getRemainingSeats(e) << '\n';
    }
}

void displayAllCustomers() {
    cout << "\n--- All Customers ---\n";
    if (customers.empty()) {
        cout << "No customers registered.\n";
        return;
    }
    for (auto& c : customers) {
        cout << "Name: " << c.name << ", CNIC: " << c.cnic << ", Contact: " << c.contact
             << ", Event: " << c.bookedEvent << ", Seats: " << c.seatsBooked
             << ", Paid: " << (c.paymentDone ? "Yes" : "No") << ", Booking Date: " << c.bookingDate << '\n';
    }
}

// ----- Logic Functions -----

bool isUniqueCNIC(const string& cnic) {
    for (auto& c : customers) {
        if (c.cnic == cnic)
            return false;
    }
    return true;
}

int findEventIndexByID(int id) {
    for (int i = 0; i < (int)events.size(); i++) {
        if (events[i].id == id)
            return i;
    }
    return -1;
}

void registerCustomer() {
    Customer c;
    cout << "Enter Name: ";
    getline(cin, c.name);
    cout << "Enter CNIC: ";
    getline(cin, c.cnic);
    if (!isUniqueCNIC(c.cnic)) {
        cout << "Customer with this CNIC already exists!\n";
        return;
    }
    cout << "Enter Contact: ";
    getline(cin, c.contact);
    c.eventId = 0;
    c.seatsBooked = 0;
    c.paymentDone = false;
    customers.push_back(c);
    saveCustomers();
    cout << "Customer Registered Successfully.\n";
}

void addEvent() {
    Event e;
    cout << "Enter Event ID: ";
    cin >> e.id;
    cin.ignore();
    cout << "Enter Event Name: ";
    getline(cin, e.name);
    cout << "Enter Venue: ";
    getline(cin, e.location);
    cout << "Enter Date: ";
    getline(cin, e.date);
    cout << "Enter Time: ";
    getline(cin, e.time);
    cout << "Enter Ticket Price: ";
    cin >> e.price;
    cout << "Enter Total Seats: ";
    cin >> e.totalSeats;
    e.bookedSeats = 0;
    cin.ignore();
    events.push_back(e);
    saveEvents();
    cout << "Event Added Successfully.\n";
}

void bookEvent() {
    string cnic;
    int eventId, seats;

    cout << "Enter your CNIC: ";
    getline(cin, cnic);

    auto custIt = find_if(customers.begin(), customers.end(), [&](const Customer& c) { return c.cnic == cnic; });
    if (custIt == customers.end()) {
        cout << "Customer not registered. Please register first.\n";
        return;
    }

    cout << "Enter Event ID: ";
    cin >> eventId;
    cin.ignore();
    int idx = findEventIndexByID(eventId);
    if (idx == -1) {
        cout << "Event not found!\n";
        return;
    }

    cout << "Enter number of tickets (max 5): ";
    cin >> seats;
    cin.ignore();
    if (seats <= 0 || seats > 5 || seats > getRemainingSeats(events[idx])) {
        cout << "Invalid or insufficient seats.\n";
        return;
    }

    bookSeats(events[idx], seats);
    custIt->bookedEvent = events[idx].name;
    custIt->eventId = eventId;
    custIt->seatsBooked = seats;
    custIt->paymentDone = false;
    custIt->bookingDate = "2025-05-19"; // Replace with real date if needed

    saveEvents();
    saveCustomers();

    cout << "Booking Successful!\n";
}

void payForBooking() {
    string cnic;
    cout << "Enter CNIC for payment: ";
    getline(cin, cnic);

    auto custIt = find_if(customers.begin(), customers.end(), [&](const Customer& c) { return c.cnic == cnic; });
    if (custIt == customers.end()) {
        cout << "Customer not found.\n";
        return;
    }
    if (custIt->paymentDone) {
        cout << "Payment already done.\n";
        return;
    }
    custIt->paymentDone = true;
    saveCustomers();
    cout << "Payment successful. Status updated to Paid.\n";
}

// ----- Main -----

int main() {
    loadEvents();
    loadCustomers();

    int choice;
    do {
        cout << "\n--- Event Management System ---\n";
        cout << "1. Add Event\n2. Register Customer\n3. Book Event\n4. Pay for Booking\n5. Show All Events\n6. Show All Customers\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addEvent(); break;
            case 2: registerCustomer(); break;
            case 3: bookEvent(); break;
            case 4: payForBooking(); break;
            case 5: displayAllEvents(); break;
            case 6: displayAllCustomers(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 7);

    return 0;
}
