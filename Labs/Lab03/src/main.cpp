// Main function to operate the program

#include <iostream>
#include "MD5.hpp"

inline void help(const char *program)
{
    /**
     * Print the help information of the program
     * @param program the name of the program
     * @return void
     */
    // Help Info
    std::cerr << "------------------------------ MD5 Help Info ------------------------------" << std::endl;

    std::string programPath(program);
    size_t pos = programPath.find_last_of("/\\");
    std::string programName = programPath.substr(pos + 1);

    std::cerr << "Usage: ./" << programName << " [-Option] [file path of the file validated] [file path of the .md5 file]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  [-h] --help" << std::endl;
    std::cerr << "  [-t] --test MD5 application" << std::endl;
    std::cerr << "  [-c] [file path of the file computed]\n"
              << "       --compute MD5 of the given file" << std::endl;
    std::cerr << "  [-v] [file path of the file validated]\n"
              << "       --validate the integrality of a given file by manual input MD5 value" << std::endl;
    std::cerr << "  [-f] [file path of the file validated] [file path of the .md5 file]\n"
              << "       --validate the integrality of a given file by read MD5 value from .md5 file" << std::endl;

    std::cerr << "---------------------------------------------------------------------------" << std::endl;
    return;
}

inline void test()
{
    /**
     * Test the MD5 application using offical test vectors to verify the correctness of the MD5 algorithm
     * @return void
     */

    std::cout << "----------------------------- Testing MD5 Correctness ------------------------------" << std::endl;
    std::cout << "[INFO]The following examples can be found in MD5 Offical Doc:RFC1321." << std::endl;
    std::vector<std::string> testStrings = {"", "a", "abc", "message digest", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};
    MD5 md5;
    for (int i = 0; i < testStrings.size(); i++)
    {
        std::string testString = testStrings[i];
        md5.compute(testString);
        std::string md5Value = md5.toString();
        std::cout << "MD5(\"" << testString << "\") = " << md5Value << std::endl;
    }
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    return;
}

inline void computeMD5(const char *filePath)
{
    /**
     * Compute the MD5 value of the given file
     * @param filePath the file path of the file to be computed
     * @return void
     */
    std::cout << "----------------------------- Compute MD5 for File------------------------------" << std::endl;
    MD5 md5;
    md5.compute(filePath);
    std::cout << "The MD5 value of file(\"" << filePath << "\") is:" << std::endl;
    std::cout << md5.toString() << std::endl;
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    return;
}

inline void validate(const char *validatedFile, const char *md5File = DEFAULT_MD5_FILE, INT Approach = MANUAL)
{
    /**
     * Validate the integrality of the given file by manual input MD5 value
     * @param validatedFile the file path of the file to be validated
     * @param md5File the file path of the .md5 file
     * @return void
     */
    switch (Approach)
    {
    case MANUAL:
        /* code */
        break;
    case AUTO:
        /* code */
        break;
    default:
        // If the approach is not defined, print the help info to instruct the user
        help("MD5");
        break;
    }
}
int main(int argc, char *argv[])
{

    // Arguments Parsing
    if (argc >= 2)
    {
        /**
         * Normally parsing the arguments here
         * (1) -h : help
         * (2) -t : test MD5 application
         * (3) -c : compute MD5 of the given file
         * (4) -v : validate the integrality of a given file by manual input MD5 value
         * (5) -f : validate the integrality of a given file by read MD5 value from .md5 file
         */
        std::string optionArg = argv[1];
        if (optionArg == "-h" || optionArg == "--help")
        {
            // If the option is help, print the help information
            help(argv[0]);
        }
        else if (optionArg == "-t" || optionArg == "--test")
        {
            // If the option is test, test the MD5 application
            test();
        }
        else if (optionArg == "-c" || optionArg == "--compute")
        {
            if (argc == 3)
            {
                // If the option is compute, compute the MD5 value of the given file
                computeMD5(argv[2]);
            }
            else
            {
                // If the option is compute,
                // But the number of arguments is not correct, print the help info to instruct the user
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == "-v" || optionArg == "--validate")
        {
            if (argc == 3)
            {
                validate(argv[2]);
            }
            else
            {
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == "-f" || optionArg == "--file")
        {
            if (argc == 4)
            {
                validate(argv[2], argv[3], AUTO);
            }
            else
            {
                help(argv[0]);
                return 1;
            }
        }
        else
        {
            // If the prasing option is not defined, print the help info to instruct the user
            help(argv[0]);
            return 1;
        }
    }
    else
    {
        // If the number of arguments is less than 2, print the help info to instruct the user
        help(argv[0]);
        return 1;
    }
    return 0;
}