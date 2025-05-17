const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";
const std::string RESET = "\033[0m";

void help() 
{
    std::cout << CYAN << "Available commands:\n\n" << RESET;

    std::cout << YELLOW << "help\n" << RESET;
    std::cout << "    Displays this help message.\n\n";

    std::cout << YELLOW << "rename <old_path> <new_path>\n" << RESET;
    std::cout << "    Renames a file or directory from " << CYAN << "<old_path>" << RESET << " to " << CYAN << "<new_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "makedir <directory_path>\n" << RESET;
    std::cout << "    Creates a new directory at the specified " << CYAN << "<directory_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "fileinfo <file_path>\n" << RESET;
    std::cout << "    Displays information about the specified " << CYAN << "<file_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "delete <file_path>\n" << RESET;
    std::cout << "    Deletes the specified " << CYAN << "<file_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "listdir <directory_path>\n" << RESET;
    std::cout << "    Lists the contents of the specified " << CYAN << "<directory_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "search <directory_path> <search_term>\n" << RESET;
    std::cout << "    Searches for " << CYAN << "<search_term>" << RESET << " in files within " << CYAN << "<directory_path>" << RESET << ".\n\n";

    std::cout << YELLOW << "diskusage <directory_path>\n" << RESET;
    std::cout << "    Displays the disk usage of the specified " << CYAN << "<directory_path>" << RESET << ".\n\n";
}
