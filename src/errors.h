/**
 * @file errors.h
 *
 * Contains commonly used error codes
 */

#ifndef ERRORS_H
#define ERRORS_H

/**
 * @defgroup ERROR_CODES Error codes used for indicating failures
 *
 * @{
 */
#define ERROR_SPI_INIT 1
#define ERROR_AD7792_CONNECTION 2
#define ERROR_AD7792_CONFIGURATION 3

#define ERROR_AD7792_SLOT1 10
#define ERROR_AD7792_SLOT2 20

#define ERROR_SCALE_USART_INIT 30
#define ERROR_SCALE_INIT 31
/** @} */


/**
 * @defgroup LOG Log Levels and default indent
 *
 * @{
 */
#define LOG_INDENT "           "

#define LOG_LEVEL_CSV 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL LOG_LEVEL_CSV
/** @} */


#endif /* ERRORS_H */
