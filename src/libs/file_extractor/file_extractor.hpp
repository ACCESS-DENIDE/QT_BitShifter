/**
 * @file file_extractor.hpp
 * @brief Defines functions to extract files list by provided params.
 */
#ifndef ACD_FILE_EXTRACTOR
#define ACD_FILE_EXTRACTOR

#include <QString>
#include <QStringList>

// Debug define used to halt thread for provided mseconds in each iterration of (GetAllFileByMask)
#define DEBUG_BIG_WORK_TIME_FILE_EXECUTER 10000

/**
 * @namespace ACD
 * @brief Main namespace for code created by me (ACCESS-DENIDE)
 */
namespace ACD
{
    /**
     * @brief Extracts all availible files using mask (* - can be used as single layer wildcard)
     * 
     * @param mask QString containing file search mask for file extraction
     * 
     * @return List of all filepathes accessable via provided mask
     * 
     */
    QStringList GetAllFileByMask(const QString& mask);

    /**
     * @brief Extracts all availible files using several masks separated by ';'. (* - can be used as single layer wildcard) See GetAllFileByMask.
     * 
     * @param config_string QString containing file search masks separated by ';'
     * 
     * @return List of all filepathes accessable via provided masks
     * 
     */
    QStringList GetAllFileByConfig(const QString& config_string);
    
} // namespace ACD


#endif