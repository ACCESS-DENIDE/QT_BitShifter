/**
 * @file task_creation_dialog.hpp
 * @brief Defines the ACD::TaskCreationDialog class. Dialog box for creating new tasks.
 */
#ifndef ACD_TASK_CREATION
#define ACD_TASK_CREATION

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

#include "../task_card/task_card.hpp"

/**
 * @namespace ACD
 * @brief Main namespace for code created by me (ACCESS-DENIDE)
 */
namespace ACD
{
    /**
     * @class ACD::TaskCreationDialog
     * @brief A dialog window to setup new bit shif task.
     * 
     * This widget collects info from user, creating new TaskExecutor config, to be sent to freshly created executor.
     */
    class TaskCreationDialog : public QDialog{
        Q_OBJECT

    private:
        /**
         * @brief Line edit to set input files mask
         */
        QLineEdit* input_files_le;

        /**
         * @brief Line edit to set output folder
         */
        QLineEdit* output_folder_le;

        /**
         * @brief Spinbox to set delay between file search attempts
         */
        QSpinBox* file_search_timer_sb;

        /**
         * @brief Spinbox to set delay between task execution rounds
         */
        QSpinBox* restart_timer_sb;

        /**
         * @brief Line edit to applied bit mask (as hex)
         */
        QLineEdit* mask_le;

        /**
         * @brief Checkbox to keep input files (or delete them)
         */
        QCheckBox* is_keep_input_files_cb;


        /**
         * @brief Checkbox to overwrite existing files (or modify output file name)
         */
        QCheckBox* is_override_existing_files_cb;

        /**
         * @brief Dialog window's buttons (Ok - accept settings, Cancel - cancel)
         */
        QDialogButtonBox* dialog_btns;
        

        /**
         * @brief Variable to keep user generated config
         */
        ACD::TaskExecutor::TaskConfig generated_config;

        /**
         * @brief Sets up the layout and initialize UI components.
         * 
         * This function typycaly called within the constructor
         */
        void SetupUi();

    protected:
        /**
         * @brief Overwritten accept slot of QDialog. (So the data can be verifyed before accepting.)
         */
        void accept() override;
        

    public:
        
        /**
         * @brief Constructs a TaskCreationDialog.
         * 
         * @param parent The parent widget of this card (default is nullptr)
         * 
         */
        explicit TaskCreationDialog(QWidget* parent=nullptr);

        /**
         * @brief Returns user generated config
         * 
         * @return New task's config
         */
        ACD::TaskExecutor::TaskConfig GetCreatedConfig();

    };
}

#endif