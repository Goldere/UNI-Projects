/* Education Center
 *
 * Description:
 * This program is a user command based and it can display information
 * about courses in different locations. The program reads input from
 * a text file and organizes its data into a map structure allowing
 * the user to interact with it and give commands.
 *
 * Commands:
 * locations: Display all available locations.
 * themes_in_location <location>: Display themes available in a specified location.
 * available: Display available courses with enrollments below the maximum limit.
 * courses_in_theme <theme>: Display courses based on a specified theme.
 * courses <location> <theme>: Display courses in a specified location and theme.
 *                             the theme can be typed in with double quotation marks.
 * favorite_theme: Identify and display the theme/themes with the highest total enrollments.
 * quit: Exit the program.
 *
 * The program informs the user if the command, theme or location is unknown.
 * It also informs the user if the text file doesn't include the necessary
 * information to form a database for the program.
 *
 * */

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <vector>

using namespace std;

// Variable for the maximum amount of enrollments a course can have.
const int MAX_ENROLLMENTS = 50;

struct Course {
    string name;
    string theme;
    int enrollments;

    Course(const std::string& n, const std::string& t, int e) :
        name(n), theme(t), enrollments(e) {}
};

// Splits a string into words and returns it as vector.
std::vector<std::string> split(const std::string& s, const char delimiter,
                               bool ignore_empty = false){

    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }

    return result;
}


// Checks if a line in the input file is valid
bool is_line_valid(vector <string>& strings){

    // Checks whether the last zone is "full" or positive number.
    if (strings[3] != "full"){
        for(char character : strings[3]) {
            if (!isdigit(character)){
                cout << "Error: empty field" << endl;
                 return false;
            }
        }
    }

    // Checks whether each zone includes a character, so it's not only empty spaces.
    for(string& part : strings){

        bool character_found = false;

        for (char character : part) {
            if (isalpha(character) || isdigit(character)) {
               character_found = true;
            }
        }

        if (character_found == false) {
            cout << "Error: empty field" << endl;
            return false;
        }
    }

    // Checks whether the vector has enough zones.
    if (strings.size() < 4) {
        cout << "Error: empty field" << endl;
        return false;
    }

    return true;
}

// Prints courses that are available
void print_available(map<string, vector<Course>>& course_map){

    map<string, map<string, set<string>>> available_courses;

    // Adds courses to the availables_courses if they are not full.
    for (const auto& pair : course_map){
        for (const Course& course : pair.second){
            if (course.enrollments < MAX_ENROLLMENTS) {
                // Course is not full, add it to the available_courses.
                available_courses[pair.first][course.theme].insert(course.name);
            }
        }
    }

    // Prints out courses that are available.
    for (const auto& outer_pair : available_courses){
        for (const auto& inner_pair : outer_pair.second){
             for (const auto& course_name : inner_pair.second){
                 cout << outer_pair.first << " : " <<
                         inner_pair.first << " : " << course_name << endl;
             }
        }
    }
}

// Prints courses based on location and theme.
void print_courses(map<string, vector<Course>>& course_map,
                   string location, string theme){

    // Remove double quotes
    string modified_location;
    for (char ch : location){
        if (ch != '\"'){
             modified_location += ch;
        }
    }

    // Remove double quotes
    string modified_theme;
    for (char ch : theme){
        if (ch != '\"'){
             modified_theme += ch;
        }
    }

    map<string, int> sorted_names;

    for (const auto& pair : course_map){
        // Find location
        if (pair.first == modified_location) {
            for (const Course& course : pair.second){
                // Find the theme
                if (course.theme == modified_theme){
                    // If theme's course is not added into set, add it.
                     if (sorted_names.find(course.name) == sorted_names.end()){
                          sorted_names[course.name] = course.enrollments;
                     }
                }
            }
        }
    }

    // Print out the names in alphabetical order
    for (const auto& pair : sorted_names){
        if (pair.second == MAX_ENROLLMENTS){
           cout << pair.first << " --- " << "full" << endl;
        }
        else  {
            cout << pair.first << " --- " << pair.second << " enrollments" << endl;
        }

    }
}

// Prints themes in a location
void print_themes_in_location(map<string, vector<Course>>& course_map,
                              string location){
    // Create a set so the themes can be printed out in alphabetical order
    set<string> sorted_themes;

    for (const auto& pair : course_map){
        // Find location
        if (pair.first == location) {
            // Loop through its courses
            for (const auto& course : pair.second){
                // Insert course theme to the set
                sorted_themes.insert(course.theme);
            }
        }
    }

    // Print out the names in alphabetical order
    for (const auto& name : sorted_themes){
        cout << name << endl;
    }
}

// Prints all locations
void print_locations(map<string, vector<Course>>& course_map){
    for (const auto& pair : course_map){
        cout << pair.first << endl;
    }
}

// Prints courses based on theme
void print_courses_in_theme(map<string, vector<Course>>& course_map,
                            string theme){
    // Create a set so the themes can be printed out in alphabetical order
    set<string> sorted_names;

    for (const auto& pair : course_map){
        for (const auto& course : pair.second){
            // Find the theme
            if (course.theme == theme){
                // If theme's course is not added into set, add it.
                 if (sorted_names.find(course.name) == sorted_names.end()){
                      sorted_names.insert(course.name);
                 }
            }
        }
    }

    // Print out the names in alphabetical order
    for (const auto& name : sorted_names){
        cout << name << endl;
    }
}

// Prints the favorite theme
void print_favorite_theme(map<string, vector<Course>>& course_map){

    // if text file is empty print out "no enrollments" & end the function
    if (course_map.size() == 0){
        cout << "No enrollments" << endl;
        return;
    }

    map<string, int> theme_enrollments;

    // Adds the courses theme and its enrollments to the theme_enrollments map.
    for (const auto& pair : course_map){
        for (const auto& course : pair.second){
            // If theme is not found in map, add it and give it a value.
            if (theme_enrollments.find(course.theme) == theme_enrollments.end()){
                theme_enrollments[course.theme] = course.enrollments;
            }
            else {
                theme_enrollments[course.theme] += course.enrollments;
            }
        }
    }

    int most_enrollemnts = 0;

    // Changes the value of most_enrollments based on the theme with most enrollments.
    for (const auto& pair : theme_enrollments){
        if (pair.second > most_enrollemnts) {
             most_enrollemnts = pair.second;
        }
    }

    cout << most_enrollemnts << " enrollments in themes" << endl;

    // Prints out the courses with the same amount of enrollments
    // as variable most_enrollments has.
    for (const auto& pair : theme_enrollments){
        if (pair.second == most_enrollemnts) {
             cout << "--- " << pair.first << endl;
        }
    }
}

// Checks if a theme is unknown
// Return false if theme is unknown otherwise true
bool theme_is_unknown(map<string, vector<Course>>& course_map, string theme){
    // Remove double quotes
    string modified_theme;
    for (char ch : theme){
        if (ch != '\"'){
             modified_theme += ch;
        }
    }

    // Loop through all the courses, to find the theme inside the map.
    for (const auto& pair : course_map){
        for (const Course& course : pair.second){
            if (modified_theme == course.theme){
                return false;
            }
        }
    }

    return true;
}

// Adds course to the course map
void add_course(map<string, vector<Course>>& course_map,
                vector<string> strings) {

    string key_to_find = strings[0];

    bool update_course_enrollments = false;

    //Checks whether the value is updated
    if (course_map.find(key_to_find) != course_map.end()){

        for (Course& course : course_map[strings[0]]) {

            if (course.name == strings[2] && course.theme == strings[1]){

                if (strings[3] == "full"){
                    course.enrollments = MAX_ENROLLMENTS;
                    update_course_enrollments = true;
                    break;
                }

                else {
                    course.enrollments = stoi(strings[3]);
                    update_course_enrollments = true;
                    break;
                }
            }
        }
    }


    // Define Course
    // If the course is full enrollments should be 50
    if (!update_course_enrollments) {
        if (strings[3] == "full"){
            course_map[strings[0]].push_back
                    (Course(strings[2], strings[1], MAX_ENROLLMENTS));
        } else {
            course_map[strings[0]].push_back
                    (Course(strings[2], strings[1], stoi(strings[3])));
        }
    }
}

// Handles user commands
// Outputs error message, if user inputs an incorrect command.
// Returns false if user gives command "quit" otherwise true.
bool handle_commands(map<string, vector<Course>>& course_map){

    string command_string = "";
    cout << "> ";
    getline(cin, command_string);

    // Split command into multiple strings.
    vector<string> command = split(command_string, ' ', false);


    if (command[0] == "quit") {
        return false;
    }

    else if (command[0] == "locations") {
        print_locations(course_map);
    }

    else if (command[0] == "themes_in_location"){

        // Checks whether the location is known.
        if (course_map.find(command[1]) == course_map.end()){
           cout << "Error: unknown location" << endl;
        }

        else {
            if (command.size() == 2) {
                print_themes_in_location(course_map, command[1]);
            } else {
                cout << "Error in command themes_in_location" << endl;
            }
        }
    }

    else if (command[0] == "available"){
        print_available(course_map);
    }

    else if (command[0] == "courses_in_theme") {
        if (theme_is_unknown(course_map, command[1])){
                cout << "Error: unknown theme" << endl;
        }  else {
            if (command.size() == 2){
                print_courses_in_theme(course_map, command[1]);
            } else {
                cout << "Error in command courses_in_theme" << endl;
            }
        }
    }

    else if (command[0] == "courses"){

        // Checks whether the location is known.
        if (course_map.find(command[1]) == course_map.end()){
           cout << "Error: unknown location name" << endl;
        }

        else {

            // Checks whether the theme is known for a theme
            // that consist of two words.
            if (command.size() == 4){

                if (theme_is_unknown(course_map, command[2])
                        && theme_is_unknown(
                            course_map, command[2] + " " + command[3])){
                    cout << "Error: unknown theme" << endl;
                }

                // Theme is known, prints its courses.
                else {
                    print_courses(course_map,
                                  command[1], command[2] + " " + command[3]);
                }
            }

            // Checks whether the theme is known for a theme
            // that consist of one word.
            else if (command.size() == 3){

                if (theme_is_unknown(course_map, command[2])){
                    cout << "Error: unknown theme" << endl;
                }

                // Theme is known, prints its courses.
                else {
                    print_courses(course_map, command[1], command[2]);
                }
            }
            else {
                cout << "Error: error in command courses" << endl;
            }
        }
    }

    else if (command[0] == "favorite_theme"){
        print_favorite_theme(course_map);
    }

    else {
        cout << "Error: Unknown command: " << command[0] << endl;
    }

    return true;
}


// Reads courses from an input file and fills the course map
// Returns false: file cannot be read
// Returnfs true: file is read and course map is filled with its contents
bool read_courses_from_file(map<string, vector<Course>>& course_map,
                            const string& filename) {

    ifstream file_variable(filename);

    if (!file_variable) {
        cout << "Error: the input file cannot be opened" << endl;
        return false;
    }

    string line;

    while (getline(file_variable, line)) {
        vector<string> strings = split(line, ';', false);

        // Check if the line includes location, course theme,
        // course name and amount of enrollemnts.
        if (!is_line_valid(strings)) {
            return false;
        }

        // Add course to the map
        add_course(course_map, strings);
    }

    file_variable.close();

    // File succesfully read, return true.
    return true;
}

int main() {
    map<string, vector<Course>> course_map;

    string text_file_name = "";
    cout << "Input file: ";
    getline(cin, text_file_name);

    if (read_courses_from_file(course_map, text_file_name)) {
        // Command loop
        while (handle_commands(course_map));
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

