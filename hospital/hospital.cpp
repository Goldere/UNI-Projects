#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocating all patients
    for( std::map<std::string, Person*>::iterator
         iter = all_patients_.begin();
         iter != all_patients_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocating all care periods
    for( CarePeriod* cp : care_periods_)
    {
        delete cp;
    }
}

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

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
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
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::enter(Params params)
{
    std::string patient_id = params.at(0);

    // Patient is already in care period
    if (current_patients_.find(patient_id) != current_patients_.end()){
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }

    // Check whether the patient has been in the hospital before
    // If not, add him
    if (all_patients_.find(patient_id) == all_patients_.end()){
        all_patients_[patient_id] = new Person(patient_id);
    }

    //  Same procedures for new/old patients
    //  Adding to the current patients and creating a new care period
    Person* new_patient = all_patients_.find(patient_id)->second;
    current_patients_[patient_id] = new_patient;

    CarePeriod* care_period = new CarePeriod(utils::today, new_patient);
    care_periods_.push_back(care_period);

    std::cout << PATIENT_ENTERED << std::endl;
}

void Hospital::leave(Params params)
{
    std::string patient_id = params.at(0);

    // Check if the patient is currently in the hospital
    if (current_patients_.find(patient_id) == current_patients_.end()){
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // End the care period
    // Find the care period corresponding to the patient
    CarePeriod* care_period = nullptr;
    for (CarePeriod* cp : care_periods_) {
        if (cp->get_patient() == all_patients_[patient_id]
                and cp->period_ended() == false) {
            care_period = cp;
            break;
        }
    }

    if (care_period) {
       care_period->end_period(utils::today);
    }

    std::cout << PATIENT_LEFT << std::endl;

    // Remove the patient from the hospital's current patients
    current_patients_.find(patient_id)->second = nullptr;
    current_patients_.erase(current_patients_.find(patient_id));

    care_period = nullptr;
}

void Hospital::assign_staff(Params params)
{
    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);

    // Check if the staff member is in the hospital
    if (staff_.find(staff_id) == staff_.end()){
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    // Check if the patient is currently in the hospital
    if (current_patients_.find(patient_id) == current_patients_.end()){
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // Find the care period corresponding to the patient
    CarePeriod* care_period = nullptr;
    for (CarePeriod* cp : care_periods_) {
        if (cp->get_patient() == all_patients_[patient_id]
                and cp->period_ended() == false) {
            care_period = cp;
            break;
        }
    }

    // Adding a staff member
    if (care_period) {
        Person* staff_member = staff_[staff_id];
        // Adding to a care period
        care_period->add_staff(staff_id, staff_member);
    }

    std::cout << STAFF_ASSIGNED << patient_id << std::endl;

    care_period = nullptr;

}

void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);

    // Check if the patient is in the hospital
    if (all_patients_.find(patient_id) == all_patients_.end()){
        std::cout << CANT_FIND <<patient_id << std::endl;
        return;
    }

    // Print the care periods
    for (CarePeriod* cp : care_periods_) {
        if (cp->get_patient() == all_patients_[patient_id]) {
            std::cout << "* Care period: ";
            cp->print_patient_care_period();
        }
    }

    // Print medicines
    std::cout << "* Medicines:";
    all_patients_.find(patient_id)->second->print_medicines("  - ");
}

void Hospital::print_care_periods(Params params)
{
    std::string staff_id = params.at(0);

    bool care_period_found = false;

    // Check if the staff member is in the hospital
    if (staff_.find(staff_id) == staff_.end()){
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    // Check if staff member has been recruited to any care period
    for (CarePeriod* cp : care_periods_) {
        if (cp->find_staff_id(staff_id)) {
            care_period_found = true;

            // Staff member is recruited so we can print out ongoing care periods
            if (cp->period_ended() == false) {
                cp->print_staff_care_period();
            }
        }
    }

    // If no care periods found, print "None"
    if (!care_period_found) {
        std::cout << "None" << std::endl;
        return;
    }

    // Print ended care periods
    for (CarePeriod* cp : care_periods_) {
        if (cp->find_staff_id(staff_id) and cp->period_ended()) {
            cp->print_staff_care_period();
        }
    }
}

void Hospital::print_all_medicines(Params)
{
    // Create a map for medicines and the patients linked with them
    std::map<std::string, std::set<std::string>*>* medicines_map
            = new std::map<std::string, std::set<std::string>*>();

    // Iterate over all patients
    for (const auto& pair : all_patients_) {       
        // Get the patient's medicines
        std::vector<std::string> patient_medicines
                = pair.second->get_medicines();

        // Add the medicine and all the patients related to it to the map
        for (const std::string& medicine : patient_medicines) {

            // Check whether the medicine is in the map
            // If not, define a new set with the medicine
            if (medicines_map->find(medicine) == medicines_map->end()) {      
                (*medicines_map)[medicine] = new std::set<std::string>;
            }      

            // Add the patient to the medicine's set
            (*medicines_map)[medicine]->insert(pair.first);
        }
    }

    // Print the medicines and the patients related with them in alphabetical order
    if (medicines_map->empty()) {
        std::cout << "None" << std::endl;
    }
    else {
        for (const auto& pair : *medicines_map) {     
            std::cout << pair.first << " prescribed for" << std::endl;   
            for (const std::string& patient : *(pair.second)) {
                std::cout << "* " << patient << std::endl;
            }

            delete pair.second;
        }
    }

    // Release the memory and pointer
    delete medicines_map;
    medicines_map = nullptr;
}

void Hospital::print_all_patients(Params)
{
    // Check if there have been any patients in the hospital
    if (all_patients_.empty()){
        std::cout << "None" << std::endl;
    }

    // Iterate through all patients and their care periods
    for (const auto& pair : all_patients_) {
        std::cout << pair.second->get_id() << std::endl;

        for (CarePeriod* cp : care_periods_) {        
            // If care period is patient's, print out its information
            if (cp->get_patient() == all_patients_[pair.second->get_id()]) {
                std::cout << "* Care period: ";
                cp->print_patient_care_period();
            }
        }

        // Print medicines
        std::cout << "* Medicines:";
        all_patients_.find(pair.second->get_id())
                ->second->print_medicines("  - ");
    }
}

void Hospital::print_current_patients(Params)
{
    // Check if there are any patients in the hospital
    if (current_patients_.empty()){
        std::cout << "None" << std::endl;
    }

    // Iterate through all current patients and their care periods
    for (const auto& pair : current_patients_) {
        std::cout << pair.second->get_id() << std::endl;

        for (CarePeriod* cp : care_periods_) {   
            // If care period is patient's, print out its information
            if (cp->get_patient() == current_patients_[pair.second->get_id()]) {
                std::cout << "* Care period: ";
                cp->print_patient_care_period();
            }
        }

        // Print medicines
        std::cout << "* Medicines:";
        current_patients_.find(pair.second->get_id())
                ->second->print_medicines("  - ");
    }
}

