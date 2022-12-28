/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a patient's care period in hospital.
 * Date is initialized with a start date and bool which tells
 * if care period is active. Careperiod also has information on
 * working staff as well as care period end date (if it care period
 * has ended). Also bunch of getter and setter methods.
 * */


#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>
#include <set>

class CarePeriod
{
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    // Getter method to get start date of care period.
    Date get_start_date();

    // Getter method to get end date of care period.
    Date get_end_date();

    // Setter method to set end date of care period.
    void set_end_date(const Date& end);

    // Method to add staff to patient's care period.
    // Adds them in a set structure
    void add_staff(std::string);

    // Prints all staff of a care period in a desired format.
    // Takes pretext as a param to change print format slightly.
    void print_staff(const std::string& pretext);

    // Method to know if careperiod is still active.
    bool is_it_active();

    // Get name of patient who is treated in this care
    // period.
    std::string get_name();

    // Method to set care_period inactive.
    void set_careperiod_inactive();

    // Method to find, if staff is found from this
    // care period.
    bool find_staff(std::string staff_name);

    // Method to print start and end date in a desired format.
    // Takes pretext as a param to change print format slightly.
    void print_date_info(const std::string& pretext);

private:

    // Patient who'se care period this is.
    Person* patient_;

    // Start date of care period
    Date start_;

    // End date of care period.
    Date end_;

    // Staff personnel in alphabetical order in a set.
    std::set<std::string> staff_of_patient_;

    // Bool to know if period is active or inactive.
    bool is_period_active_;
};

#endif // CAREPERIOD_HH
