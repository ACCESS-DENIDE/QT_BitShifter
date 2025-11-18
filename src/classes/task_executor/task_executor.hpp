/**
 * @file task_executor.hpp
 * @brief Defines the ACD::TaskExecutor class. Used for executing bit shift operations on the background.
 */
#ifndef ACD_TASK_EXECUTOR
#define ACD_TASK_EXECUTOR

#include <QObject>
#include <QThread>

#include "../bit_modifier/bit_modifier.hpp"
#include "../../libs/file_extractor/file_extractor.hpp"

//Symbol used to separate different config fields in string representation for saving in file (deprecated)
#define CONFIG_SPLITTER char(7)

/**
 * @namespace ACD
 * @brief Main namespace for code created by me (ACCESS-DENIDE)
 */
namespace ACD
{
    /**
     * @class ACD::TaskExecutor
     * @brief An executor, thats handles new thread creation and flow control.
     * 
     */
    class TaskExecutor : public QObject
    {
        Q_OBJECT
    public:

        /**
         * @brief Enumeration describing current stage of the executor.
         * @enum ACD::TaskExecutor::STAGE
         */
        enum STAGE{
            /**
             * @brief Executor's files extractor searching for valid files
             */
            SearchingFiles=0,
            
            /**
             * @brief Executor's files extractor waiting to start file search (halted by timer)
             */
            AwaitingFiles=1,
            
            /**
             * @brief Executor process files with bit shift operation
             */
            Processing=2,
            
            /**
             * @brief Executor waiting to start next execution round
             */
            AwaitingNextRound=3,
            
            /**
             * @brief Executor done all operations and exited working thread
             */
            Done=4,
            
            /**
             * @brief Executor failed
             */
            Fail=5
        };

        /**
         * @brief Structure representing TaskExecutor's configuration
         * 
         * @struct ACD::::TaskExecutor::TaskConfig
         */
        struct TaskConfig{
            /**
             * @brief QString representing file search mask
             */
            QString input_file_config;

            /**
             * @brief QString representing output directory
             */
            QString output_directory;

            /**
             * @brief Interval in mseconds between file search attempts
             */
            int files_wait_interval_msec;

            /**
             * @brief Interval in mseconds between bit shifting rounds
             */
            int round_wait_interval_msec;

            /**
             * @brief Executor's bit mask for XOR operation
             */
            BitMask task_mask;

            /**
             * @brief If true, files with same name will be overwritten, otherwise files will be created with suffixes
             */
            bool is_override;

            /**
             * @brief If true, input files will be deleted, keeped otherwise
             */
            bool is_keep_input_files;
        };

    private:
        /**
         * @brief Pointer to a task executing thread
         */
        QThread* working_thread;

        /**
         * @brief Bit shifter bounded to this executor
         */
        BitModifier my_executor;

        /**
         * @brief Input file search mask
         */
        QString input_files_config;

        /**
         * @brief Directory to store processed files
         */
        QString fail_info;

        int files_wait_interval_msec;
        int round_wait_interval_msec;

        int processed_files;
        
        /**
         * @brief Does executor marked to stop after this round
         */
        bool is_marked_to_stop;

        /**
         * @brief Does executor marked to abort after this file
         */
        bool is_marked_to_abort;
    
        /**
         * @brief Main executor's loop
         * 
         * Launched in different thread
         */
        void Process();

    private slots:
        /**
         * @brief Slot, called when current processing file percent changed
         */
        void onProcessingPercentChange(float cur_percent);

    public:
        /**
         * @brief Constructs a TaskExecutor.
         * 
         * @param task_conf Configs for executor's BitShifter
         * 
         */
        TaskExecutor(const TaskConfig& task_conf);

        /**
         * @brief Starts executor's working thread
         */
        void StartProcessing();

        /**
         * @brief Returns verbal information about executor's error
         * 
         * @return Verbal error info
         */
        QString GetFailInfo();

        /**
         * @brief Deconstructs a TaskExecutor.
         */
        ~TaskExecutor();


    signals:
        /**
         * @brief Signal called when executor's stage changes
         * 
         * @param cur_stage New stage of the executor
         */
        void executorStageChange(STAGE cur_stage);

        /**
         * @brief Signal called when total amount or processed amount of files changes
         * 
         * @param total_files New total omount of processing files
         * 
         * @param processed_files New amount of already processed files
         */
        void processedFilesChanged(int total_files, int processed_files);

        /**
         * @brief Signal called when executor accurs an error
         * 
         * @param fail_info Verbal error info
         */
        void executorFailed(QString fail_info);

        /**
         * @brief Signal called when current file progress changes
         * 
         * @param cur_progress New processing progress
         */
        void curentFileProcessChanged(float cur_progress);
    
    public slots:
        /**
         * @brief Slot called when requests executor to stop operation
         */
        void onStopTask();

        /**
         * @brief Slot called when requests executor to abort operation
         */
        void onAbortExecution();
    };
    
} // namespace ACD


#endif