/* Hospital
 *
 * Description:
 * This program is a user command based and it can display and manage
 * information about the hospital its staff members, patients,
 * patients' medicines and their care periods.
 *
 * Functionality:
 *
 * Staff Management: Users can recruit new staff members into
 * the hospital and assign them specific care period(s)
 *
 * Patient Management: Patients can enter and leave the hospital
 * multiple times.
 *
 * Medicine Management: Users can add, remove, and manage medicines prescribed
 * to patients during their care periods.
 *
 * Care Period Management: Each time a patient enters the hospital, a
 * new care period is created, and when the patient leaves,
 * the care period ends. Users can manage these by assigning staff members
 * and updating medicines.
 *
 * The program can also read input from a text file and organizes
 * its data into a map structures defined in hospital.hh.
 * ´
 * Commands:
 * Recruit staff : RECRUIT R
 * Take patient to hospital : ENTER E
 * Take patient from hospital : LEAVE L
 * Assign staff for a patient : ASSIGN_STAFF AS
 * Add medicine for a patient : ADD_MEDICINE AM
 * Remove medicine from a patient : REMOVE_MEDICINE RM
 * Print patient's info : PRINT_PATIENT_INFO PPI
 * Print care periods per staff : PRINT_CARE_PERIODS PCPS
 * Print all used medicines : PRINT_ALL_MEDICINES PAM
 * Print all staff : PRINT_ALL_STAFF PAS
 * Print all patients : PRINT_ALL_PATIENTS PAP
 * Print current patients : PRINT_CURRENT_PATIENTS PCP
 * Set date : SET_DATE SD
 * Advance date : ADVANCE_DATE AD
 * Read : READ_FROM RF
 * Help : HELP H
 * Quit : QUIT Q
 *
 * The program informs the user if the command is unknown or if the
 * command receives wrong amount of parameters.
 *
 * */

#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
