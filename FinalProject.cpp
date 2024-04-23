#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
/*
   Developer: Delfino Jimenez-Vasquez, Laurenz Becker
   Date: March 15, 2024
   COP 2006 - CRN 13969 - Programming I - Spring 2024
   Group Project: Login/Registration system
   Description:
       This program will ask the user if they are a new user, and if they are, then will ask them to create a username
       and password. Then the program will write the information in a file called Login.txt to store. Afterward,
       the user can then log in to their newly made account.
   Sources:
       Appending a file: https://youtu.be/4Vsp22HQYhw?si=1EVwci1_Dki9Y1s0
       Uppercase the input: https://youtu.be/tQWYTU81uAs?si=mtldx4RVCJ7L6FYO
       https://stackoverflow.com/questions/6605282/how-can-i-check-if-a-string-has-special-characters-in-c-effectively
       Editing a File: https://youtu.be/Ka5Rgfxw9JM?si=L2kLNhT12W4as2OQ
*/
//Checks if the user has any white space in the input
bool whitespace(const std::string& line)
{
    //repeats based on how many characters are in the input
    for(char character : line)
    {
        //if the character is empty, then it will return true
        if (character == ' ')
        {
            return true;
        }
    }
    //otherwise, the bool function returns false
    return false;
}
//Checks if the inputted password meets the criteria for a secure password
bool password_reqs(const std::string& password)
{
    //Declares bool variables. Upper is used to check for uppercase, and number is for any integers
    bool upper = false;
    bool number = false;
    //checks if the password has any special characters. npos means there were no special characters.
    if(password.find_first_of("!@#$%^&*") != std::string::npos)
    {
        //range-based for loop
        for (char character: password)
        {
            //if the character is uppercase, upper is true
            if (isupper(character))
            {
                upper = true;
            }
            //if the character is a digit,
            if(isdigit(character))
            {
                number = true;
            }
        }
    }
    //if both conditions are met, then the password is good
    if(number && upper && password.size()>8 && password.size()<28)
    {
        return true;
    }
    //otherwise, the password is not good enough
    return false;
}
//Depending on the change flag, the function either changes the username, password, or deletes both from the Logins file
void file_edit(int flag, std::string* credit, std::string* input)
{
    //Declares variables
    bool invalid_input = true;
    std::string ID, Pass, new_input;
    //creates vector that will hold a back-up of the file contents
    std::vector<std::string> back_up_lines;
    //opens login file to copy contents into vector
    std::ifstream vector_add("Logins.txt");
    switch (flag) {
        //if flag is 0, the user's account will not be written in the copy
        case 0:
            //adds every line in the file into the vector
            while (vector_add >> ID >> Pass)
            {
                //if the password inputted by the user matches the system record, the account will be skipped in the
                //archiving process
                if (ID == *credit && Pass == *input) {
                    continue;
                }
                    //otherwise, the account is not the user's and is saved to the vector
                else {
                    back_up_lines.push_back(ID);
                    back_up_lines.push_back(Pass);
                }
            }
            vector_add.close();
            break;
            //if the change flag is , then the username is being changed
        case 1:
            while(invalid_input)
            {
                //Asks user to enter new username
                std::cout << "Enter a new username.\nUsername cannot have whitespaces and between 8-28 characters.\n";
                std::getline(std::cin, new_input);
                //checks if the username is within the size limit and contains no white space
                if(new_input.size() > 28 || new_input.size() < 8 || whitespace(new_input))
                {
                    std::cout << "Invalid username. Try again.\n";
                }
                    //if it is valid, then it will check if the username is taken
                else
                {
                    //opens the login file
                    std::ifstream info_check("Logins.txt");
                    //scans the lines within the file to find the username
                    while (info_check >> ID >> Pass) {
                        //if it finds the username, the check_result is set to true, and stops the search,
                        //then tells the user to try again.
                        if (ID == new_input) {
                            invalid_input = true;
                            std::cout << "Username is taken.\n";
                            break;
                        }
                        invalid_input = false;
                    }
                    //closes the login file
                    info_check.close();
                }
            }


            //Then it will copy over the data from the file in the vector
            while (vector_add >> ID >> Pass)
            {
                //once it finds the original account information, it will instead add the new username and old password
                if (ID == *input && Pass == *credit) {
                    back_up_lines.push_back(new_input);
                    back_up_lines.push_back(Pass);
                }
                    //otherwise, it will copy over the other accounts
                else {
                    back_up_lines.push_back(ID);
                    back_up_lines.push_back(Pass);
                }
            }
            //changes the username in the user_option parameters to keep the system up to date.
            *input = new_input;
            vector_add.close();
            break;
            //if the change flag is set to 2, then the password is being changed.
        case 2:
            while(invalid_input)
            {
                //Asks the user for a password
                std::cout << "Enter a new password.\nPassword must be 8-28 characters long with no spaces, a "
                             "capital letter, numbers, and a special\ncharacter: !, @, #, $, %, &, *.\n";
                std::getline(std::cin, new_input);
                //checks if the password meets the requirements and contains no whitespace
                if (password_reqs(new_input) && !whitespace(new_input)){
                    invalid_input = false;
                }
                    //otherwise the password is invalid
                else
                {
                    std::cout << "Invalid password. Try again.\n";
                }
            }
            //copies file content over into the vector
            while (vector_add >> ID >> Pass)
            {
                //once the account is found, the new password is saved instead of the old one.
                if (ID == *credit && Pass == *input) {
                    back_up_lines.push_back(ID);
                    back_up_lines.push_back(new_input);
                }
                    //otherwise, the other accounts remain untouched
                else {
                    back_up_lines.push_back(ID);
                    back_up_lines.push_back(Pass);
                }
            }
            //changes the password in the user_option parameters to keep the system up to date.
            *input = new_input;
            vector_add.close();
            break;
            //In case the change_flag variable is any other number besides 0, 1, 2.
        default:
            std::cout << "Error has occurred.\nManually terminate program.\n";
            vector_add.close();
    }
    //Adds the vector contents back into the login file.
    std::ofstream re_add("Logins.txt");
    for (int count = 0; count < back_up_lines.size(); count += 2)
    {
        re_add << back_up_lines[count] << ' ' << back_up_lines[count + 1] << std::endl;
    }
    re_add.close();
    //clears vector to free up memory.
    back_up_lines.clear();
}
//Registers user into the system, by asking for a valid username and password.
void registration()
{
    bool return_check=false;
    std::string username, password, un, check;
    //Tells readers username requirements
    std::cout << "Username length must be 8-28 characters, and spaces are not allowed.\n";
    while (!return_check)
    {
        //Declares variable and sets to true
        bool valid_username = true;
        //Asks user for username
        std::cout << "Enter a username:\n";
        //collects username from user
        std::getline(std::cin, username);
        check = username;
        std::transform(check.begin(), check.end(), check.begin(), toupper);
        if(check == "RETURN")
        {
            std::cout << "RETURNING TO MAIN MENU...\n";
            return_check = true;
        }
            //calls the whitespace bool function and checks if it has whitespace
        else if (whitespace(username))
        {
            std::cout << "Cannot have whitespace try again.\n";
        }
            //Checks if username is less than 28 characters
        else if (username.size() > 28 || username.size() < 8)
        {
            std::cout << "Username length is invalid. Try again.\n";
        }
            //otherwise, it will check if the username already exists
        else
        {
            //opens the login file
            std::ifstream info_check("Logins.txt");
            //scans the lines within the file to find the username
            while (info_check >> un)
            {
                //if it finds the username, the check_result is set to true, and stops the search,
                //then tells the user to try again.
                if (un == username)
                {
                    std::cout << "Username is taken. Try again.\n";
                    valid_username = false;
                    break;
                }
            }
            //closes the login file
            info_check.close();
            //if the username is valid, the loop ends
            if (valid_username)
            {
                break;
            }
        }
    }
    //loops until the user enters a password less than 28 characters
    while (!return_check)
    {
        //Asks user for a password and tells them the requirements to make a secure password
        std::cout << "Password must be 8-28 characters long with no spaces, at least 1 capital letter, numbers, and a "
                     "special\ncharacter: !, @, #, $, %, &, *.\n";
        std::cout << "Enter a password:\n";
        //collects user's input
        std::getline(std::cin, password);
        //if the user's password meets the requirements, the loop ends
        check = password;
        std::transform(check.begin(), check.end(), check.begin(), toupper);
        if(check == "RETURN")
        {
            std::cout << "RETURNING TO MAIN MENU...\n";
            return_check = true;
        }
        else if(password_reqs(password))
        {
            break;
        }
            //otherwise, the password is rejected and tells the users the requirements again.
        else
        {
            std::cout << "Password is invalid.\n";
        }
    }
    if(!return_check)
    {
        //opens the file Login.txt in appending mode, allowing the user to add their information
        std::ofstream reading("Logins.txt", std::ios::app);
        //Adds username and password to the Login.txt file
        reading << username << ' ' << password << std::endl;
        //closes file
        reading.close();
        //informs user that the account has been made
        std::cout << "Information has been added.\nNow you can login to your account.\n";
        std::cout << "RETURNING TO MAIN MENU...\n";
    }
}
//Actions the user can do while logged in the system, including changing username or password, and account deletion.
//Its parameters are used to store the username and password of the current session
void user_options(std::string* username, std::string* password)
{
    //Declare variables
    bool logged_in =true;
    std::string choice,return_check;
    //repeats while the user is logged in their account
    while(logged_in)
    {
        //Inside the loop to default as 0;
        int change_flag =0;
        //Tells user their options. Is also the "Home Page"
        std::cout << "What would you like to do " << *username <<'?' << std::endl;
        std::cout << "Enter CHANGE to change your username or password.\nEnter DELETE to delete your account.\n";
        std::cout << "Otherwise enter LOG OFF to exit.\n";
        //Gets user information
        std::getline(std::cin, choice);
        //Makes the user's input in all caps to check for all possible ways of typing the given options
        std::transform(choice.begin(), choice.end(), choice.begin(), toupper);
        if(choice == "LOG OFF")
        {
            //ends the loop and tell the reader, the program is logging off
            std::cout << "LOGGING OUT...\n";
            logged_in = false;
        }
            //enters account deletion process
        else if(choice == "DELETE")
        {
            while(true)
            {
                //Warns the user about account deletion, as is prompted to re-enter their password, or enter return.
                std::cout<<"WARNING: ACCOUNT DELETION IS IRREVERSIBLE\nEnter your password to confirm your decision.\n";
                //rather than automatically upper-casing the input, the user has to manually enter return in all caps.
                std::cout << "Otherwise, type out RETURN in all caps to return to the Homepage.\n";
                std::getline(std::cin, choice);
                return_check = choice;
                std::transform(return_check.begin(), return_check.end(), return_check.begin(), toupper);
                //if RETURN is entered, the user is returned to the account homepage
                if (return_check == "RETURN") {
                    std::cout << "RETURNING TO HOMEPAGE...\n";
                    break;
                }
                    //if the password is entered correctly, the account is deleted.
                else if (choice == *password)
                {
                    file_edit(change_flag, username, password);
                    std::cout << "Account Deleted.\nLOGGING OUT...\n";
                    logged_in = false;
                    break;
                }
                    //otherwise, the user is asked to try again.
                else
                {
                    std::cout << "Incorrect Password. Try again.\n";
                }
            }
        }
            //if the user enters CHANGE, then they will be asked which login info they wish to change.
        else if(choice == "CHANGE")
        {
            while(true)
            {
                //Instructs user on how to select which login credential they would like to change
                std::cout << "Enter USERNAME to change your username, or PASSWORD to change Password.\n";
                std::cout << "Or enter RETURN to go back to the homepage.\n";
                //collects user input
                std::getline(std::cin, choice);
                std::transform(choice.begin(), choice.end(), choice.begin(), toupper);
                //if return, then the user returns to homepage
                if(choice == "RETURN")
                {
                    std::cout << "RETURNING TO HOMEPAGE...\n";
                    break;
                }
                    //if username, then the flag is set to 1, and the file_edit function is called
                else if (choice == "USERNAME")
                {
                    change_flag = 1;
                    file_edit(change_flag,password, username);
                    //once completed, user is informed that the change was successful.
                    std::cout << "Username changed successfully.\n";
                    break;
                }
                    //if password, then the flag is set to 2, and the file_edit function is called
                else if (choice == "PASSWORD")
                {
                    change_flag = 2;
                    file_edit(change_flag,username, password);
                    //once completed, user is informed that the change was successful.
                    std::cout << "Password changed successfully.\n";
                    break;
                }
                    //otherwise, the user is prompted to try again.
                else
                {
                    std::cout << "Not a valid option try again.\n";
                }
            }
        }
    }
}
//Logs in the user to their account and calls the user_options once logged in
void login()
{
    //Declares variables
    bool not_logged_in = true;
    std::string username, password, ID, Pass, main_check;
    std::cout << "Welcome back User!\n";
    //repeats until condition is no longer true
    while (not_logged_in)
    {
        //opens the login file
        std::ifstream login("Logins.txt");
        //Asks user for username
        std::cout << "Enter your USERNAME:\n";
        std::getline(std::cin, username);
        main_check = username;
        std::transform(main_check.begin(), main_check.end(), main_check.begin(), toupper);
        if (main_check == "RETURN")
        {
            std::cout << "RETURNING TO MAIN MENU...\n";
            break;
        }
        //Asks for password
        std::cout << "Enter your PASSWORD:\n";
        std::getline(std::cin, password);
        //checks if the username and password have no whitespace
        if(!whitespace(username) && !whitespace(password))
        {
            //checks line by line in the login file if the user's information is correct.
            while (login >> ID >> Pass)
            {
                //if both their username and password are correct, not_logged_in is set to false, and the loop ends
                if (ID == username && Pass == password)
                {
                    not_logged_in = false;
                    break;
                }
            }
        }
        //if not_logged_in is still true, the user will be asked to try again
        if(not_logged_in)
        {
            std::cout << "Failed to Login. USERNAME or PASSWORD is incorrect. Try again.\n";
            std::cout << "If you are not registered, enter RETURN to return to the MAIN MENU.\n";
        }
            //otherwise, the user is logged in, and user_options function is called.
        else
        {
            std::cout << "Welcome " << username << std::endl;
            user_options(&username, &password);
        }
    }
}
int main()
{
    //Declare variable
    std::string choice;
    while(true)
    {
        //Greets user and gives options
        std::cout << "Welcome!\n";
        std::cout<< "If you are new, enter REGISTER.\nIf you are a returning user, enter LOGIN.\n";
        std::cout << "Otherwise, enter CLOSE to exit.\n";
        //collects user options
        std::getline(std::cin, choice);
        //Makes the user's input in all caps to check for all possible ways of typing the given options
        std::transform(choice.begin(), choice.end(), choice.begin(), toupper);
        //if the user is new, they are then prompted to create a username and password.
        if (choice == "REGISTER")
        {
            registration(); //calls the registration function
        }
            //if the user isn't new, the user will then be able to log in to their account
        else if (choice == "LOGIN")
        {
            login();
        }
            //if the user wishes to terminate the program, then they enter "close"
        else if(choice == "CLOSE")
        {
            std::cout << "Program terminated.";
            break;
        }
            //otherwise, their choice is invalid and they are prompted to try again.
        else
        {
            std::cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}


