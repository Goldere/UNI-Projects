#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

void CarePeriod::add_staff(std::string staff_name, Person* staff_member_id)
{
    staff_members_[staff_name] = staff_member_id;
}

void CarePeriod::end_period(const Date &end){
    end_ = end;
    ended_ = true;
}

bool CarePeriod::period_ended()
{
    return ended_;
}

void CarePeriod::print_patient_care_period()
{
    start_.print();
    std::cout <<" - ";
    end_.print();
    std::cout << std::endl;

    // Check if there are any staff_members
    // If not print "None"
    std::cout << "  - Staff:";
    if (staff_members_.empty()){
         std::cout << " None" << std::endl;
         return;
    }

    // Print out staff members
    for (const auto& staff_member : staff_members_){
        std::cout << " ";
        staff_member.second->print_id();
    }
    std::cout << std::endl;
}

void CarePeriod::print_staff_care_period()
{
    // Print period date
    start_.print();
    std::cout <<" - ";
    end_.print();
    std::cout << std::endl;

    // Print period patients
    std::cout << "* Patient: ";
    patient_->print_id();
    std::cout << std::endl;
}

bool CarePeriod::find_staff_id(std::string staff_id)
{
    if (staff_members_.find(staff_id) == staff_members_.end()) {
        return false;
    }  
    return true;
}

Person* CarePeriod::get_patient(){
    return patient_;
}
