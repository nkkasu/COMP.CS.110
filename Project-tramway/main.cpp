/* Tramway
 *
 * Description:
 * User gives a file name that program tries to open. If text file is
 * not found, program ends into EXIT_FAILURE. If text file is found,
 * information in file is parsed into a data structure. Information is
 * only parsed into a data structure, if information is in the right
 * format in the text file. If format of any of the lines is incorrect,
 * program ends into a EXIT_FAILURE. After file is read and put into a
 * data structure, command interface will be run. User can give different
 * commands in the command interface, such as Lines, stops, Line <linename>
 * Distance <line> <stop1> <stop2>, Stops, STOP <stopname>, Stop <stopname>
 * addline <linename>, addstop <linename> <stopname> remove <stopname>.
 * Longer stop/line names can be put around quote marks and will be handled
 * as one stop/line. Program will check that user input is in a correct format
 * and user gives right input values. If values are incorrect, program will
 * print accordingly. If values are correct, commands are processed and put
 * into different functions depending on command. Functions do different things
 * depending on command. Command can print all lines for example. User is prompted
 * of commands until user gives command QUIT. If user gives quit command program is
 * terminated with EXIT_SUCCESS.
 *
 * Program Author
 * Name: Niilo Kasurinen
 * Student number: H291879
 * UserID: rfnika
 * E-Mail: niilo.kasurinen@tuni.fi
**/

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <algorithm>

// Making structures look more manageable
using std::map;
using std::string;
using std::vector;
using std::pair;

// To make code look prettier
using Tramway = map < string, vector < pair < string, float >>>;

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Splits the string by chosen character (delimiter). Has option to include empty spaces
// with a boolean, which means that final vector may have empty strings in it. Function
// is taken from earlier exercise, therefore not made by me.
std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false){
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

// Changes all lowercase letters in a string to uppercase and returns uppercase
// version of that said string. Loop through each letter in a string and make it
// uppercase. Concatenate each letter to final word one by one and return final word.
string word_to_upper(const string& word)
{
    string final_word = "";

    for (size_t i = 0; i < word.size(); ++i)
    {
        final_word += toupper(word.at(i));
    }
    return final_word;
}

// Processes user input and adds words of user input into a vector.
// String around a quotation marks is classified as one part.
// Example: user input: Distance Tampere "Hervannan kampus" "Tampereen rautatieasema"
// Is put into a vector the following way:
// {Distance, Tampere, Hervannan kampus, Tampereen rautatieasema}
vector<string> process_user_input(const string& input)
{
    // Final vector that is returned
    vector<string> final = {};
    std::size_t find_comma = input.find('"');

    if (find_comma == std::string::npos)
    {
        return split(input, ' ', true);
    }

    vector<string> temp_vec = {};

    // Quotation marks were found. Split word by quotation marks.
    vector<string> split_by_quote_mark = split(input, '"', true);

    // Loop through each word that is split by quotation mark.
    for (string word : split_by_quote_mark)
    {
        // Words that didn't have quotation marks around them have
        // Empty space as the end.
        if (word.back() == ' ')
        {
            temp_vec = split(word, ' ', true);

            for (string wrd_split_space : temp_vec)
            {
                final.push_back(wrd_split_space);
            }
        }
        // No empty spaces, so it's a word around quotation marks
        else
        {
            final.push_back(word);
        }
    }
    return final;
}

// Checks if we can add stop to a line. Stop can be added if there isn't same named stop
// in a line or if a stop doesn't have same distance from a starting point as another stop.
// If stop name or distance in a pair is same as the one we want to add, returns false.
bool check_if_add_allowed(Tramway& tramway_data, const string& stop_name, const string& line_name,
                          const string& distance)
{
    for (pair<string, float> pair : tramway_data.at(line_name))
    {
        // Check if same named or same distance.
        if (pair.first == stop_name || pair.second == stof(distance))
        {
            std::cout << "Error: Stop/line already exists." << std::endl;
            return false;
        }
    }
    return true;
}

// Function that checks if formatting in a text file line is correct.
// If formatting is incorrect, returns false.
// Also checks if same named stop already exists in a line.
// Also checks if the distance is same. If distance or
// name is same as some other stop in a line, returns false.
bool check_line_format(Tramway& tramway_data, const vector<string>& line)
{
    string distance = "0";

    // First check for correct line size (split by semicolon)
    if (line.size() > 3 || line.size() < 2)
    {
        std::cout << "Error: Invalid format in file.";
        return false;
    }
    // Check for correct information in LINES or STOPS in a file line.
    if (line.at(0) == "" || line.at(1) == "")
    {
        std::cout << "Error: Invalid format in file." << std::endl;
        return false;
    }

    // Assign value to a distance depending on what third element is.
    if (line.size() == 3 && line.at(2) != "")
    {
        distance = line.at(2);
    }

    // Format should be right. Next check if line exists.
    if (tramway_data.count(line.at(0)) != 0)
    {
        if (!(check_if_add_allowed(tramway_data, line.at(1), line.at(0), distance)))
        {
            return false;
        }
    }
    // Return true if all checks are passed.
    return true;
}
// Important function that creates a container if file formatting is correct.
// Container type is Tramway = map < string, vector < pair < string, float >>>
// Container is used in almost all functions of the program. Stores all the
// important information of a tramway (lines, stops of each line, distance
// of stop from starting point)
bool create_container(Tramway& tramway_data, const vector<string>& file_lines)
{
    for (string line : file_lines)
    {
        vector<string> fields = split(line, ';');

        if (!check_line_format(tramway_data, fields))
        {
            return false;
        }

        // Check if line is already in a container. If not, add it
        if (tramway_data.count(fields.at(0)) == 0)
        {
            tramway_data.insert({fields.at(0), {}});
        }
        // If vector size is 2, format is like this: {LINE, STOP}
        if (fields.size() == 2)
        {
            // Add stop to a line with a distance of 0.
            tramway_data.at(fields.at(0)).push_back({fields.at(1), 0});

        }
        // If format of row is like this: {LINE, STOP, ""}
        else if (fields.at(2) == "")
        {
            tramway_data.at(fields.at(0)).push_back({fields.at(1), 0});
        }
        // Format is like this: {LINE, STOP, DISTANCE}.
        else
        {
            // Add stop to a line with distance of DISTANCE.
            tramway_data.at(fields.at(0)).push_back({fields.at(1), stof(fields.at(2))});
        }
    }
    return true;
}

// Asks user of a file name. Opens it if possible. If not, return false which ends
// program. Stores lines of a file into a vector. File lines are then used to create
// a tramway container of type map < string, vector < pair < string, float >>>
// in a function named create_container, if formatting is correct in a text file.
bool read_file(Tramway& tramway_data)
{
    std::string file_name = "";
    vector<string> file_lines_in_vector = {};
    std::cout << "Give a name for input file: ";

    getline(std::cin, file_name);
    std::ifstream file_object(file_name);
    // Try opening a file.
    if (not file_object)
    {
        std::cout << "Error: File could not be read.";
        return false;
    }

    std::string file_line = "";
    // Add all lines of a file into a vector.

    while(getline(file_object, file_line))
    {
        file_lines_in_vector.push_back(file_line);
        vector<string> splitted = split(file_line, ';');
    }
    file_object.close();

    // File lines added into a vector. Now let's add all information
    // into a structure if format of file lines is valid. If not, return false.
    if (!(create_container(tramway_data, file_lines_in_vector)))
    {
        return false;
    }
    return true;
}

// Command to print all tramway lines stored in a data structure.
void lines_command(const Tramway& tramway_data)
{
    std::cout << "All tramlines in alphabetical order:" << std::endl;
    for (auto line : tramway_data)
    {
        std::cout << line.first << std::endl;
    }
}

// Command to print all stops in a in a order that is dependent on a
// distance from a starting point.
void line_command(const Tramway& tramway_data, const string& line)
{
    // Try finding a line
    if (tramway_data.count(line) == 0)
    {
        std::cout << "Error: Line could not be found." << std::endl;
        return;
    }
    std::cout << "Line " << line << " goes through these stops in the order they are listed: " << std::endl;
    for (pair<string, float> stop : tramway_data.at(line))
    {
        std::cout << "- " << stop.first << " : " << stop.second << std::endl;
    }
}
// Command to print all stops found in a tramway system, alphabetically.
void print_all_stops(const Tramway& tramway_data)
{
    std::cout << "All stops in alphabetical order:" << std::endl;

    vector<string> stops = {};

    // Go through all lines
    for (auto line : tramway_data)
    {
        // Go through all stops in a line
        for (auto stop : line.second)
        {
            // If stop is not in a vector yet, add it.
            if(!(std::find(stops.begin(), stops.end(), stop.first) != stops.end()))
            {
                stops.push_back(stop.first);
            }
        }
    }
    // Sort alphabetically
    sort(stops.begin(), stops.end());
    for (vector<string>::iterator it=stops.begin(); it != stops.end(); ++it)
    {
        // Print each stop alphabetically.
        std::cout << *it << std::endl;
    }
}

// STOP command tells from which lines a stop can be found.
void stop_command(Tramway& tramway_data, string searched_stop)
{
    // Store lines in a set.
    std::set<string> lines = {};
    bool stop_found = false;
    // Line in a tramway data
    for (auto line : tramway_data)
    {
        // Stop in a line.
        for (auto stop : line.second)
        {
            // If searched stop is found in lines
            if (searched_stop == stop.first)
            {
                // Add linename to set.
                lines.insert(line.first);
                stop_found = true;
            }
        }
    }
    // Stop found in a line. Print all lines.
    if (stop_found)
    {
        std::cout << "Stop " << searched_stop
                  << " can be found on the following lines:" << std::endl;

        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            std::cout << "- " << *it << std::endl;
        }
    }
    else
    {
        std::cout << "Error: Stop could not be found." << std::endl;
    }
}
// Command that tells distance between two stops in a same line
// both stops have to be found in a same line. Also line has to be found.
void distance_command(const Tramway& tramway_data, const string& line,
                      const string& stop_one, const string& stop_two)
{
    float stop_one_distance = 0;
    float stop_two_distance = 0;
    bool stop_one_found = false;
    bool stop_two_found = false;

    // Check if line is found
    if (tramway_data.count(line) == 0)
    {
        std::cout << "Error: Line could not be found." << std::endl;
        return;
    }
    // Check if stop one is found from line.
    for (auto stop : tramway_data.at(line))
    {
        if (stop.first == stop_one)
        {
            stop_one_distance = stop.second;
            stop_one_found = true;
        }
    }
    // Check if stop two is found from line.
    for (auto stop : tramway_data.at(line))
    {
        if (stop.first == stop_two)
        {
            stop_two_distance = stop.second;
            stop_two_found = true;
        }
    }
    // Check if either of the stops hasnt been found.
    if (stop_one_found == false || stop_two_found == false)
    {
        std::cout << "Error: Stop could not be found." << std::endl;
        return;
    }
    // Print the distance between stops.
    std::cout << "Distance between " << stop_one << " and " << stop_two << " is "
              << std::abs(stop_one_distance - stop_two_distance) << std::endl;
}

// Command that adds a line without stops to the Tramway data structure.
void command_addline(Tramway& tramway_data, const string& line_name)
{
    // Check if line is already in a structure.
    if (tramway_data.count(line_name) != 0)
    {
        std::cout << "Error: Stop/line already exists." << std::endl;
        return;
    }
    // Add a line to the structure.
    tramway_data.insert({line_name, {}});
    std::cout << "Line was added." << std::endl;
}

// Adds a stop to the chosen line if line exists. There can't be a stop
// with similar name in a line. Stop in a line can't have similar distance than
// the one being added. These have to be checked.
void command_addstop(Tramway& tramway_data, const string& line_name,
                     const string& stop_name, const string& distance)
{
    int num = 0;
    // Check if line is found.
    if (tramway_data.count(line_name) == 0)
    {
        std::cout << "Error: Line could not be found." << std::endl;
        return;
    }
    // Check if it is allowed to add a stop.
    if (!(check_if_add_allowed(tramway_data, stop_name, line_name, distance)))
    {
        return;
    }
    // Find a spot where it is added in a vector. When place is found, break out of the loop
    // And add a stop to the vector in a wanted place.
    for (auto stop : tramway_data.at(line_name))
    {
        // Compare if distance of a stop is greater than one comparing to.
        if (stof(distance) > stop.second)
        {
            num += 1;
        }
        else
        {
            break;
        }
    }
    // Add stop to a vector at line LINE.
    auto it = tramway_data.at(line_name).begin() + num;
    tramway_data.at(line_name).insert(it, {stop_name, stof(distance)});
    std::cout << "Stop was added." << std::endl;

}
// Command that removes a stop from all Lines where the stop is found from.
void command_remove(Tramway& tramway_data, const string& stop_name)
{
    bool stop_found = false;
    vector<string> stops = {};
    // Add all stops into a vector.
    for (auto line : tramway_data)
    {
        for (auto stop : line.second)
        {
            if(!(std::find(stops.begin(), stops.end(), stop.first) != stops.end()))
            {
                stops.push_back(stop.first);
            }
        }
    }
    // Check if stop_name is found from all stops.
    for (string stop : stops)
    {
        if (stop == stop_name)
        {
            stop_found = true;
        }
    }
    // If stop is not found, there isn't such a stop in a data structure.
    if (stop_found == false)
    {
        std::cout << "Error: Stop could not be found." << std::endl;
        return;
    }
    // Stop is found. Remove it from all lines it is found from.
    for (auto& line : tramway_data)
    {
        int index = 0;
        for (auto& stop : line.second)
        {
            if (stop.first == stop_name)
            {
                line.second.erase(line.second.begin() + index);
                break;
            }
            index += 1;
        }
    }
    std::cout << "Stop was removed from all lines." << std::endl;
}

// Function that processes user input and provides a simple interface.
bool interface(Tramway& tramway_data)
{
    while (true)
    {
        std::string user_input = "";
        std::cout << "tramway> ";

        // Get user input
        getline(std::cin, user_input);

        // Process user input depending on if user uses quotation marks or not.
        vector<string> input_parts = process_user_input(user_input);

        // First word of user input should be command. Make word uppercase.
        string command = word_to_upper(input_parts.at(0));

        // Execute different commands depending on user input.
        if (command == "LINES")
        {
            lines_command(tramway_data);
        }
        else if (command == "LINE" && input_parts.size() == 2)
        {
            line_command(tramway_data, input_parts.at(1));
        }
        else if (command == "STOPS")
        {
            print_all_stops(tramway_data);
        }
        else if (command == "STOP" && input_parts.size() == 2)
        {
            stop_command(tramway_data, input_parts.at(1));
        }
        else if (command == "REMOVE" && input_parts.size() == 2)
        {
            command_remove(tramway_data, input_parts.at(1));
        }
        else if (command == "ADDLINE" && input_parts.size() == 2)
        {
            command_addline(tramway_data, input_parts.at(1));
        }
        else if (command == "ADDSTOP" && input_parts.size() == 4)
        {
            command_addstop(tramway_data, input_parts.at(1), input_parts.at(2), input_parts.at(3));
        }
        else if (command == "DISTANCE" && input_parts.size() == 4)
        {
            distance_command(tramway_data, input_parts.at(1), input_parts.at(2), input_parts.at(3));
        }
        else if (command == "QUIT")
        {
            return true;
        }
        else
        {
            std::cout << "Error: Invalid input." << std::endl;
        }
    }

// Short and sweet main.
}
int main()
{
    // Make a data structure for lines, stops and stop distances.
    Tramway tramway_data;
    // Prints a tram made out of ASCII characters.
    print_rasse();
    // Process file data if possible.
    if (!read_file(tramway_data))
    {
        return EXIT_FAILURE;
    }
    // Run simple interface for commands. Process commands.
    if (interface(tramway_data))
    {
        return EXIT_SUCCESS;
    }
}
