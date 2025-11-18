/**
 * @file task_card.hpp
 * @brief Defines the ACD::TaskCard class. UI component for diaplsying bit shif executors.
 */
#ifndef ACD_UI_TASKCARD
#define ACD_UI_TASKCARD

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>

#include "../../classes/task_executor/task_executor.hpp"

/**
 * @namespace ACD
 * @brief Main namespace for code created by me (ACCESS-DENIDE)
 */
namespace ACD
{

    /**
     * @class ACD::TaskCard
     * @brief A UI widget representing single bit shift executor(TaskExecutor).
     * 
     * This widget displays task's ID, status, progress of current file, total file amount.
     * Additionaly it provides control buttons:
     * Stop - Wait for processing of ALL files in current cycle and stops executor.
     * Abort - Wait for processing CURRENT file and stops executor.
     * Delete - ABORTs executer and deletes it from heap.
     * Must be connected to TaskExecutor for proper work.
     */
    class TaskCard : public QWidget{
        Q_OBJECT
        
        private:

            /**
             * @brief Pointer to the bounded TaskExecutor.
             * 
             * Executor handles actual bit shif operations.
             */
            TaskExecutor* my_executor;

            /**
             * @brief Label to display task's id
             */
            QLabel* id_label;

            /**
             * @brief Label to display executor's status
             */
            QLabel* status_label;

            /**
             * @brief Label to display processed files amount
             */
            QLabel* file_progress_label;

            /**
             * @brief Circle to display visual indicator of status
             */
            QFrame* status_indicator;

            /**
             * @brief Progress bar to display current file's progress
             */
            QProgressBar* cur_file_progress;

            /**
             * @brief Button to delete executor
             */
            QPushButton* delete_btn;

            /**
             * @brief Button to stop executor
             */
            QPushButton* stop_btn;

            /**
             * @brief Button to abort executor
             */
            QPushButton* abort_btn;
            
            
            /**
             * @brief The unique ID of the task
             */
            int id;

            /**
             * @brief Marks if TaskCrad and executor marked to deallocation
             */
            bool is_marked_to_delete;

            /**
             * @brief Sets up the layout and initialize UI components.
             * 
             * This function typycaly called within the constructor
             */
            void SetupUI();
            
            /**
             *@brief Updates color indicator of the executor
             *
             * @param new_color A QString containing the verbal color value (e.g. "red", "blue", e.t.c.) 
             */
            void UpdateStatusColor(const QString& new_color);

        private slots:

            /**
             * @brief Slot called when delete button clicked
             * 
             */
            void onDeleteClick();

            /**
             * @brief Slot called when stop button clicked
             * 
             */
            void onStopClick();

            /**
             * @brief Slot called when abort button clicked
             * 
             */
            void onAbortClick();


            /**
             * @brief Slot to update current percentage of processing file
             * 
             * @param cur_percent Current processing value (0.0, 100.0)
             * 
             */
            void onProcessingPercentChange(float cur_percent);

            /**
             * @brief Slot to update current amount of processed files
             * 
             * @param total_files Toatal amount of files to process
             * @param processed_files Current amount of processed files
             * 
             */
            void onProcessingFilesChange(int total_files, int processed_files);
            
            /**
             * @brief Slot to update current executor's status
             * 
             * @param cur_stage New executor's stage value
             * 
             */
            void onExecutorStageChange(ACD::TaskExecutor::STAGE cur_stage);

            /**
             * @brief Slot to update current current fail information
             * 
             * @param fail_info String representation of fail
             * 
             */
            void onExecutorFail(QString fail_info);
        
        
        public:
            /**
             * @brief Constructs a TaskWidget.
             * 
             * @param task_id The unique ID of constructed executor
             * @param bounded_executor Pointer to bounded bit shift executor
             * @param parent The parent widget of this card (default is nullptr)
             * 
             */
            explicit TaskCard(int task_id,TaskExecutor* bounded_executor=nullptr, QWidget* parent=nullptr);

            /**
             * @brief Updates unique executor's ID
             * 
             * @param new_id New executor's id
             * 
             */
            void SetTaskId(int new_id);

            /**
             * @brief Returns unique ID of the executor
             * @return Current executor's ID
             * 
             */
            int GetTaskId();

            /**
             * @brief Marks card and it's executor to delete
             */
            void MarkToDelete();
        signals:
            /**
             * @brief Signal emmited when the user clicks the Abort button
             * 
             * This instructs TaskExecutor to abort the task
             * 
             */
            void abortClick();

            /**
             * @brief Signal emmited when the user clicks the Stop button
             * 
             * This instructs TaskExecutor to stop the task
             * 
             */
            void stopClick();

            /**
             * @brief Signal emmited when the user clicks the Delete button
             * 
             * This instructs TaskExecutor to delete the task
             * 
             */
            void deleteClick();
    };
    
} // namespace ACD


#endif