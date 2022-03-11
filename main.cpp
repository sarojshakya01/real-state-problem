#include<fstream>
#include<iostream>
#include<iomanip>
#include<vector>
#include <map>

#include <algorithm>

#define DAYS_IN_A_MONTH 31
#define PROPERTY_FILE "properties.txt"
#define OUTPUT_FILE "yearly.txt"

using namespace std;

struct customDate {
    int month, day;
};

typedef struct customDate mdDate;

struct dateRange {
    mdDate start, end;
};

typedef struct dateRange dateRange;

string const MONTHS[12] = {"January", "February", "March", "April", "May", "Jun", "July", "August", "September", "October", "November", "December"};

class realStateProperty;

bool isValidMonth(int);
bool isValidDate(mdDate);
bool isValidDateRange(realStateProperty* rsp, dateRange *dr);
bool isValidAddress(string);
string getFormattedDate(mdDate);
mdDate getDate(string, char);
dateRange stringToDateRange(string);
string dateRangeToString(dateRange *);
void generateReport(realStateProperty *);
vector<string> getMonthCalenderData(int, vector<int> *, double &, double r = 0.00);
void updateProperties(realStateProperty*);

class realStateProperty {
    double pricePerNight;
    int tenants;

    public:
    string address, bookedDatesString;
    vector <dateRange> bookedDates;
    realStateProperty() {}
    realStateProperty(string);
    ~realStateProperty() {
        generateReport(this);
    }
    void book();
    void updateBookedDates(dateRange*);
    void showMonth();
    double getPricePerNight() {
        return this->pricePerNight;
    }
    int getTenants() {
        return this->tenants;
    }
    void display() {
        cout << "Address: " << address << endl;
        cout << "Price per night: " << pricePerNight << endl;
        cout << "tenants: " << tenants << endl;
        for(dateRange d : this->bookedDates){
            cout << "date = " << getFormattedDate(d.start) << " - " << getFormattedDate(d.end) << endl;getFormattedDate(d.start);
        }  
    }
};

realStateProperty :: realStateProperty(string address) {
    ifstream infile(PROPERTY_FILE);
    string line;
    bool isPropertyFound = false;
    int propertyCount = 0;
    while(getline(infile, line)) {
        if (isPropertyFound) {
            if (line[0] == '\0'){
                break;
            } else {
                if (propertyCount == 1) {
                    this->pricePerNight = stod(line);
                } else if (propertyCount == 2) {
                    this->tenants = stoi(line);
                } else {
                    this->bookedDates.push_back(stringToDateRange(line));
                }
            }
            propertyCount++;
        }
        if (line == address) {
            isPropertyFound = true;
            this->address = line;
            propertyCount = 1;
        }
    }
    infile.close();
}

void realStateProperty :: book() {
    dateRange dr;

    do {
        do {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Enter the start month and day: ";
            cin >> dr.start.month >> dr.start.day;
        } while(!isValidDate(dr.start));

        do {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Enter the end month and day: ";
            cin >> dr.end.month >> dr.end.day;
        } while(!isValidDate(dr.end));

    } while(!isValidDateRange(this, &dr));

    this->updateBookedDates(&dr);
    
    updateProperties(this);
    
    cout << "Booked!" << endl;
}

void realStateProperty :: updateBookedDates(dateRange * dr) {
    this->bookedDates.push_back(*dr);
    this->tenants = this->tenants + 1;
    sort(this->bookedDates.begin(), this->bookedDates.end(), [](dateRange &a, dateRange &b)->bool { return a.start.month < b.start.month; });
}

void realStateProperty :: showMonth() {
    int month;
    do {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "Enter the month number: ";
        cin >> month;
    } while(!isValidMonth(month));

    
    vector<int> bookedDates;
    for(dateRange d : this->bookedDates){
        if (d.start.month == month) {
            for (int day = d.start.day; day <= d.end.day; day++) {
                bookedDates.push_back(day);
            }
        }
    }
    double formalVar = 0;
    for (string str: getMonthCalenderData(month, &bookedDates, formalVar)){
        cout << str;
    }
}

int main() {

    // test statements
    // realStateProperty rp1("Argus Street 12311");
    // dateRange dr = {mdDate{1,1}, mdDate{1,1}};
    // rp1.updateBookedDates(&dr);
    // updateProperties(&rp1);
    // generateReport(&rp1);
    // return 0;
    char option;
    string inputAdd;
    bool exited = false;
    try {
        while(!exited) {

            cout << "Enter the address of the property you want to book or exit to exit: ";
            
            getline(cin, inputAdd);
            if (inputAdd == "exit") {
                exited = true;
            }
            if (!isValidAddress(inputAdd)) {
                cout << "Address not found" << endl;
            } else {
                bool complete = false;
                while(!complete) {
                    cout << "Enter an option:\n1. Show the calendar of the property on a given month\n2. Book a period of time for a given property\nq. Quit" << endl;
                    cin >> option;

                    realStateProperty rs(inputAdd);

                    switch(option) {
                        case '1':
                            rs.showMonth();
                            break;
                        case '2':
                            rs.book();
                            complete = true;
                            break;
                        case 'q':
                            complete = true;
                            break;
                        default:
                            cout << "Invalid option\n";
                    }
                }
                cin.ignore();
            }
        }
    } catch (...) {
        cout << "Some error occured" << endl;
    }
    return 0;
}

bool isValidMonth(int month) {
    if (month > 12 || month < 1) {
        cout << "Invalid month number" << endl;
        return false;
    }
    return true;
}

bool isValidDate(mdDate d) {
    if (d.month > 12 || d.month < 1) {
        cout << "Invalid month number" << endl;
        return false;
    }
    if (d.day > 31 || d.day < 1) {
        cout << "Invalid day number" << endl;
        return false;
    }
    return true;   
}

bool isValidDateRange(realStateProperty* rsp, dateRange *dr) {
    bool valid =  true;
    if (dr->start.month > dr->end.month || dr->start.day > dr->end.day) {
        cout << "Invalid date range" << endl;
        return false;
    }
    vector <string> dates;
    for (dateRange bdr: rsp->bookedDates) {
        for (int mon = bdr.start.month; mon <= bdr.end.month; mon++ ) {
            for (int day = bdr.start.day; day <= bdr.end.day; day++) {
                dates.push_back(to_string(mon) + "/" + to_string(day));
            }
        }
    }

    for (int mon = dr->start.month; mon <= dr->end.month; mon++ ) {
        for (int day = dr->start.day; day <= dr->end.day; day++) {
            string date = to_string(mon) + "/" + to_string(day);
            if (find(dates.begin(), dates.end(), date) != dates.end()) {
                valid = false;
                cout << "Date range has dates that are already booked!" << endl;
                break;   
            }
        }
    }
    return valid;
}

bool isValidAddress(string address) {
    ifstream infile(PROPERTY_FILE);
    string line;
    bool valid = false;
    while(getline(infile, line)) {
        if (line == address) {
            valid = true;
            break;
        }
    }
    infile.close();
    return valid;
}

string getFormattedDate(mdDate d) {
    string tempDateString = to_string(d.month) + "/" + to_string(d.day);
    return tempDateString;
}

mdDate getDate(string dateString, char ch) {
    vector<string> nums;
    string num;
    mdDate tempDate;
    for (auto x : dateString) {
        if (x == ' ') {
            nums.push_back(num);
            num = "";
        } else {
            num = num + x;
        }
    }
    nums.push_back(num);
    if (ch == 'e') {
        tempDate.month = stoi(nums[2]);
        tempDate.day = stoi(nums[3]);
    } else {
        tempDate.month = stoi(nums[0]);
        tempDate.day = stoi(nums[1]);
    }
    return tempDate;
}

dateRange stringToDateRange(string dateString){
    dateRange tempDateRange = {getDate(dateString, 's'), getDate(dateString, 'e')};
    return tempDateRange;
}

string dateRangeToString(dateRange *date){
    string dateString = to_string(date->start.month) + " " + to_string(date->start.day) + " " + to_string(date->end.month) + " " + to_string(date->end.day);
    return dateString;
}

void generateReport(realStateProperty *rsp) {
    ofstream outfile(OUTPUT_FILE);
    outfile << "Address: " << rsp->address << endl;
    outfile << fixed << setprecision(2);
    // int totalDays = DAYS_IN_A_MONTH * (dr->end.month - dr->start.month) + (dr->end.day - dr->start.day);
    double totalYearlyRevenue = 0.00;

    vector<int> bookedDates;
    double monthlyRevenue = 0.00, rate = rsp->getPricePerNight();

    for (int i = 0; size_t(i) < rsp->bookedDates.size(); i++) {
        dateRange &dr = rsp->bookedDates[i];

        int mon = dr.start.month;

        int startDay = (dr.end.month - dr.start.month > 0) && (mon > dr.start.month) ? 1 : dr.start.day;
        int endDay = (dr.end.month - dr.start.month > 0) && (mon < dr.end.month) ? DAYS_IN_A_MONTH : dr.end.day;
        for(int day = startDay; day <= endDay; day++){
            bookedDates.push_back(day);
        }
        
        int nextMon = rsp->bookedDates[i+1].start.month;
        
        if (mon != nextMon) {
            vector <string> calenderData = getMonthCalenderData(mon, &bookedDates, monthlyRevenue, rate);
            
            outfile << MONTHS[mon-1] << ": " << monthlyRevenue << endl;

            for (string str: calenderData){
                outfile << str;
            }

            outfile << "\n";

            totalYearlyRevenue = totalYearlyRevenue + monthlyRevenue;
            bookedDates.clear();
            monthlyRevenue = 0.00;
        }
    }

    outfile << "Total: $" << totalYearlyRevenue;
    outfile.close();
}

vector <string> getMonthCalenderData(int month, vector<int> * bookedDates, double &monthlyRevenue, double rate) {
    int dayCounter = 1;
    int startDay = 0; //(month - 1) * 3 % 7; // dynamic start Day of given month
    
    vector <string> calender;
    for (int week = 1; week < 7; week++) {
        for (int day = 1; day <= 7; day++) {
            if ( (week == 1) && (week * day) <= startDay) {
                // cout << "  ";
                calender.push_back("  ");
            }
            else {
                if (find(bookedDates->begin(), bookedDates->end(), dayCounter) != bookedDates->end()) {
                    if (day < 3) {
                        // cout << "D ";
                        calender.push_back("D ");
                        monthlyRevenue = monthlyRevenue + rate * 2;
                    } else {
                        // cout << "B ";
                        calender.push_back("B ");
                        monthlyRevenue = monthlyRevenue + rate;
                    };
                } else {
                    // cout << "* ";
                    calender.push_back("* ");
                }
                dayCounter++;
            }
            if (dayCounter > DAYS_IN_A_MONTH) break;
        }
        // cout << endl;
        calender.push_back("\n");
        if (dayCounter > DAYS_IN_A_MONTH) break;
    }
    return calender;
}

void updateProperties(realStateProperty *rsp) {

    string line;
    fstream fin;
    fin.open(PROPERTY_FILE);
    fstream fout;
    fout.open("temp.txt", ios::out);
    while(getline(fin, line)) {
        fout << line << endl;
    }

    fin.close();
    fout.close();

    fin.open("temp.txt");
    vector<realStateProperty> properties;
    string thisAddress;
    vector<string> property;
    while(getline(fin, line)) {
        if (line[0] == '\0'){
            realStateProperty rs(property[0]);
            properties.push_back(rs);
            property.clear();
        } else {
            property.push_back(line);
        }
    }
    realStateProperty rs(property[0]);
    properties.push_back(rs);
    fin.close();

    remove("temp.txt");

    fout.open(PROPERTY_FILE, ios::out | ios::trunc);
    fout << fixed << setprecision(2);
    for(realStateProperty &rs: properties) {
        fout << rs.address << endl;
        fout << rs.getPricePerNight() << endl;
        if (rs.address == rsp->address) {
            fout << rsp->getTenants() << endl;
            for (dateRange dr : rsp->bookedDates) {
                fout << dateRangeToString(&dr) << endl;
            }
        } else {
            fout << rs.getTenants() << endl;
            for (dateRange dr : rs.bookedDates) {
                fout << dateRangeToString(&dr) << endl;
            }
        }

        if (&rs != &properties.back()) {
            fout << endl;
        }
        
    }
    fout.close();
}
