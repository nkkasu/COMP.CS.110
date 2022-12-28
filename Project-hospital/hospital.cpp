#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>

// Constructor
Hospital::Hospital()
{
}

// Destructor to deallocate dynamic memory of
// Person* and CarePeriod*
Hospital::~Hospital()
{
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }
    for (std::map<std::string, Person*>::iterator
         iter = alltime_patients_.begin();
         iter != alltime_patients_.end();
         ++iter )
    {
        delete iter->second;
    }
    for (std::vector<CarePeriod*>::iterator
         iter = care_periods_in_order_.begin();
         iter != care_periods_in_order_.end();
         ++iter )
    {
        delete *iter;
    }
}

// Used to recruit new staff. If staff already exists,
// print that staff member already exists.
void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);

    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }

    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

// Add entering patient to a hospital. If patient to be added is currently
// in the hospital, prints that patient already exists in hospital.
// If patient is new, add patient to alltime_patients_.
// If patient is only re-entering hospital, add him to current_patients.
void Hospital::enter(Params params)
{
    std::string patient_name = params.at(0);

    // Find if patient is currently in the hospital.
    if (current_patients_.find(patient_name) != current_patients_.end())
    {
        std::cout << ALREADY_EXISTS << patient_name << std::endl;
        return;
    }
    std::cout << PATIENT_ENTERED << std::endl;

    // Try finding patient from alltime patients.
    if (alltime_patients_.find(patient_name) != alltime_patients_.end())
    {
        re_enter_patient(patient_name);
        return;
    }
    enter_new_patient(patient_name);
}

// Used to enter patient that is returning back to the hospital.
// because patient is returning, we can take a Person* from alltime_patients_,
// but returning patient needs a new careperiod. Patient is put into
// current_patients_.
void Hospital::re_enter_patient(std::string patient_name)
{
    Person* new_patient = alltime_patients_.at(patient_name);
    current_patients_.insert({patient_name, new_patient});
    // Add a new careperiod to a patient

    CarePeriod* new_care_period = new CarePeriod(utils::today, new_patient);
    care_periods_.at(patient_name).push_back(new_care_period);
    care_periods_in_order_.push_back(new_care_period);
}

// Used to enter patient that is completely new one. Patient is new, so
// we need to create an entirely new Person* object, as well as new CarePeriod*.
// Put new patient into current_patients_ as well as alltime_patients_.
void Hospital::enter_new_patient(std::string patient_name)
{
    Person* new_patient = new Person(patient_name);
    current_patients_.insert({patient_name, new_patient});
    add_alltime_patient(patient_name, new_patient);

    CarePeriod* new_care_period = new CarePeriod(utils::today, new_patient);

    std::vector<CarePeriod*> care_periods_vector = {};
    care_periods_vector.push_back(new_care_period);
    care_periods_.insert({patient_name, care_periods_vector});

    care_periods_in_order_.push_back(new_care_period);
}

// Add a new alltime patient into a data structure.
void Hospital::add_alltime_patient(std::string patient_name, Person* new_patient)
{
    // Try finding if patient already exists in a alltime_patients_.
    if (alltime_patients_.find(patient_name) != alltime_patients_.end())
    {
        return;
    }
    alltime_patients_.insert({patient_name, new_patient});
}

// Used when current patients leave a hospital. Set end date to careperiod
// and set the care period inactive. Remove patient from current_patients_.
void Hospital::leave(Params params)
{
    std::string patient_name = params.at(0);
    // Try finding a patient name from current_patients_.
    if (current_patients_.find(patient_name) != current_patients_.end())
    {
        // Update leave date to careperiod.
        care_periods_.at(patient_name).back()->set_end_date(utils::today);

        // Care period has ended, set it inactive.
        care_periods_.at(patient_name).back()->set_careperiod_inactive();

        // Erase patient from current patients.
        current_patients_.erase(patient_name);

        std::cout << PATIENT_LEFT << std::endl;
        return;
    }
    std::cout << CANT_FIND << patient_name << std::endl;
}

// Assign new staff to a patient if patient exists or if
// staff exists.
void Hospital::assign_staff(Params params)
{
    std::string staff_name = params.at(0);
    std::string patient_name = params.at(1);

    // Check if user gave existing staff member.
    if (staff_.find(staff_name) == staff_.end() )
    {
        std::cout << CANT_FIND << staff_name << std::endl;
        return;
    }

    // Check if user gave existing patient.
    if (current_patients_.find(patient_name) == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient_name << std::endl;
        return;
    }

    // Add staff for a chosen patient. Last CarePeriod* element in a vector
    // is always the currently active one, so we can take it.
    care_periods_.at(patient_name).back()->add_staff(staff_name);
    std::cout << STAFF_ASSIGNED << patient_name << std::endl;
}
// Add medicine to a Person* patient.
void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);

    // Check if given input is proper numeric value. Print accordingly.
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }

    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }

    // Add medicine to patient.
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}
// Remove chosen medicine from a Person* patient, if patient has
// that medicine in use.
void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);

    // Try finding a patient
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }

    // Remove medicine from a patient.
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

// Used to print patient info, if patient info is found.
// Basically prints all CarePeriods, active and inactive,
// all staff assigned to a patient, as well as all the
// medicines currently in use.
void Hospital::print_patient_info(Params params)
{
    std::string patient_name = params.at(0);

    if(care_periods_.find(patient_name) != care_periods_.end() )
    {
        for (CarePeriod* care_period : care_periods_.at(patient_name))
        {
            // Print care period info
            care_period->print_date_info("* Care period: ");
            care_period->print_staff("  - Staff: ");
        }
        std::cout << "* Medicines:";
        alltime_patients_.at(patient_name)->print_medicines("  - ");
        return;
    }
    // Patient can't be found.
    std::cout << CANT_FIND << patient_name << std::endl;
}

// Print care periods, where staff has been assigned to.
// Prints start and end date of periods, as well as patient name.
void Hospital::print_care_periods_per_staff(Params params)
{
    // Bool to know if staff has been assigned to any care period.
    bool is_found = false;

    std::string staff_name = params.at(0);
    if (staff_.find(staff_name) != staff_.end() )
    {
        for (CarePeriod* care_period : care_periods_in_order_)
        {
            if (care_period->find_staff(staff_name))
            {
                care_period->print_date_info("");
                std::cout << "* Patient: " << care_period->get_name() << std::endl;
                is_found = true;
            }
        }
        if (!is_found)
        {
            std::cout << "None" << std::endl;
        }
        return;
    }
    std::cout << CANT_FIND << staff_name << std::endl;
}
// Used to create a set containing of all meds used by patients.
std::set<std::string> Hospital::make_set_of_meds()
{
    std::set<std::string> meds_set;
    for (std::pair<std::string, Person*> patient_pair : alltime_patients_)
    {
        std::vector<std::string> meds = patient_pair.second->get_medicines();
        for (std::string med : meds)
        {
            meds_set.insert(med);
        }
    }
    return meds_set;
}

void Hospital::print_all_medicines(Params)
{
    std::set<std::string> meds_set = make_set_of_meds();
    if (meds_set.size() == 0)
    {
        std::cout << "None" << std::endl;
        return;
    }

    for (std::string med_to_find : meds_set)
    {
        std::cout << med_to_find << " prescribed for" << std::endl;

        for (std::pair<std::string, Person*> pair : alltime_patients_)
        {
            for (std::string med : pair.second->get_medicines())
            {
                if (med_to_find == med)
                {
                    std::cout << "* " << pair.second->get_id() << std::endl;
                }
            }
        }
    }
}
// Function to print all staff of hospital.
void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}
// Used to print info all current patients as well as patients
// that have left the hospital.
void Hospital::print_all_patients(Params)
{
    if (alltime_patients_.empty())
    {
        std::cout << "None" << std::endl;
        return;
    }

    // Go through all patients one by one.
    for (std::pair<std::string, Person*> patient_pair : alltime_patients_)
    {
        // Print patient info.
        std::cout << patient_pair.first << std::endl;
        Params patient_as_param = {patient_pair.first};
        print_patient_info(patient_as_param);
    }
}

// Used to print info of all current patients one by one.
// Print format same to print_patient_info()
void Hospital::print_current_patients(Params)
{
    // Check for empty container.
    if (current_patients_.empty())
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (std::pair<std::string, Person*> patient_pair : current_patients_)
    {
        std::cout << patient_pair.first << std::endl;
        Params patient = {patient_pair.first};
        print_patient_info(patient);
    }
}
// Function to set date.
void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}
// Function to advance date. Goes forward in days by chosen
// amount.
void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}
