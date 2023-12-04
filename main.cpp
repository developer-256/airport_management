// password: hello
#include <iostream>
#include <conio.h> // to use _getch in getPassword
#include <string>  // to use push and pop in getPassword
#include <fstream> // for file handeling
using namespace std;

class Employee
{
private:
    string name;
    string ID;
    string designation;

public:
    Employee(string n = "", string id = "", string d = "") { name = n, ID = id, designation = d; }
    string getName() { return name; }
    string getID() { return ID; }
    string getDesignation() { return designation; }
};

class Flight
{
private:
    string origin;
    string destination;
    string departure;
    string arrival;
    string flighCode;
    string price;
    string totalSeats;
    static int bookedSeats;

public:
    Flight(string o = "", string dest = "", string dep = "", string a = "", string f = "", string ts = "", string p = "")
    {
        origin = o;
        destination = dest;
        departure = dep;
        arrival = a;
        flighCode = f;
        price = p;
        totalSeats = ts;
        bookedSeats++;
    }
    string getOrigin() { return origin; }
    string getDestination() { return destination; }
    string getDeparture() { return departure; }
    string getArrival() { return arrival; }
    string getFlighCode() { return flighCode; }
    string getPrice() { return price; }
    string getTotalSeats() { return totalSeats; }
    int getBookedSeats() { return bookedSeats; }
};
int Flight::bookedSeats = 0;

class Passenger
{
private:
    string name;
    string id;
    string flightID;

public:
};

// %%%%%%%%%%%%%%%%%%%%% (backend start) %%%%%%%%%%%%%%%%%%%%%
class BackendAccess
{
private:
    int rows;     // total rows of data
    int Capacity; // data rows + empty rows
    int cols;

public:
    string **_2Darray;
    BackendAccess(int cl, string fileName);
    ~BackendAccess();
    int getRows() { return rows; }
    void setRows(int r) { rows = r; }
    int getCols() { return cols; }
    int getCapacity() { return Capacity; }
    void exportData(string fileName);
    void writeEmployee(string name, string id, string designation, string fileName);
    void writeFlight(string o, string dest, string dep, string a, string f, string s, string p, string fileName);
};
BackendAccess::BackendAccess(int cl, string fileName) : Capacity(10), rows(0), cols(cl)
{
    // initializing 2D array
    _2Darray = new string *[Capacity];
    for (int i = 0; i < Capacity; i++)
    {
        _2Darray[i] = new string[cols];
    }

    ifstream file; // it must be ifstream not fstream
    file.open(fileName);
    while (!file.eof()) // reading file till end of file
    {
        string temp_row = "";
        getline(file, temp_row); // take line 1 of file as input in temp_row
        if (temp_row == "")      // if it is empty row you will skip to next row
        {
            rows++;
            continue;
        }
        int column = 0; // this is used to enter in every column of row in 2D array then 0 in start of next row
        string temp_col = "";
        for (int i = 0; i < temp_row.length(); i++) // breaking row that we input into different values
        {
            if (temp_row[i] != ',') // if , does not occur keep adding character in temp_row
            {
                temp_col += temp_row[i];
            }
            else // if , occur save temp_row into 2D array
            {
                _2Darray[rows][column] = temp_col;
                temp_col = ""; // empty temp_col
                column++;
            }
        }
        rows++;

        // if capacity is full first increase the capacity, then copy previous array to new array with increased size, then empty previous array, then pass ownership of new array to old one, then perform the tasks.
        if (rows == Capacity)
        {
            rows--; // because we are going to copy

            // increasing capacity
            Capacity *= 2;

            // make a new 2Darray with increased capacity
            string **new2Darray = new string *[Capacity];
            for (int i = 0; i < Capacity; i++)
            {
                new2Darray[i] = new string[cols];
            }

            // copy previous array to new one
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    new2Darray[i][j] = _2Darray[i][j];
                }
            }

            // delete everything from previous array
            for (int i = 0; i < rows; i++)
            {
                delete[] _2Darray[i];
            }
            delete[] _2Darray;

            // transfer the ownership of new array to old array so that we don't have to change in our code to perform functions on new array. We can just make new array transfer ownership to old array and make changes in old array which we do changes in new array.
            _2Darray = new2Darray; // address of new2Darray is now address of _2Darray

            // now do same as we were doing before
            rows++;
        }
    }
    file.close(); // closing file
}
BackendAccess::~BackendAccess()
{
    for (int i = 0; i < Capacity; i++)
    {
        delete[] _2Darray[i];
    }
    delete[] _2Darray;
}
void BackendAccess::exportData(string fileName)
{
    ofstream backend;
    backend.open(fileName);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            backend << _2Darray[i][j] << ',';
        }
        if (i < rows - 1) // if we dont do this after last row, an empty row will be added
        {
            backend << endl;
        }
    }
}
void BackendAccess::writeEmployee(string name, string id, string designation, string fileName)
{
    bool fileIsEmpty;
    /////////////////////////////////////////////////////////
    ifstream file(fileName);
    // Check if the file is empty
    file.seekg(0, ios::end); // Move to the end of the file
    streampos fileSize = file.tellg();
    if (fileSize == 0)
    {
        fileIsEmpty = true;
    }
    else
    {
        fileIsEmpty = false;
    }
    // Close the file
    file.close();
    /////////////////////////////////////////////////////////

    ofstream backend;
    backend.open(fileName, ios::app);
    if (fileIsEmpty)
    {
        backend << name << ',' << id << ',' << designation << ',';
    }
    else
    {
        backend << endl // go to next line then enter data
                << name << ',' << id << ',' << designation << ',';
    }

    // update backend array also
    _2Darray[rows][0] = name;
    _2Darray[rows][1] = id;
    _2Darray[rows][2] = designation;
    rows++;
}
void BackendAccess::writeFlight(string origin, string destination, string departure, string arrival, string flightCode, string totalSeats, string price, string fileName)
{
    bool fileIsEmpty;
    /////////////////////////////////////////////////////////
    ifstream file(fileName);
    // Check if the file is empty
    file.seekg(0, ios::end); // Move to the end of the file
    streampos fileSize = file.tellg();
    if (fileSize == 0)
    {
        fileIsEmpty = true;
    }
    else
    {
        fileIsEmpty = false;
    }
    // Close the file
    file.close();
    /////////////////////////////////////////////////////////

    ofstream backend;
    backend.open(fileName, ios::app);
    if (fileIsEmpty)
    {
        backend << origin << ',' << destination << ',' << departure << ',' << arrival << ',' << flightCode << ',' << totalSeats << ',' << price << ',';
    }
    else
    {
        backend << endl // go to next line then enter data
                << origin << ',' << destination << ',' << departure << ',' << arrival << ',' << flightCode << ',' << totalSeats << ',' << price << ',';
    }

    // update backend array also
    _2Darray[rows][0] = origin;
    _2Darray[rows][1] = destination;
    _2Darray[rows][2] = departure;
    _2Darray[rows][3] = arrival;
    _2Darray[rows][4] = flightCode;
    _2Darray[rows][5] = totalSeats;
    _2Darray[rows][6] = price;
    rows++;
}
// %%%%%%%%%%%%%%%%%%%%% (backend end) %%%%%%%%%%%%%%%%%%%%%

// %%%%%%%%%%%%%%%%%%%%% (AirportManagementSystem start) %%%%%%%%%%%%%%%%%%%%%
class AirportManagementSystem
{
private:
    string airportName;
    string city;
    Employee *allEmployees;
    Flight *allFlights;
    // Passenger *allPassengers;
    BackendAccess EmployeeBackend;
    BackendAccess FlightsBackend;
    // BackendAccess PassengersBackend;
    int employeeCapacity;
    int employeeRows;
    int flightCapacity;
    int flightRows;

public:
    AirportManagementSystem() : EmployeeBackend(3, "employeeBackend.txt"), employeeCapacity(10), employeeRows(0), FlightsBackend(7, "flightBackend.txt"), flightCapacity(10), flightRows(0) // passing no. of cols
    {
        allEmployees = new Employee[employeeCapacity];
        allFlights = new Flight[flightCapacity];

        for (int i = 0; i < EmployeeBackend.getRows(); i++)
        {
            if (employeeCapacity != employeeRows)
            {
                // initializing every element of employee array with name, id, designation
                *(allEmployees + i) = Employee(EmployeeBackend._2Darray[i][0], EmployeeBackend._2Darray[i][1], EmployeeBackend._2Darray[i][2]);
                employeeRows++;
            }

            // if capacity is full first increase the capacity, then copy previous array to new array with increased size, then empty previous array, then pass ownership of new array to old one, then perform the tasks.
            else
            {
                // increasing capacity
                employeeCapacity *= 2;

                // make a new database with increased capacity
                Employee *newAllEmployees = new Employee[employeeCapacity];

                // copy previous array to new one
                for (int j = 0; j < employeeRows; j++)
                {
                    newAllEmployees[j] = allEmployees[j];
                }

                // delete everything from previous array
                delete[] allEmployees;

                // transfer the ownership of new array to old array so that we don't have to change in our code to perform functions on new array. We can just make new array transfer ownership to old array and make changes in old array which we do changes in new array.
                allEmployees = newAllEmployees; // address of newDatabase is now address of Database

                // now do same as we did in if condition
                *(allEmployees + i) = Employee(EmployeeBackend._2Darray[i][0], EmployeeBackend._2Darray[i][1], EmployeeBackend._2Darray[i][2]);
                employeeRows++;
            }
        }

        for (int i = 0; i < FlightsBackend.getRows(); i++)
        {
            if (flightCapacity != flightRows)
            {
                *(allFlights + i) = Flight(FlightsBackend._2Darray[i][0], FlightsBackend._2Darray[i][1], FlightsBackend._2Darray[i][2], FlightsBackend._2Darray[i][3], FlightsBackend._2Darray[i][4], FlightsBackend._2Darray[i][5], FlightsBackend._2Darray[i][6]);
                flightRows++;
            }

            // if capacity is full first increase the capacity, then copy previous array to new array with increased size, then empty previous array, then pass ownership of new array to old one, then perform the tasks.
            else
            {
                // increasing capacity
                flightCapacity *= 2;

                // make a new database with increased capacity
                Flight *newAllFlights = new Flight[employeeCapacity];

                // copy previous array to new one
                for (int j = 0; j < flightRows; j++)
                {
                    newAllFlights[j] = allFlights[j];
                }

                // delete everything from previous array
                delete[] allFlights;

                // transfer the ownership of new array to old array so that we don't have to change in our code to perform functions on new array. We can just make new array transfer ownership to old array and make changes in old array which we do changes in new array.
                allFlights = newAllFlights; // address of newDatabase is now address of Database

                // now do same as we did in if condition
                *(allFlights + i) = Flight(FlightsBackend._2Darray[i][0], FlightsBackend._2Darray[i][1], FlightsBackend._2Darray[i][2], FlightsBackend._2Darray[i][3], FlightsBackend._2Darray[i][4], FlightsBackend._2Darray[i][5], FlightsBackend._2Darray[i][6]);
                flightRows++;
            }
        }
    }
    void pushEmployee(string n, string id, string d);
    void popEmployee(string id);
    void showEmployeeByID(string id);
    void showEmployees();

    void pushFlight(string Origin, string Destination, string Departure, string Arrival, string FlightCode, string TotalSeats, string Price);
    void popFlight(string flightCode);
    void showFlightByFlightCode(string flightCode);
    void showAllFlight();
    void showFlightByOrigin(string Origin);
    void showFlightByDestination(string destinition);

    void pushPassenger();
    void popPassenger();
    void showPassengerByID();
    void showPassenger();

    void exportData();
    void importData();
    ~AirportManagementSystem();
};
void AirportManagementSystem::pushEmployee(string n, string id, string d)
{
    if (employeeRows != employeeCapacity)
    {
        // first add an employee at index row, then increase row by 1 ie row++;
        allEmployees[employeeRows++] = Employee(n, id, d);
        EmployeeBackend.writeEmployee(n, id, d, "employeeBackend.txt");
    }

    // if capacity is full first increase the capacity, then copy previous array to new array with increased size, then empty previous array, then pass ownership of new array to old one, then perform the tasks.
    else
    {
        // increasing capacity
        employeeCapacity *= 2;

        // make a new database with increased capacity
        Employee *newAllEmployees = new Employee[employeeCapacity];

        // copy previous array to new one
        for (int i = 0; i < employeeRows; i++)
        {
            newAllEmployees[i] = allEmployees[i];
        }

        // delete everything from previous array
        delete[] allEmployees;

        // transfer the ownership of new array to old array so that we don't have to change in our code to perform functions on new array. We can just make new array transfer ownership to old array and make changes in old array which we do changes in new array.
        allEmployees = newAllEmployees; // address of newDatabase is now address of Database

        // now do same as we did in if condition
        allEmployees[employeeRows++] = Employee(n, id, d);
    }
}
void AirportManagementSystem::popEmployee(string id)
{
    // first pop from allEmployees then from employeeBackend

    // first find the index at which the person with specific id is, when found that index, shift all array next to it one index behind. In this way, you will delete person of that id, then after for loop decrease no. of rows by 1.
    for (int i = 0; i < employeeRows; i++)
    {
        if (allEmployees[i].getID() == id)
        {
            // when index is found, shift remaining elements to fill the gap
            for (int j = i; j < employeeRows - 1; j++)
            {
                allEmployees[j] = allEmployees[j + 1];
            }

            // decreasing 1 from rows
            --employeeRows;
        }
    }

    for (int i = 0; i < EmployeeBackend.getRows(); i++)
    {
        if (EmployeeBackend._2Darray[i][1] == id)
        {
            for (int j = i; j < EmployeeBackend.getRows() - 1; j++)
            {
                EmployeeBackend._2Darray[j] = EmployeeBackend._2Darray[j + 1];
            }
            EmployeeBackend.setRows(EmployeeBackend.getRows() - 1);
        }
    }

    EmployeeBackend.exportData("employeeBackend.txt");
}
void AirportManagementSystem::showEmployeeByID(string id)
{
    for (int i = 0; i < employeeRows; i++)
    {
        if ((allEmployees + i)->getID() == id)
        {
            cout << "Name: " << (allEmployees + i)->getName() << endl;
            cout << "ID: " << (allEmployees + i)->getID() << endl;
            cout << "Designation: " << (allEmployees + i)->getDesignation() << endl;
            return;
        }
    }
}
void AirportManagementSystem::showEmployees()
{
    for (int i = 0; i < employeeRows; i++)
    {
        cout << "Employee # " << i + 1 << endl;
        cout << "Name: " << (allEmployees + i)->getName() << endl;
        cout << "ID: " << (allEmployees + i)->getID() << endl;
        cout << "Designation: " << (allEmployees + i)->getDesignation() << endl;
        cout << endl;
    }
}

void AirportManagementSystem::pushFlight(string Origin, string Destination, string Departure, string Arrival, string FlightCode, string TotalSeats, string Price)
{
    if (flightRows != flightCapacity)
    {
        // first add an employee at index row, then increase row by 1 ie row++;
        allFlights[flightRows++] = Flight(Origin, Destination, Departure, Arrival, FlightCode, TotalSeats, Price);
        FlightsBackend.writeFlight(Origin, Destination, Departure, Arrival, FlightCode, TotalSeats, Price, "flightBackend.txt");
    }

    // if capacity is full first increase the capacity, then copy previous array to new array with increased size, then empty previous array, then pass ownership of new array to old one, then perform the tasks.
    else
    {
        // increasing capacity
        flightCapacity *= 2;

        // make a new database with increased capacity
        Flight *newAllFlights = new Flight[flightCapacity];

        // copy previous array to new one
        for (int i = 0; i < flightRows; i++)
        {
            newAllFlights[i] = allFlights[i];
        }

        // delete everything from previous array
        delete[] allFlights;

        // transfer the ownership of new array to old array so that we don't have to change in our code to perform functions on new array. We can just make new array transfer ownership to old array and make changes in old array which we do changes in new array.
        allFlights = newAllFlights; // address of newDatabase is now address of Database

        // now do same as we did in if condition
        allFlights[flightRows++] = Flight(Origin, Destination, Departure, Arrival, FlightCode, TotalSeats, Price);
    }
}
void AirportManagementSystem::popFlight(string flightCode)
{
    // first pop from allFlights then from flightBackend

    // first find the index at which the person with specific id is, when found that index, shift all array next to it one index behind. In this way, you will delete person of that id, then after for loop decrease no. of rows by 1.
    for (int i = 0; i < flightRows; i++)
    {
        if (allFlights[i].getFlighCode() == flightCode)
        {
            // when index is found, shift remaining elements to fill the gap
            for (int j = i; j < flightRows - 1; j++)
            {
                allFlights[j] = allFlights[j + 1];
            }

            // decreasing 1 from rows
            --flightRows;
        }
    }

    for (int i = 0; i < FlightsBackend.getRows(); i++)
    {
        if (FlightsBackend._2Darray[i][4] == flightCode)
        {
            for (int j = i; j < FlightsBackend.getRows() - 1; j++)
            {
                FlightsBackend._2Darray[j] = FlightsBackend._2Darray[j + 1];
            }
            FlightsBackend.setRows(FlightsBackend.getRows() - 1);
        }
    }

    FlightsBackend.exportData("flightBackend.txt");
}
void AirportManagementSystem::showFlightByFlightCode(string flightCode)
{
    for (int i = 0; i < flightRows; i++)
    {
        if ((allFlights + i)->getFlighCode() == flightCode)
        {
            cout << "Flight Code: " << (allFlights + i)->getFlighCode() << endl;
            cout << "Flight Origin: " << (allFlights + i)->getOrigin() << endl;
            cout << "Flight Destination: " << (allFlights + i)->getDestination() << endl;
            cout << "Flight Price per seat: " << (allFlights + i)->getPrice() << endl;
            cout << "Flight Total Seats: " << (allFlights + i)->getTotalSeats() << endl;
            cout << "Flight Seats Booked: " << (allFlights + i)->getBookedSeats() << endl;
            cout << "Flight Departure: " << (allFlights + i)->getDeparture() << endl;
            cout << "Flight Arrival: " << (allFlights + i)->getArrival() << endl;
            cout << endl;
        }
    }
}
void AirportManagementSystem::showAllFlight()
{
    for (int i = 0; i < flightRows; i++)
    {
        cout << "Flight Code: " << (allFlights + i)->getFlighCode() << endl;
        cout << "Flight Origin: " << (allFlights + i)->getOrigin() << endl;
        cout << "Flight Destination: " << (allFlights + i)->getDestination() << endl;
        cout << "Flight Price per seat: " << (allFlights + i)->getPrice() << endl;
        cout << "Flight Total Seats: " << (allFlights + i)->getTotalSeats() << endl;
        cout << "Flight Seats Booked: " << (allFlights + i)->getBookedSeats() << endl;
        cout << "Flight Departure: " << (allFlights + i)->getDeparture() << endl;
        cout << "Flight Arrival: " << (allFlights + i)->getArrival() << endl;
        cout << endl;
    }
}
void AirportManagementSystem::showFlightByOrigin(string Origin)
{
    for (int i = 0; i < flightRows; i++)
    {
        if ((allFlights + i)->getOrigin() == Origin)
        {
            cout << "Flight Code: " << (allFlights + i)->getFlighCode() << endl;
            cout << "Flight Origin: " << (allFlights + i)->getOrigin() << endl;
            cout << "Flight Destination: " << (allFlights + i)->getDestination() << endl;
            cout << "Flight Price per seat: " << (allFlights + i)->getPrice() << endl;
            cout << "Flight Total Seats: " << (allFlights + i)->getTotalSeats() << endl;
            cout << "Flight Seats Booked: " << (allFlights + i)->getBookedSeats() << endl;
            cout << "Flight Departure: " << (allFlights + i)->getDeparture() << endl;
            cout << "Flight Arrival: " << (allFlights + i)->getArrival() << endl;
            cout << endl;
        }
    }
}
void AirportManagementSystem::showFlightByDestination(string destinition)
{
    for (int i = 0; i < flightRows; i++)
    {
        if ((allFlights + i)->getDestination() == destinition)
        {
            cout << "Flight Code: " << (allFlights + i)->getFlighCode() << endl;
            cout << "Flight Origin: " << (allFlights + i)->getOrigin() << endl;
            cout << "Flight Destination: " << (allFlights + i)->getDestination() << endl;
            cout << "Flight Price per seat: " << (allFlights + i)->getPrice() << endl;
            cout << "Flight Total Seats: " << (allFlights + i)->getTotalSeats() << endl;
            cout << "Flight Seats Booked: " << (allFlights + i)->getBookedSeats() << endl;
            cout << "Flight Departure: " << (allFlights + i)->getDeparture() << endl;
            cout << "Flight Arrival: " << (allFlights + i)->getArrival() << endl;
            cout << endl;
        }
    }
}

void AirportManagementSystem::pushPassenger() {}
void AirportManagementSystem::popPassenger() {}
void AirportManagementSystem::showPassengerByID() {}
void AirportManagementSystem::showPassenger() {}

void AirportManagementSystem::exportData() {}
void AirportManagementSystem::importData() {}
AirportManagementSystem::~AirportManagementSystem()
{
    delete[] allEmployees;
    delete[] allFlights;
    // delete[] allPassengers;
}
// %%%%%%%%%%%%%%%%%%%%% (AirportManagementSystem end) %%%%%%%%%%%%%%%%%%%%%

// user defined function declerations
inline void greetings();
void authentication();
string getPassword(const string &x);
void manageEmployees(AirportManagementSystem *ptrSys1);
void managePassengers(AirportManagementSystem *ptrSys1);
void manageFlights(AirportManagementSystem *ptrSys1);

int main()
{
    greetings();
    authentication();
    cout << endl;

    AirportManagementSystem System1;
    // /*
    int choice;
    do
    {
        cin.clear();
        fflush(stdin);

        cout << "________________________________\n";
        cout << "|---------( Main Page )--------|\n";
        cout << "| Choose one of the following: |\n";
        cout << "********************************\n";
        cout << "| 1: Manage Employees          |\n";
        cout << "| 2: Manage Flights            |\n";
        cout << "| 3: Manage Passengers         |\n";
        cout << "| 4: Exit                      |\n";
        cout << "********************************\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls"); // to clear console
            manageEmployees(&System1);
            break;
        case 2:
            system("cls");
            manageFlights(&System1);
            break;
        case 3:
            system("cls");
            managePassengers(&System1);
            break;
        case 4:
            system("cls");
            cout << "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
            cout << "%%% (Thanks for using our AMS) %%%\n";
            cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n";
            break;

        default:
            system("cls");
            cout << "Invalid Input\n";
            break;
        }

    } while (choice != 4);
    // */

    return 0;
}
// user defined function definitions
inline void greetings()
{
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
    cout << "%%%% Welcome to Superior Airport Management System %%%%\n";
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
    cout << endl
         << endl;
    cout << "Press enter to continue...";
    cin.ignore();
    system("cls");
}
void authentication()
{
    int tries = 0;
    do
    {
        string password = getPassword("Enter the password: ");
        if (password == "hello")
        {
            return; // if password correct, return from function
        }
        system("cls");
        if (tries != 2)
        {
            cout << "Wrong Password! Try again\n";
        }
        else
        {
            cout << "You got password wrong 3 times\n";
            exit(0);
        }
        tries++;

    } while (tries < 3);
}
string getPassword(const string &x)
{
    cout << x;
    char temp = 0;   // ascii 0 = NULL // each character will first come to temp, then concatinate with y
    string password; // this is the password that we will return

    // using (get character) _getch instead of getch : https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch?view=msvc-170
    while ((temp = _getch()) != 13) // ascii 13 = Enter
    {
        if (temp == 8 && !password.empty()) // ascii 8 means backspace
        {
            cout << "\b \b";     // move cursor back, then print space, then move back again
            password.pop_back(); // remove last character from string
        }
        else
        {
            cout << '*';
            password.push_back(temp);
        }
    }
    return password;
}

void manageEmployees(AirportManagementSystem *ptrSys1)
{
    int choice;
    do
    {
        cin.clear();
        fflush(stdin);

        cout << "________________________________\n";
        cout << "|---------( Main Page )--------|\n";
        cout << "| Choose one of the following: |\n";
        cout << "********************************\n";
        cout << "| 1: Show all Employees        |\n";
        cout << "| 2: Show Employee by ID       |\n";
        cout << "| 3: Add Employee              |\n";
        cout << "| 4: Remove Employee           |\n";
        cout << "| 5: Exit                      |\n";
        cout << "********************************\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string name;
        string ID; // because this cannot be declared within the switch
        string designation;
        switch (choice)
        {
        case 1:
            system("cls"); // to clear console
            ptrSys1->showEmployees();
            break;
        case 2:
            system("cls");
            cout << "Enter the Employee ID: ";
            cin >> ID;
            ptrSys1->showEmployeeByID(ID);
            break;
        case 3:
            system("cls");
            cout << "Enter the name of Employee: ";
            getline(cin >> ws, name);
            cout << "Enter the ID of Employee: ";
            cin >> ID;
            cout << "Enter the designation of Employee: ";
            cin >> designation;
            ptrSys1->pushEmployee(name, ID, designation);
            break;
        case 4:
            system("cls");
            cout << "Enter the Employee ID: ";
            cin >> ID;
            ptrSys1->popEmployee(ID);
            break;
        case 5:
            system("cls");
            break;

        default:
            system("cls");
            cout << "Invalid Input\n";
            break;
        }

    } while (choice != 5);
}
void manageFlights(AirportManagementSystem *ptrSys1)
{
    int choice;
    do
    {
        cin.clear();
        fflush(stdin);

        cout << "_________________________________\n";
        cout << "|----------( Main Page )--------|\n";
        cout << "| Choose one of the following:  |\n";
        cout << "*********************************\n";
        cout << "| 1: Show all Flights           |\n";
        cout << "| 2: Show Flight by Origin      |\n";
        cout << "| 3: Show Flight by Destination |\n";
        cout << "| 4: Show Flight by Flight Code |\n";
        cout << "| 5: Add Flight                 |\n";
        cout << "| 6: Remove Flight              |\n";
        cout << "| 7: Exit                       |\n";
        cout << "*********************************\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // because this cannot be declared within the switch
        string origin;
        string destination;
        string departure;
        string arrival;
        string flighCode;
        string price;
        string totalSeats;

        switch (choice)
        {
        case 1:
            system("cls"); // to clear console
            ptrSys1->showAllFlight();
            break;
        case 2:
            system("cls"); // to clear console
            cout << "Enter origin: ";
            cin >> origin;
            ptrSys1->showFlightByOrigin(origin);
            break;
        case 3:
            system("cls"); // to clear console
            cout << "Enter Destination: ";
            cin >> destination;
            ptrSys1->showFlightByDestination(destination);
            break;
        case 4:
            system("cls");
            cout << "Enter Flight Code: ";
            cin >> flighCode;
            ptrSys1->showFlightByFlightCode(flighCode);
            break;
        case 5:
            system("cls");
            cout << "Enter flightCode: ";
            getline(cin >> ws, flighCode);
            cout << "Enter Origin: ";
            getline(cin >> ws, origin);
            cout << "Enter Destination: ";
            getline(cin >> ws, destination);
            cout << "Enter Departure Time: ";
            getline(cin >> ws, departure);
            cout << "Enter Arrival Time: ";
            getline(cin >> ws, arrival);
            cout << "Enter Total no. of seats: ";
            cin >> totalSeats;
            cout << "Enter price per seat: ";
            cin >> price;
            ptrSys1->pushFlight(origin, destination, departure, arrival, flighCode, totalSeats, price);
            break;
        case 6:
            system("cls");
            cout << "Enter the Flight Code: ";
            cin >> flighCode;
            ptrSys1->popFlight(flighCode);
            break;
        case 7:
            system("cls");
            break;

        default:
            system("cls");
            cout << "Invalid Input\n";
            break;
        }

    } while (choice != 7);
}
void managePassengers(AirportManagementSystem *ptrSys1)
{
}