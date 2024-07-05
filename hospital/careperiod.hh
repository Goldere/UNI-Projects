/* Class CarePeriod
 * ----------
 * Class for describing a patient's care period in hospital.
 *
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>

class CarePeriod
{
public:
    /**
      * @brief CarePeriod
      * @param start date given as a string (ddmmyyyy)
      * @param patient whose care period will be created
      */
    CarePeriod(const std::string& start, Person* patient);

    /**
      * @brief CarePeriod
      * @param start date given as a Date object
      * @param patient whose care period will be created
      */
    CarePeriod(const Date& start, Person* patient);

    /**
      * @brief destructor
      */
    ~CarePeriod();

    /**
     * @brief add_staff
     * Add staff member to the patient's current care period.
     * @param patient_id
     * @param nurse_id
     */
    void add_staff(std::string staff_name, Person* staff_member_id);

    /**
     * @brief add_staff
     * Ends the care period, sets the end date as the current date
     * And sets bool value ended_ to true
     */
    void end_period(const Date& end);

    /**
     * @brief period_ended
     * @return true if care period is ended, otherwise false
     */
    bool period_ended();

    /**
     * @brief print_patient_care_period
     * Prints all the patients' care periods
     */
    void print_patient_care_period();

    /**
     * @brief print_staff_care_period
     * Prints care periods to which a staff member has been assigned
     */
    void print_staff_care_period();

    /**
     * @brief find_staff_id
     * looks for a staff member with the given id inside the care period
     * @return true if staff member has worked in this care period
     * , otherwise false
     */
    bool find_staff_id(std::string staff_id);

    /**
     * @brief get_patient
     * @return the value of patient_
     */
    Person* get_patient();

private:
    Person* patient_;
    Date start_;
    Date end_;

    std::map<std::string, Person*> staff_members_;
    bool ended_ = false;
};

#endif // CAREPERIOD_HH
