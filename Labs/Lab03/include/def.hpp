// Defines the constants and types used in the MD5 validation algorithm.

#ifndef MD5_DEFS_H
#define MD5_DEFS_H

#define DEFAULT_TMP_DIR "tmp"      // Default temporary directory for saving files for correctness application
#define DEFAULT_FILE_NAME "test"   // Default file name to be validated
#define DEFAULT_MD5_FILE "MD5.md5" // Default file name for MD5 hash
#define SUCCESS 0                  // Success return code
#define FAILURE 1                  // Failure return code
#define MANUAL 0                   // Manual validation approach
#define AUTO 1                     // Auto validation approach

/* Options for CommandLine Interaction */
#define pHelp "-h"     // Help option
#define pTest "-t"     // Test option
#define pCompute "-c"  // Compute option
#define pValidate "-v" // Manual validate option
#define pFile "-f"     // Automatic file validate option
#define pFileMD5 "-fm" // Automatic file validate option with default MD5 file

typedef int INT; // Define INT as an integer type

#endif // MD5_DEFS_H