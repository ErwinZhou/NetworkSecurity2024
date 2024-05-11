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

    std::cerr << "Usage: ./" << programName << " [-Option] [file path of the file validated] [file path of the .md5 file] [times]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  [-h] --help" << std::endl;
    std::cerr << "  [-t] --test MD5 application" << std::endl;
    std::cerr << "  [-c] [file path of the file computed] [times]\n"
              << "       --compute MD5 of the given file for [times] times\n"
              << "       --If [times] equals -1, it stands for divide and merge method" << std::endl;
    std::cerr << "  [-v] [file path of the file validated]\n"
              << "       --validate the integrality of a given file by manual input MD5 value" << std::endl;
    std::cerr << "  [-f] [file path of the file validated] [file path of the .md5 file]\n"
              << "       --validate the integrality of a given file by read MD5 value from .md5 file\n"
              << "       --[file path of the .md5 file] or both the file path can be empty" << std::endl;
    std::cerr << "  [-fm] [file path of the file validated] [file path of the .md5 file]\n"
              << "       --validate the integrality of a given file by read MD5 value from .md5 file\n"
              << "       --[file path of the file validated] or both the file path can be empty" << std::endl;

    std::cerr
        << "---------------------------------------------------------------------------" << std::endl;
    return;
}

inline void test()
{
    /**
     * Test the MD5 application using offical test vectors to verify the correctness of the MD5 algorithm
     * @return void
     */

    std::cout << "----------------------------- Testing MD5 Correctness ------------------------------" << std::endl;
    std::cout << "[INFO] The following examples can be found in MD5 Offical Doc:RFC1321." << std::endl;
    std::vector<std::string> testStrings = {"", "a", "abc", "message digest", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};
    MD5 md5;
    for (int i = 0; i < testStrings.size(); i++)
    {
        std::string testString = testStrings[i];
        md5.compute(testString);
        std::string md5Value = md5.toString();
        std::cout << "MD5(\"" << testString << "\") = " << md5Value << std::endl;
        std::string md5FileName = "../tmp/RFC-" + std::to_string(i + 1) + ".md5";
        std::ofstream md5File(md5FileName);
        md5File << md5Value;
        md5File.close();
    }
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    return;
}

inline INT computeMD5(const char *filePath, INT times = 1)
{
    /**
     * Compute the MD5 value of the given file
     * @param filePath the file path of the file to be computed
     * @param times In order to defend against the dictionary attack
     *              The times for the MD5 algorithm to be computed
     *              If times = -1, it stands for divide and merge method
     * @return INT: 0 for SUCCESS, 1 for FAILURE
     */
    std::cout << "----------------------------- Compute MD5 for File------------------------------" << std::endl;
    MD5 md5;
    std::ifstream file(filePath, std::ios::binary);
    std::string leftMD5, rightMD5, encryptedLeftMD5, encryptedRightMD5, finalMD5;
    // To make sure if the file is opened successfully
    if (!file)
    {
        std::cerr << "[ERROR] Cannot open the file: \"" << filePath << "\"" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return FAILURE;
    }
    if (times == -1)
    {
        // Divide and Merge Method
        std::cout << "[INFO] Using Divide and Merge Method to compute the MD5 value of file(\"" << filePath << "\")" << std::endl;
        md5.compute(file);
        // Divide the md5 value into two separate parts
        leftMD5 = md5.toString().substr(0, 16);
        rightMD5 = md5.toString().substr(16, 16);
        // Compute the MD5 value of the two 16 bytes separately
        md5.compute(leftMD5);
        encryptedLeftMD5 = md5.toString();
        md5.compute(rightMD5);
        encryptedRightMD5 = md5.toString();
        // Merge the two MD5 values
        md5.compute(encryptedLeftMD5 + encryptedRightMD5);
        finalMD5 = md5.toString();
        std::cout << "The MD5 value of file(\"" << filePath << "\") is:" << std::endl;
        std::cout << finalMD5 << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
    else
    {
        // Compute Multiple Times Method
        std::cout << "[INFO] Computing the MD5 value of file(\"" << filePath << "\") for " << times << " time(times)." << std::endl;
        md5.compute(file);
        times--;
        while (times > 0)
        {
            // If it only computes once, directly output the MD5 value, it won't enter the loop
            md5.compute(md5.toString());
            times--;
        }

        std::cout << "The MD5 value of file(\"" << filePath << "\") is:" << std::endl;
        std::cout << md5.toString() << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
}

inline INT validate(const char *validatedFilePath = DEFAULT_FILE_NAME, const char *md5FilePath = DEFAULT_MD5_FILE, INT Approach = MANUAL)
{
    /**
     * Validate the integrality of the given file by manual or automatically input MD5 value
     * @param validatedFilePath the file path of the file to be validated
     * @param md5FilePath the file path of the .md5 file
     * @return INT: 0 for SUCCESS, 1 for FAILURE
     */
    MD5 md5;
    std::string computedMD5Value;
    std::string inputMD5Value;
    std::string readMD5Value;
    // Open the tested file
    std::ifstream validatedFile(validatedFilePath, std::ios::binary);
    // Open the .md5 file
    std::ifstream md5File(md5FilePath, std::ios::binary);
    switch (Approach)
    {
    case MANUAL:
        // Manual validation approach
        // Input the MD5 value of the file manually
        std::cout << "----------------------------- Validate File Integrality Manually------------------------------" << std::endl;
        std::cout << "Please input the MD5 value of the file(\"" << validatedFilePath << "\"):" << std::endl;
        std::cin >> inputMD5Value;

        // To make sure if the file is opened successfully
        if (!validatedFile)
        {
            std::cerr << "[ERROR] Cannot open the validated file: \"" << validatedFilePath << "\"" << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return FAILURE;
        }

        // Compute the MD5 value of the file
        md5.reset();
        md5.compute(validatedFile);
        computedMD5Value = md5.toString();

        // Compare the input MD5 value with the computed MD5 value
        if (inputMD5Value == computedMD5Value)
        {
            std::cout << "[INFO] OK!The file(\"" << validatedFilePath << "\") is integrality." << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return SUCCESS;
        }
        else
        {
            std::cout << "[INFO] Match Error! The file (\"" << validatedFilePath << "\") has been modified!" << std::endl;
            std::cout << "[INFO] It is not integrality." << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return FAILURE;
        }
        break;
    case AUTO:
        // Automatic validation approach
        std::cout << "----------------------------- Validate File Integrality Automatically------------------------------" << std::endl;

        // To make sure if the validated file is opened successfully
        if (!validatedFile)
        {
            std::cerr << "[ERROR] Cannot open the validated file: \"" << validatedFilePath << "\"" << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return FAILURE;
        }
        // To make sure if the .md5 file is opened successfully
        if (!md5File)
        {
            std::cerr << "[ERROR] Cannot open the MD5 file: \"" << md5FilePath << "\"" << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return FAILURE;
        }

        // Compute the MD5 value of the file

        md5.reset();
        md5.compute(validatedFile);
        computedMD5Value = md5.toString();

        // Read the MD5 value from the .md5 file
        getline(md5File, readMD5Value);

        // Compare the read MD5 value with the computed MD5 value
        if (readMD5Value == computedMD5Value)
        {
            std::cout << "[INFO] OK!The file(\"" << validatedFilePath << "\") is integrality." << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return SUCCESS;
        }
        else
        {
            std::cout << "[INFO] Match Error! The file (\"" << validatedFilePath << "\") has been modified!" << std::endl;
            std::cout << "[INFO] It is not integrality." << std::endl;
            std::cout << "-------------------------------------------------------------------------------------" << std::endl;
            return FAILURE;
        }
        break;
    default:
        // If the approach is not defined, print the help info to instruct the user
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        return FAILURE;
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
         * (3) -c : compute MD5 of the given file for [times] times
         * (4) -v : validate the integrality of a given file by manual input MD5 value
         * (5) -f : validate the integrality of a given file by read MD5 value from .md5 file, can leave the .md5 file path or both file path empty
         * (6) -fm : validate the integrality of a given file by read MD5 value from .md5 file, can leave the tested file path or both file path empty
         */
        std::string optionArg = argv[1];
        if (optionArg == pHelp)
        {
            // If the option is help, print the help information
            help(argv[0]);
        }
        else if (optionArg == pTest)
        {
            // If the option is test, test the MD5 application
            test();
        }
        else if (optionArg == pCompute)
        {
            if (argc == 4)
            {
                // If the option is compute, compute the MD5 value of the given file
                if (computeMD5(argv[2], std::stoi(argv[3])) == FAILURE)
                    help(argv[0]);
            }
            else
            {
                // If the option is compute,
                // But the number of arguments is not correct, print the help info to instruct the user
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == pValidate)
        {
            if (argc == 3)
            {
                if (validate(argv[2]) == FAILURE)
                    help(argv[0]);
            }
            else
            {
                std::cout << "[INFO] Using the default file to be validated:\"" << DEFAULT_FILE_NAME << "\"" << std::endl;
                if (validate(DEFAULT_FILE_NAME) == FAILURE)
                    help(argv[0]);
            }
        }
        else if (optionArg == pFile)
        {
            /**
             * If the option is file, validate the integrality of the given file by read MD5 value from .md5 file
             * (1) -f [file path of the file validated] [file path of the .md5 file]
             * (2) -f [file path of the file validated]
             * (3) -f
             */
            if (argc == 4)
            {
                if (validate(argv[2], argv[3], AUTO) == FAILURE)
                    help(argv[0]);
            }
            else if (argc == 3)
            {
                std::cout << "[INFO] Using the default .md5 file to read value:\"" << DEFAULT_MD5_FILE << std::endl;
                if (validate(argv[2], DEFAULT_MD5_FILE, AUTO) == FAILURE)
                    help(argv[0]);
            }
            else if (argc == 2)
            {
                std::cout << "[INFO] Using the default file to be validated:\"" << DEFAULT_FILE_NAME << "\" and the default MD5 file:\"" << DEFAULT_MD5_FILE << "\"" << std::endl;
                if (validate(DEFAULT_FILE_NAME, DEFAULT_MD5_FILE, AUTO) == FAILURE)
                    help(argv[0]);
            }
            else
            {
                // If the option is file,
                // But the number of arguments is > 4, print the help info to instruct the user
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == pFileMD5)
        {
            /**
             * If the option is fileMD5, validate the integrality of the given file by read MD5 value from .md5 file
             * (1) -fm [file path of the file validated] [file path of the .md5 file]
             * (2) -fm [file path of the .md5 file]
             * (3) -fm
             */
            if (argc == 4)
            {
                if (validate(argv[2], argv[3], AUTO) == FAILURE)
                    help(argv[0]);
            }
            else if (argc == 3)
            {
                std::cout << "[INFO] Using the default file to be validated:\"" << DEFAULT_FILE_NAME << "\"" << std::endl;
                if (validate(DEFAULT_FILE_NAME, argv[2], AUTO) == FAILURE)
                    help(argv[0]);
            }
            else if (argc == 2)
            {
                std::cout << "[INFO] Using the default file to be validated:\"" << DEFAULT_FILE_NAME << "\" and the default MD5 file:\"" << DEFAULT_MD5_FILE << "\"" << std::endl;
                if (validate(DEFAULT_FILE_NAME, DEFAULT_MD5_FILE, AUTO) == FAILURE)
                    help(argv[0]);
            }
            else
            {
                // If the option is file,
                // But the number of arguments is > 4, print the help info to instruct the user
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