#include "task_creation_dialog.hpp"

#include <QMessageBox>

void ACD::TaskCreationDialog::SetupUi()
{
    QVBoxLayout* window_layout=new QVBoxLayout(this);
    QFormLayout* form_layout=new QFormLayout();

    input_files_le=new QLineEdit(this);
    input_files_le->setPlaceholderText("Input files mask");
    form_layout->addRow("Input files: ", input_files_le);

    output_folder_le=new QLineEdit(this);
    output_folder_le->setPlaceholderText("Output folder");
    form_layout->addRow("Output folder: ", output_folder_le);

    file_search_timer_sb=new QSpinBox(this);
    file_search_timer_sb->setRange(0, 86400000);
    file_search_timer_sb->setValue(0);
    form_layout->addRow("File search delay (msec): ", file_search_timer_sb);

    restart_timer_sb=new QSpinBox(this);
    restart_timer_sb->setRange(-1, 86400000);
    restart_timer_sb->setValue(-1);
    form_layout->addRow("Restart delay (msec)\n[-1 - no restarts]: ", restart_timer_sb);

    mask_le=new QLineEdit(this);
    mask_le->setPlaceholderText("Mask");
    form_layout->addRow("Modification mask (hex): ", mask_le);

    is_keep_input_files_cb=new QCheckBox(this);
    is_keep_input_files_cb->setChecked(true);
    form_layout->addRow("Keep input files: ", is_keep_input_files_cb);

    is_override_existing_files_cb=new QCheckBox(this);
    is_override_existing_files_cb->setChecked(false);
    form_layout->addRow("Override existing files: ", is_override_existing_files_cb);

    dialog_btns=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    form_layout->addRow("", dialog_btns);

    window_layout->addLayout(form_layout);

    connect(dialog_btns, &QDialogButtonBox::accepted, this, &TaskCreationDialog::accept);
    connect(dialog_btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    
}

void ACD::TaskCreationDialog::accept()
{
    QMessageBox error_popup(this);
    error_popup.setWindowTitle("Task config error");
    error_popup.setIcon(QMessageBox::Warning);
    error_popup.setText("Wrong modification mask");
    error_popup.setStandardButtons(QMessageBox::Ok);
    error_popup.setDefaultButton(QMessageBox::Ok);

    QString mask_text=mask_le->text();

    if(mask_text.size()!=16){
        error_popup.exec();
        return;
    }

    mask_text=mask_text.toUpper();
    QString probe_string;
    bool ok;
    for(int i=0; i<MASK_SIZE_BYTES*2; i+=2){
        probe_string=mask_text[i];
        probe_string+=mask_text[i+1];
        generated_config.task_mask[i/2]=probe_string.toInt(&ok, 16);
        if(!ok){
            error_popup.exec();
            return;
        }
    }


    generated_config.input_file_config=input_files_le->text();
    generated_config.output_directory=output_folder_le->text();
    generated_config.files_wait_interval_msec=file_search_timer_sb->value();
    generated_config.round_wait_interval_msec=restart_timer_sb->value();
    generated_config.is_keep_input_files=is_keep_input_files_cb->isChecked();
    generated_config.is_override=is_override_existing_files_cb->isChecked();

    QDialog::accept();

}

ACD::TaskCreationDialog::TaskCreationDialog(QWidget *parent)
:QDialog(parent)
{
    SetupUi();
    setWindowTitle("Create new task");
    setMinimumWidth(300);
}

ACD::TaskExecutor::TaskConfig ACD::TaskCreationDialog::GetCreatedConfig()
{
    return generated_config;
}

