#include<fstream>
#include<iostream>
#include<string>
#include<string.h>
#include<iomanip>
#include<vector>
#include<algorithm>

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

string const MONTHS[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

class realStateProperty;

vector<realStateProperty> properties;

bool isEqualString(string, string);
bool isValidMonth(int);
bool isValidDate(dateRange);
bool isValidDateRange(realStateProperty* rsp, dateRange *dr);
bool isValidAddress(string);
string getFormattedDate(mdDate);
mdDate getDate(string, char);
dateRange stringToDateRange(string);
string dateRangeToString(dateRange *);
void generateReport();
vector<string> getMonthCalenderData(int, vector<int> *);
void updateProperty(realStateProperty*);
void copyProperties();
void updateProperties(realStateProperty*);


class realStateProperty {
    double pricePerNight;
    int tenants;

    public:
    string address, bookedDatesString;
    vector <dateRange> bookedDates;
    realStateProperty(){}
    realStateProperty(string);
    realStateProperty(vector <string> *);
    // ~realStateProperty() {
    //     generateReport(this);
    // }
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
        cout << "Address:" << address << endl;
        cout << "Price per night:" << pricePerNight << endl;
        cout << "tenants:" << tenants << endl;
        for(dateRange d : this->bookedDates){
            cout << "date = " << getFormattedDate(d.start) << " - " << getFormattedDate(d.end) << endl;getFormattedDate(d.start);
        }
    }
};

realStateProperty :: realStateProperty(string address) {
    
    for (realStateProperty rs: properties) {
        if (isEqualString(rs.address,address)) {
            this->address = rs.address;
            this->pricePerNight = rs.pricePerNight;
            this->tenants = rs.tenants;
            this->bookedDates = rs.bookedDates;
            break;
        }
    }
}

realStateProperty :: realStateProperty(vector <string> *prop) {
    
    this->address = (*prop)[0];
    this->pricePerNight = stod((*prop)[1]);
    this->tenants = stoi((*prop)[2]);
    for (int i = 0; i < tenants; i++) {
        this->bookedDates.push_back(stringToDateRange((*prop)[i+3]));
    }
}

void realStateProperty :: book() {
    dateRange dr;

    cin.clear();
    cin.ignore(100, '\n');
    cout << "Enter the start month and day:\n";
    cin >> dr.start.month >> dr.start.day;
    cout << "Enter the end month and day:\n";
    cin >> dr.end.month >> dr.end.day;
    if(isValidDate(dr) && isValidDateRange(this, &dr)) {
        this->updateBookedDates(&dr);
        // updateProperties(this);
        cout << "Booked!" << endl;        
    };
}

void realStateProperty :: updateBookedDates(dateRange * dr) {
    this->bookedDates.push_back(*dr);
    this->tenants = this->tenants + 1;
    sort(this->bookedDates.begin(), this->bookedDates.end(), [](dateRange &a, dateRange &b)->bool { return a.start.month < b.start.month; });
}

void realStateProperty :: showMonth() {
    int month;
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Enter the month number:\n";
    cin >> month;
    if(isValidMonth(month)) {
        vector<int> bookedDates;
        for(dateRange d : this->bookedDates){
            if (d.start.month == month) {
                for (int day = d.start.day; day <= d.end.day; day++) {
                    bookedDates.push_back(day);
                }
            }
        }

        for (string str: getMonthCalenderData(month, &bookedDates)){
            cout << str;
        }
        cout << endl;
    }
}

int main() {

    char option;
    string inputAdd ;
    bool exited = false;
    try {
        copyProperties();
        while(!exited) {

            cout << "Enter the address of the property you want to book or exit to exit:\n";

            getline(cin, inputAdd);

            if (inputAdd.compare("exit") == 0) {
                exited = true;
            } else if (!isValidAddress(inputAdd)) {
                cout << "Address not found" << endl;
            } else {
                realStateProperty rs(inputAdd);
                bool complete = false;
                while(!complete) {
                    cout << "Enter an option:\n1. Show the calendar of the property on a given month\n2. Book a period of time for a given property\nq. Quit" << endl;
                    cin >> option;

                    switch(option) {
                        case '1':
                            rs.showMonth();
                            break;
                        case '2':
                            rs.book();
                            break;
                        case 'q':
                            complete = true;
                            break;
                        default:
                            cout << "Invalid option\n";
                    }
                }
                updateProperty(&rs);
                cin.ignore();
            }
        }
        generateReport();
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

bool isValidDate(dateRange d) {
    if (d.start.month > 12 || d.start.month < 1 || d.end.month > 12 || d.end.month < 1) {
        cout << "Invalid month number" << endl;
        return false;
    } else if (d.start.day > 31 || d.start.day < 1 || d.end.day > 31 || d.end.day < 1) {
        cout << "Invalid day number" << endl;
        return false;
    }
    return true;
}

bool isValidDateRange(realStateProperty* rsp, dateRange *dr) {
    if (dr->start.month > dr->end.month || dr->start.day > dr->end.day) {
        cout << "Invalid date range" << endl;
        return false;
    } else {
        bool valid = true;
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
}

bool isEqualString(string str1, string str2) {
    // if (str1.length() != str2.length()) {
    //     return false;
    // }
    bool equal = true;
    for(unsigned int i = 0; i < str1.length() - 1; i++) {
        if (str1[i] != str2[i]) {
            equal = false;
            break;
        } 
    }
    return equal;
}

bool isValidAddress(string address) {
    ifstream infile(PROPERTY_FILE);
    string fileLine, prevLine;
    int count = 0;
    bool valid = false;
    while(getline(infile, fileLine)) {
        if ((count == 0 || int(prevLine[0]) == 0 || int(prevLine[0]) == 13) && isEqualString(fileLine, address)) {
            valid = true;
            break;
        }
        prevLine = fileLine;
        count++;
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
    dateRange tempDR = {getDate(dateString, 's'), getDate(dateString, 'e')};
    return tempDR;
}

string dateRangeToString(dateRange *date){
    string dateString = to_string(date->start.month) + " " + to_string(date->start.day) + " " + to_string(date->end.month) + " " + to_string(date->end.day);
    return dateString;
}

// void generateReport() {
//     ofstream outfile(OUTPUT_FILE, ios::out | ios::trunc);
//     for (int n = 0; size_t(n) < properties.size(); n++) {
//         realStateProperty &temprsp = properties[n];
//         double totalYearlyRevenue = 0.00;

//         outfile << "Address: " << temprsp.address << endl;
//         outfile << fixed << setprecision(2);
            
//         vector<int> bookedDates;

//         double monthlyRevenue = 0.00, rate = temprsp.getPricePerNight();

//         int endMonth = temprsp.bookedDates.back().end.month, monthCounter = temprsp.bookedDates.front().start.month;
//         for (int i = 0; size_t(i) < temprsp.bookedDates.size(); i++) {
//             dateRange tempDR = temprsp.bookedDates[i];

//             do {
//                 if (tempDR.start.month == temprsp.bookedDates[i-1].end.month) {
//                     for(int day = tempDR.start.day; day <= tempDR.end.day; day++){
//                         bookedDates.push_back(day);
//                         if (day%7 == 1 || day%7 == 0) {
//                             monthlyRevenue = monthlyRevenue + 2*rate;
//                         } else {
//                             monthlyRevenue = monthlyRevenue + rate;
//                         }
//                     }
//                 } else {
//                     bookedDates.clear();
//                     for(int day = tempDR.start.day; day <= tempDR.end.day; day++){
//                         bookedDates.push_back(day);
//                         if (day%7 == 1 || day%7 == 0) {
//                             monthlyRevenue = monthlyRevenue + 2*rate;
//                         } else {
//                             monthlyRevenue = monthlyRevenue + rate;
//                         }
//                     }

//                 }
//                 if (monthCounter == tempDR.start.month) {
//                     outfile << MONTHS[monthCounter-1] << ": " << monthlyRevenue << endl;
                        
//                     vector <string> calenderData = getMonthCalenderData(monthCounter, &bookedDates);

//                     for (string str: calenderData){
//                         outfile << str;
//                     }

//                     outfile << "\n";

//                     totalYearlyRevenue = totalYearlyRevenue + monthlyRevenue;
//                     // bookedDates.clear();
//                     monthlyRevenue = 0.00;
//                 }
//                 monthCounter++;
//             } while (monthCounter <= endMonth);
//         }
//         outfile << "Total: $" << totalYearlyRevenue << endl;
//         // if (&temprsp != &properties.back()) {
//             outfile << endl;
//         // }
//     }
//     outfile.close();
// }

void generateReport() {
    ofstream outfile(OUTPUT_FILE, ios::out | ios::trunc);
    for (int n = 0; size_t(n) < properties.size(); n++) {
        realStateProperty &temprsp = properties[n];
        double totalYearlyRevenue = 0.00;

        outfile << "Address: " << temprsp.address << endl;
        outfile << fixed << setprecision(2);
            
        vector<int> bookedDates;

        double monthlyRevenue = 0.00, rate = temprsp.getPricePerNight();
        for (int i = 0; size_t(i) < temprsp.bookedDates.size(); i++) {
            dateRange &dr = temprsp.bookedDates[i];
            int startMonth = dr.start.month;
            int endMonth = dr.end.month;
            int nextDateMon = temprsp.bookedDates[i+1].start.month;
            int monthCounter = startMonth;

            do {
                int startDay = (dr.end.month - dr.start.month > 0) && (monthCounter > dr.start.month) ? 1 : dr.start.day;;
                int endDay = (dr.end.month - dr.start.month > 0) && (monthCounter < dr.end.month) ? DAYS_IN_A_MONTH : dr.end.day;
                for(int day = startDay; day <= endDay; day++){
                    bookedDates.push_back(day);
                    if (day%7 == 1 || day%7 == 0) {
                        monthlyRevenue = monthlyRevenue + 2*rate;
                    } else {
                        monthlyRevenue = monthlyRevenue + rate;
                    }
                }

                if (startMonth != nextDateMon) {
                    outfile << MONTHS[monthCounter-1] << ": " << monthlyRevenue << endl;
                    
                    vector <string> calenderData = getMonthCalenderData(monthCounter, &bookedDates);

                    for (string str: calenderData){
                        outfile << str;
                    }

                    outfile << "\n";

                    totalYearlyRevenue = totalYearlyRevenue + monthlyRevenue;
                    bookedDates.clear();
                    monthlyRevenue = 0.00;
                }
                monthCounter++;
            } while (monthCounter <= endMonth);
        }
        outfile << "Total: $" << totalYearlyRevenue << endl;
        // if (&temprsp != &properties.back()) {
            outfile << endl;
        // }
    }
    outfile.close();
}

vector <string> getMonthCalenderData(int month, vector<int> * bookedDates) {
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
                    if (day == 1 || day == 7) {
                        // cout << dayCounter == 31 ? "D" : "D ";
                        calender.push_back(dayCounter == 31 ? "D" : "D ");
                    } else {
                        // cout << dayCounter == 31 ? "B" : "B ";
                        calender.push_back(dayCounter == 31 ? "B" : "B ");
                    };
                } else {
                    // cout << dayCounter == 31 ? "*" : "* ";
                    calender.push_back(dayCounter == 31 ? "*" : "* ");
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

void copyProperties() {
    ifstream fin;
    fin.open(PROPERTY_FILE);
    vector<string> property;
    string fileLine;
    while(getline(fin, fileLine)) {
        if (int(fileLine[0]) == 0 || int(fileLine[0]) == 13){
            realStateProperty rs(&property);
            properties.push_back(rs);
            property.clear();
        } else {
            property.push_back(fileLine);
        }
    }
    realStateProperty rs(&property);
    properties.push_back(rs);
    property.clear();
    fin.close();
}

void updateProperty(realStateProperty *rsp) {
    for(realStateProperty &rs : properties) {
        if (rs.address == rsp->address) {
            rs = *rsp;
        }
    }
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
        if (rs.address.compare(rsp->address) == 0) {
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