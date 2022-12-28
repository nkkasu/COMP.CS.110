#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start), end_(start), is_period_active_(true)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start), end_(start), is_period_active_(true)
{
}

CarePeriod::~CarePeriod()
{
}

Date CarePeriod::get_start_date()
{
    return start_;
}

Date CarePeriod::get_end_date()
{
    return end_;
}

void CarePeriod::set_end_date(const Date& end)
{
    end_ = end;
}

void CarePeriod::add_staff(std::string staff_personnel)
{
    // add staff into set.
    staff_of_patient_.insert(staff_personnel);
}

void CarePeriod::print_staff(const std::string& pretext)
{
    std::cout << pretext;
    // No staff for patient. Print none.
    if (staff_of_patient_.size() == 0)
    {
        std::cout << "None" << std::endl;
        return;
    }
    // Print all staff for a patient.
    for (std::string staff_name : staff_of_patient_)
    {
        std::cout << staff_name << " ";
    }
    std::cout << std::endl;
}

bool CarePeriod::is_it_active()
{
    return is_period_active_;
}

std::string CarePeriod::get_name()
{
    return patient_->get_id();
}

void CarePeriod::set_careperiod_inactive()
{
    is_period_active_ = false;
}

bool CarePeriod::find_staff(std::string staff_to_find)
{
    // Loop through all staff
    for (std::string staff_name : staff_of_patient_)
    {
        // Staff is found. return true
        if (staff_name == staff_to_find)
        {
            return true;
        }
    }
    return false;
}

void CarePeriod::print_date_info(const std::string& pretext)
{
    std::cout << pretext;
    // Print date info like format wants.
    get_start_date().print();
    std::cout << " -";
    if (!(is_it_active()) )
    {
        std::cout << " ";
        get_end_date().print();
    }
    std::cout << std::endl;
}





