#include <iostream>  // For input/output operations (cin, cout)
#include <vector>    // For using vector data structure
#include <fstream>   // For file operations (reading/writing files)
#include <algorithm> // For find_if function
using namespace std; // So we don't need to write std:: before standard functions

// ----- Structs (No Functions) -----

// Structure to store event information
struct Event {
    int id;           // Unique ID for the event
    string name;      // Name of the event
    string location;  // Where the event is happening
    string date;      // Date of the event
    string time;      // Time of the event
    float price;      // Price per ticket
    int totalSeats;   // Total seats available
    int bookedSeats;  // Seats already booked
};

// Structure to store customer information
struct Customer {
    string name;        // Customer's name
    string cnic;        // Customer's CNIC (unique ID)
    string contact;     // Customer's contact info
    string bookedEvent; // Name of event they booked
    int eventId;        // ID of the booked event
    int seatsBooked;    // Number of seats booked
    bool paymentDone;   // Whether payment is completed
    string bookingDate; // Date when booking was made
};

// ----- Global Vectors -----
vector<Event> events;     // Stores all events in the system
vector<Customer> customers; // Stores all customers in the system

// ----- Utility Functions -----

// Returns how many seats are still available for an event
int getRemainingSeats(Event e) {
    return e.totalSeats - e.bookedSeats; // Simple subtraction
}

// Updates the booked seats count for an event
void bookSeats(Event &e, int seats) {
    e.bookedSeats += seats; // Adds the new seats to existing booked seats
}

// ----- File Handling -----

// Saves all events to a file
void saveEvents() {
    ofstream out("event_data.txt"); // Open file for writing
    for (auto& e : events) { // Loop through all events
        // Write each field separated by newlines
        out << e.id << '\n' << e.name << '\n' << e.location << '\n' 
            << e.date << '\n' << e.time << '\n' << e.price << '\n' 
            << e.totalSeats << '\n' << e.bookedSeats << '\n';
    }
    out.close(); // Close the file
}

// Loads events from file into memory
void loadEvents() {
    ifstream in("event_data.txt"); // Open file for reading
    events.clear(); // Clear existing events
    Event e; // Temporary event object
    while (in >> e.id) { // Read ID (if successful, continue)
        in.ignore(); // Ignore the newline after ID
        getline(in, e.name); // Read name (can contain spaces)
        getline(in, e.location); // Read location
        getline(in, e.date); // Read date
        getline(in, e.time); // Read time
        in >> e.price >> e.totalSeats >> e.bookedSeats; // Read numbers
        in.ignore(); // Ignore newline after numbers
        events.push_back(e); // Add to events vector
    }
    in.close(); // Close the file
}

// Similar to saveEvents but for customers
void saveCustomers() {
    ofstream out("customer_data.txt");
    for (auto& c : customers) {
        out << c.name << '\n' << c.cnic << '\n' << c.contact << '\n' 
            << c.bookedEvent << '\n' << c.eventId << '\n' 
            << c.seatsBooked << '\n' << c.paymentDone << '\n' 
            << c.bookingDate << '\n';
    }
    out.close();
}

// Similar to loadEvents but for customers
void loadCustomers() {
    ifstream in("customer_data.txt");
    customers.clear();
    Customer c;
    while (getline(in, c.name)) { // Read name line
        getline(in, c.cnic);      // Then CNIC
        getline(in, c.contact);   // Then contact
        getline(in, c.bookedEvent); // Then event name
        in >> c.eventId >> c.seatsBooked >> c.paymentDone; // Read numbers/bool
        in.ignore(); // Ignore newline
        getline(in, c.bookingDate); // Read booking date
        customers.push_back(c); // Add to customers vector
    }
    in.close();
}

// ----- Display Functions -----

// Shows all events in the system
void displayAllEvents() {
    cout << "\n--- All Events ---\n";
    if (events.empty()) { // Check if no events exist
        cout << "No events available.\n";
        return;
    }
    // Loop through and display each event's details
    for (auto& e : events) {
        cout << "ID: " << e.id << ", Name: " << e.name 
             << ", Venue: " << e.location << ", Date: " << e.date 
             << ", Time: " << e.time << ", Price: " << e.price
             << ", Booked: " << e.bookedSeats << ", Available: " 
             << getRemainingSeats(e) << '\n';
    }
}

// Shows all customers in the system
void displayAllCustomers() {
    cout << "\n--- All Customers ---\n";
    if (customers.empty()) { // Check if no customers exist
        cout << "No customers registered.\n";
        return;
    }
    // Loop through and display each customer's details
    for (auto& c : customers) {
        cout << "Name: " << c.name << ", CNIC: " << c.cnic 
             << ", Contact: " << c.contact << ", Event: " << c.bookedEvent
             << ", Seats: " << c.seatsBooked << ", Paid: " 
             << (c.paymentDone ? "Yes" : "No") << ", Booking Date: " 
             << c.bookingDate << '\n';
    }
}

// ----- Logic Functions -----

// Checks if a CNIC is unique (not already registered)
bool isUniqueCNIC(const string& cnic) {
    for (auto& c : customers) { // Check all customers
        if (c.cnic == cnic) // If CNIC matches any existing
            return false; // Not unique
    }
    return true; // If no matches found, it's unique
}

// Finds the index of an event in the vector by its ID
int findEventIndexByID(int id) {
    for (int i = 0; i < (int)events.size(); i++) { // Loop through events
        if (events[i].id == id) // If ID matches
            return i; // Return the index
    }
    return -1; // If not found, return -1
}

// Registers a new customer
void registerCustomer() {
    Customer c; // Create new customer object
    cout << "Enter Name: ";
    getline(cin, c.name); // Read full name (may contain spaces)
    cout << "Enter CNIC: ";
    getline(cin, c.cnic);
    if (!isUniqueCNIC(c.cnic)) { // Check if CNIC already exists
        cout << "Customer with this CNIC already exists!\n";
        return;
    }
    cout << "Enter Contact: ";
    getline(cin, c.contact);
    // Set default values for new customer
    c.eventId = 0;
    c.seatsBooked = 0;
    c.paymentDone = false;
    customers.push_back(c); // Add to customers vector
    saveCustomers(); // Save to file
    cout << "Customer Registered Successfully.\n";
}

// Adds a new event to the system
void addEvent() {
    Event e; // Create new event object
    cout << "Enter Event ID: ";
    cin >> e.id; // Read ID (number)
    cin.ignore(); // Clear newline from buffer
    cout << "Enter Event Name: ";
    getline(cin, e.name); // Read name (may contain spaces)
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
    e.bookedSeats = 0; // Initialize booked seats to 0
    cin.ignore(); // Clear newline from buffer
    events.push_back(e); // Add to events vector
    saveEvents(); // Save to file
    cout << "Event Added Successfully.\n";
}

// Handles booking an event for a customer
void bookEvent() {
    string cnic;
    int eventId, seats;

    cout << "Enter your CNIC: ";
    getline(cin, cnic);

    // Find customer by CNIC
    auto custIt = find_if(customers.begin(), customers.end(), 
        [&](const Customer& c) { return c.cnic == cnic; });
    
    if (custIt == customers.end()) { // If customer not found
        cout << "Customer not registered. Please register first.\n";
        return;
    }

    cout << "Enter Event ID: ";
    cin >> eventId;
    cin.ignore();
    int idx = findEventIndexByID(eventId); // Find event by ID
    if (idx == -1) { // If event not found
        cout << "Event not found!\n";
        return;
    }

    cout << "Enter number of tickets (max 5): ";
    cin >> seats;
    cin.ignore();
    // Validate seat request
    if (seats <= 0 || seats > 5 || seats > getRemainingSeats(events[idx])) {
        cout << "Invalid or insufficient seats.\n";
        return;
    }

    // Update booking information
    bookSeats(events[idx], seats); // Update event's booked seats
    custIt->bookedEvent = events[idx].name; // Set event name
    custIt->eventId = eventId; // Set event ID
    custIt->seatsBooked = seats; // Set seats booked
    custIt->paymentDone = false; // Payment not done yet
    custIt->bookingDate = "2025-05-19"; // Hardcoded date (would use real date in real app)

    saveEvents(); // Save updated events
    saveCustomers(); // Save updated customers

    cout << "Booking Successful!\n";
}

// Handles payment for a booking
void payForBooking() {
    string cnic;
    cout << "Enter CNIC for payment: ";
    getline(cin, cnic);

    // Find customer by CNIC
    auto custIt = find_if(customers.begin(), customers.end(), 
        [&](const Customer& c) { return c.cnic == cnic; });
    
    if (custIt == customers.end()) { // If customer not found
        cout << "Customer not found.\n";
        return;
    }
    if (custIt->paymentDone) { // If already paid
        cout << "Payment already done.\n";
        return;
    }
    custIt->paymentDone = true; // Mark as paid
    saveCustomers(); // Save updated data
    cout << "Payment successful. Status updated to Paid.\n";
}

// ----- Main Program -----
int main() {
    // Load existing data when program starts
    loadEvents();
    loadCustomers();

    int choice;
    do {
        // Display menu
        cout << "\n--- Event Management System ---\n";
        cout << "1. Add Event\n2. Register Customer\n3. Book Event\n"
             << "4. Pay for Booking\n5. Show All Events\n6. Show All Customers\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        // Process user choice
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
    } while (choice != 7); // Continue until user chooses to exit

    return 0; // End program
}
