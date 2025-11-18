/**
 * @file bit_modifier.hpp
 * @brief Defines the ACD::BitModifier class. Class thats executes bit shif operations over files.
 */
#ifndef ACD_BIT_MODIFIER
#define ACD_BIT_MODIFIER

#include <QObject>
#include <QString>

// Bit mask size in bytes
#define MASK_SIZE_BYTES 8
//Processing chunk size in masks
#define MAX_CHUNK_SIZE 1


// Debug define used to halt thread for provided mseconds in each chunk
#define DEBUG_BIG_WORK_TIME_BIT_MOD 500

/**
 * @namespace ACD
 * @brief Main namespace for code created by me (ACCESS-DENIDE)
 */
namespace ACD
{
    typedef char BitMask[MASK_SIZE_BYTES];
    /**
     * @class ACD::BitModifier
     * 
     * @brief Handles all bit shift operations over provided file
     */
    class BitModifier : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @enum ACD::BitModifier::ERROR_CODES
         * 
         * @brief Contains error codes for bit shifter
         * 
         */
        enum ERROR_CODES{
            /**
             * @brief Finished without errors
             */
            Ok=0,
            /**
             * @brief Can't access input file
             */
            FileInaccessable=-1,
            /**
             * @brief Can't access output directory
             */
            OutputDirectoryInaccessable=-2
        };
    private:
        /**
         * @brief Output directory for processed files
         */
        QString output_dir;
        /**
         * @brief bit mask, applied to input files
         */
        BitMask applicable_mask;
        /**
         * @brief Setting to override existing files (if false, files will be saved with suffix)
         */
        bool is_override_existing;
        /**
         * @brief Setting to keep input files (if false, files will be deleted)
         */
        bool is_keep_input;
        
        /**
         * @brief XOR operation for chars
         * 
         * @param a Input char a
         * @param b Input char b
         * 
         * @return Xored a^b
         * 
         */
        char XOR(const char a, const char b);
    public:
        
        /**
         * @brief Constructs a BitModifier.
         * 
         */
        BitModifier();
        /**
         * @brief Applies mask through XOR to input file
         * 
         * @param input_file_path Input file path
         * 
         * @return Error code of operation
         */
        ERROR_CODES ApplyToFile(const QString& input_file_path);
        /**
         * @brief Sets override parametr
         * (if true Overrides existing output files, if false files will be saved with suffix)
         * 
         * @param is_override New value of override parametr
         */
        void IsOverrideExisting(bool is_override);
        /**
         * @brief Sets keep parametr
         * (if true input files will be keeped, if false input files will be deleted after processing)
         * 
         * @param is_keep New value of keep parametr
         */
        void IsKeepInputFiles(bool is_keep);
        /**
         * @brief Sets new bit mask for XOR function
         * 
         * @param new_mask New bit mask
         */
        void SetBitMask(const BitMask& new_mask);

        /**
         * @brief Sets output directory for processed files
         * 
         * @param new_output_dir New output directory
         */
        void SetOutputDirectory(const QString& new_output_dir);
        /**
         * @brief Deconstructs a BitModifier.
         */
        ~BitModifier();

    signals:
        /**
         * @brief Signal called when current file progress changes
         * 
         * @param cur_progress New processing progress
         */
        void processingPercentChange(float new_percent);
    
    };
    
    
} // namespace ACD


#endif