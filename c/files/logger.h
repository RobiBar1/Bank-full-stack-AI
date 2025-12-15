#ifndef __LOGGER_H__
#define __LOGGER_H__

typedef enum {SUCCESS, FILE_OPEN_ERROR, FILE_CLOSE_ERROR, FILE_REMOVE_ERROR, FILE_WRITE_ERROR, FILE_READ_ERROR} logger_status_t;

/** @desc Simple interactive logger which writes user input into text file.
 *  
 *  The function opens or creates a text file specified by @p file_name and continuously
 *  reads input from the user via standard input. Regular text is appended to the end of the file
 *  by default, while special command trigger specific actions.
 * 
 *  Supported commands:
 *  - "-count" - prints number of lines in text file 
 *  - "-exit" - close the program
 *  - "-remove" - delete the created file_name.txt
 *  - "<" - write to the start of the text file
 *  - By default appends the text to the end of file_name.txt
 * 
 * @param[in] file_name - file name of the desired text file
 * @return logger_status_t - returns logger_status_t enum that indicates exit status
 * @errors: FILE_OPEN_ERROR - Indicates file opening was unsuccessful.
 *          FILE_CLOSE_ERROR - Indicates file closing was unsuccessful.
 *          FILE_REMOVE_ERROR - Indicates file deleting was unsuccessful.
 *          FILE_WRITE_ERROR - Indicates file writing was unsuccessful.
 *          FILE_READ_ERROR - Indicates file reading was unsuccessful.
 * 
 * **/
logger_status_t Logger(const char* file_name);

#endif /*__LOGGER_H__*/ 

