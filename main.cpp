#include<iostream>
#include<filesystem>
#include <chrono>
#include <iomanip>
namespace fs = std::filesystem;
using namespace std;

// color coding for help()
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
void rename(fs::path& old_p, fs::path new_p)
{
    try
    {
        fs::rename(old_p, new_p);
        std::cout << "file renamed.\n";
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "File couldn't be renamed: " << e.what() << '\n';
    }
}
bool makedir(const fs::path& dir)
{
    try 
    {
        if (fs::create_directories(dir)) 
        {
            std::cout << "Directory created\n";
            return true;
        } else 
        {
            std::cout << "Directory already exists\n";
            return false;
        }
    } 
    catch (const fs::filesystem_error& e) 
    {
        std::cerr << "Error in creating Directory: " << e.what() << '\n';
        return false;
    }
}
void fileInfo(const fs::path& file_path)
{
    try
    {
        if (!fs::exists(file_path))
        {
            std::cout << "File does not exist.\n";
            return;
        }

        auto status = fs::status(file_path);
        auto fsize = fs::file_size(file_path);

        const char* sizes[] = { "B", "KB", "MB", "GB", "TB" };
        int order = 0;
        double size = static_cast<double>(fsize);
        while (size >= 1024 && order < 4) {
            size /= 1024;
            ++order;
        }


        std::string ftype;
        if (fs::is_regular_file(status)) ftype = "Regular File";
        else if (fs::is_directory(status)) ftype = "Directory";
        else if (fs::is_symlink(status)) ftype = "Symbolic Link";
        else ftype = "Other";

        auto ftime = fs::last_write_time(file_path);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        // permissions string rwxrwxrwx style
        fs::perms p = status.permissions();
        auto perms = std::string() +
            ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-") +
            ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-") +
            ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-") +
            ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-") +
            ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-") +
            ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-") +
            ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-") +
            ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-") +
            ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");

        std::cout << "File: " << file_path.filename() << '\n';
        std::cout << "Type: " << ftype << '\n';
        std::cout << "Size: " << std::fixed << std::setprecision(2) << size << " " << sizes[order] << '\n';
        std::cout << "Last Modified: " << std::put_time(std::localtime(&cftime), "%F %T") << '\n';
        std::cout << "Permissions: " << perms << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error getting file info: " << e.what() << '\n';
    }
}
bool deleteFile(const fs::path& file_path)
{
    try
    {
        if (!fs::exists(file_path))
        {
            std::cout << "File does not exist.\n";
            return false;
        }

        if (fs::remove(file_path))
        {
            std::cout << "File deleted successfully.\n";
            return true;
        }
        else
        {
            std::cout << "File could not be deleted.\n";
            return false;
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Error deleting file: " << e.what() << '\n';
        return false;
    }
}
void listDir(const fs::path& dir)
{
    try 
    {
        if (!fs::exists(dir) || !fs::is_directory(dir)) 
        {
            cerr << "Invalid directory.\n";
            return;
        }

        cout << " contents of: " << dir << '\n';

        for (const auto& entry : fs::directory_iterator(dir)) 
        {
            cout << (fs::is_directory(entry) ? "[DIR]  " : "       ");
            cout << entry.path().filename().string() << '\n';
        }
    }
    catch (const fs::filesystem_error& e) 
    {
        cerr << "Error listing directory: " << e.what() << '\n';
    }
}

void search(const fs::path& dir, const std::string& keyword)
{
    if (!fs::exists(dir) || !fs::is_directory(dir)) 
    {
        std::cerr << "Invalid directory.\n";
        return;
    }

    std::cout << "Searching in: \"" << dir << "\" for: " << keyword << '\n';

    std::error_code ec;
    fs::recursive_directory_iterator it(dir, fs::directory_options::skip_permission_denied, ec);
    fs::recursive_directory_iterator end;

    while (it != end) 
    {
        const auto current_path = it->path();  

        if (current_path.filename().string().find(keyword) != std::string::npos) 
        {
            std::cout << current_path << '\n';
        }

        it.increment(ec);
        if (ec) 
        {
            std::cerr << "Skipped: " << ec.message() << " at " << current_path << '\n';
            ec.clear();
        }
    }
}


uintmax_t getDiskUsage(const fs::path& dir) 
{
    uintmax_t total_size = 0;
    try {
        if (!fs::exists(dir) || !fs::is_directory(dir)) 
        {
            std::cerr << "Invalid directory.\n";
            return 0;
        }//m
        for (const auto& entry : fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied)) {
            try {
                if (fs::is_regular_file(entry.status()))
                {
                    total_size += fs::file_size(entry);
                }
            } 
            catch (const fs::filesystem_error& e) 
            {
                std::cerr << "Skipped: " << entry.path() << " due to " << e.what() << '\n';
            }
        }
    } catch (const fs::filesystem_error& e) 
    {
        std::cerr << "Error during disk usage calculation: " << e.what() << '\n';
    }
    return total_size;
}
void printReadableSize(uintmax_t size) 
{
    const char* sizes[] = { "B", "KB", "MB", "GB", "TB" };
    int order = 0;
    double dsize = static_cast<double>(size);
    while (dsize >= 1024 && order < 4) 
    {
        dsize /= 1024;
        ++order;
    }
    std::cout << std::fixed << std::setprecision(2) << dsize << " " << sizes[order] << '\n';
}



int main(int argc, char* argv[])
{
    if(argc < 2) 
    {
        help();
        return 1;
    }
    
    string command = argv[1];

    if (command == "help")
    {
        help();
    }
    else if (command == "rename")
    {
        rename(fs::path(argv[2]), fs::path(argv[3]));
    }
    else if (command == "makedir")
    {
        makedir(fs::path(argv[2]));
    }
    else if (command == "fileinfo")
    {
        fileInfo(fs::path(argv[2]));
    }
    else if (command == "delete")
    {
        deleteFile(argv[2]);
    }
    else if (command == "listdir")
    {
        listDir(argv[2]);
    }
    else if (command == "search")
    {
        search(argv[2], argv[3]);
    }
    else if (command == "diskusage")
    {
        uintmax_t size = getDiskUsage(fs::path(argv[2]));
        std::cout << "Disk usage of " << argv[2] << ": ";
        printReadableSize(size);
    }

}
