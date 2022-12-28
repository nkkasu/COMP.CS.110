## Hospital project

This program runs a hospital program. When program is run,
command line interpreter is run. User can give different commands
into command line interpreter. Input is processed and if input format
is wrong, it prints accordingly. Otherwise given command will do things
depending on command. Commands can be seen by typing "help" into CLI.
Many commands also have some abbreviations that can be seen by typing "help" when
program prompts a command. Most importantly purpose of the program is to save
patients, staff and careperiods and date of care period into a data structure. By using
different commands, user can print various information regarding patients, medicines
and staff personnel.

# Commands
Enter {patient id}, add patient into hospital
Leave {patient id}, patient leaves from hospital
Assign_staff {staff member id} {patient id}, assign staff for a patient
add_medicine {medicine name} {strength} {dosage} {patient id} add medicine to a patient.
remove_medicine {medicine name} {patient id} remove medicine from patient
print_patient_info {patient id} Prints patient's info (Careperiods, medicines)
print_care_periods {staff member id} print care periods per staff
print_all_medicines Print all used medicines
print_all_staff, Print all staff
print_all_patients, print all patients
print_current patients, print current patients
set_date, set date {day} {month} {year} sets wanted date.
advance_date {days} advances date for a chosen amount.
read_from {filename} read input commands from a file.
help, prints all commands
Quit, quits program

![image](https://user-images.githubusercontent.com/100607632/209877211-7de659ae-1cb5-40a2-bfa6-be1911a3f336.png)

