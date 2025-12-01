
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
using namespace std;

class Vehicle {
public:
    string vehicleNumber;
    string vehicleType;
    string ownerName;
    time_t entryTime;

    Vehicle(string number, string type, string owner) {
        vehicleNumber = number;
        vehicleType = type;
        ownerName = owner;
        entryTime = time(0);
    }
};

class ParkingSlot {
public:
    int slotID;
    bool isOccupied;
    Vehicle* vehicle;

    ParkingSlot(int id) {
        slotID = id;
        isOccupied = false;
        vehicle = nullptr;
    }
};

class ParkingLot {
private:
    vector<ParkingSlot> slots;

public:
    ParkingLot(int totalSlots) {
        for (int i = 1; i <= totalSlots; ++i) {
            slots.push_back(ParkingSlot(i));
        }
        loadFromFile();
    }

    ~ParkingLot() {
        saveToFile();
        for (auto &slot : slots) {
            delete slot.vehicle;
        }
    }

    void parkVehicle() {
        string number, type, owner;
        cout << "Enter Vehicle Number: "; cin >> number;
        cout << "Enter Vehicle Type: "; cin >> type;
        cout << "Enter Owner Name: "; cin >> owner;

        for (auto &slot : slots) {
            if (!slot.isOccupied) {
                slot.vehicle = new Vehicle(number, type, owner);
                slot.isOccupied = true;
                cout << "Vehicle parked at slot " << slot.slotID << endl;
                logToFile("Parked", *slot.vehicle, slot.slotID);
                return;
            }
        }
        cout << "No available slots!\n";
    }

    void removeVehicle() {
        string number;
        cout << "Enter Vehicle Number to Remove: "; cin >> number;
        for (auto &slot : slots) {
            if (slot.isOccupied && slot.vehicle->vehicleNumber == number) {
                logToFile("Removed", *slot.vehicle, slot.slotID);
                delete slot.vehicle;
                slot.vehicle = nullptr;
                slot.isOccupied = false;
                cout << "Vehicle removed from slot " << slot.slotID << endl;
                return;
            }
        }
        cout << "Vehicle not found!\n";
    }

    void displaySlots() {
        for (auto &slot : slots) {
            cout << "Slot " << slot.slotID << ": ";
            if (slot.isOccupied)
                cout << "Occupied by " << slot.vehicle->vehicleNumber << endl;
            else
                cout << "Available" << endl;
        }
    }

    void saveToFile() {
        ofstream file("parking_data.txt");
        for (auto &slot : slots) {
            if (slot.isOccupied) {
                file << slot.slotID << ","
                     << slot.vehicle->vehicleNumber << ","
                     << slot.vehicle->vehicleType << ","
                     << slot.vehicle->ownerName << ","
                     << slot.vehicle->entryTime << "\n";
            }
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("parking_data.txt");
        string number, type, owner;
        int slotID;
        time_t entry;
        string line;
        while (getline(file, line)) {
            sscanf(line.c_str(), "%d,%[^,],%[^,],%[^,],%lld",
                   &slotID, &number[0], &type[0], &owner[0], &entry);
            Vehicle* v = new Vehicle(number, type, owner);
            v->entryTime = entry;
            slots[slotID - 1].vehicle = v;
            slots[slotID - 1].isOccupied = true;
        }
        file.close();
    }

    void logToFile(const string& action, Vehicle& v, int slotID) {
        ofstream log("parking_log.txt", ios::app);
        log << action << ", Slot: " << slotID
            << ", Number: " << v.vehicleNumber
            << ", Type: " << v.vehicleType
            << ", Owner: " << v.ownerName
            << ", Time: " << ctime(&v.entryTime);
        log.close();
    }
};

int main() {
    ParkingLot lot(5); 
    int choice;
    while (true) {
        cout << "\n--- Parking Management Menu ---\n";
        cout << "1. Park a vehicle\n2. Remove a vehicle\n3. Display slots\n4. Exit\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: lot.parkVehicle(); break;
            case 2: lot.removeVehicle(); break;
            case 3: lot.displaySlots(); break;
            case 4: return 0;
            default: cout << "Invalid choice!\n";
        }
    }
    return 0;
}