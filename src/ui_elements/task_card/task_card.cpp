#include "task_card.hpp"

void ACD::TaskCard::SetupUI()
{
    QHBoxLayout* layout_box=new QHBoxLayout(this);

    id_label=new QLabel(this);
    id_label->setAlignment(Qt::AlignCenter);
    id_label->setMinimumWidth(40);
    layout_box->addWidget(id_label);

    status_label=new QLabel("Connecting to executor...", this);
    status_label->setAlignment(Qt::AlignCenter);
    status_label->setMinimumWidth(80);
    layout_box->addWidget(status_label);

    status_indicator=new QFrame(this);
    status_indicator->setFixedSize(16, 16);
    status_indicator->setStyleSheet("QFrame { background-color: black; border-radius: 8px; }");
    layout_box->addWidget(status_indicator);

    file_progress_label=new QLabel("0/0", this);
    file_progress_label->setMinimumWidth(40);
    file_progress_label->setAlignment(Qt::AlignCenter);
    layout_box->addWidget(file_progress_label);

    cur_file_progress=new QProgressBar(this);
    cur_file_progress->setMinimumWidth(100);
    cur_file_progress->setRange(0, 100);
    cur_file_progress->setValue(0);
    layout_box->addWidget(cur_file_progress);

    delete_btn=new QPushButton("Delete", this);
    layout_box->addWidget(delete_btn);

    stop_btn=new QPushButton("Stop", this);
    layout_box->addWidget(stop_btn);

    abort_btn=new QPushButton("Abort", this);
    layout_box->addWidget(abort_btn);

    connect(delete_btn, &QPushButton::clicked, this, &TaskCard::onDeleteClick);
    connect(stop_btn, &QPushButton::clicked, this, &TaskCard::onStopClick);
    connect(abort_btn, &QPushButton::clicked, this, &TaskCard::onAbortClick);

    setMinimumHeight(40);
}

void ACD::TaskCard::UpdateStatusColor(const QString &new_color)
{
    if(is_marked_to_delete){
        return;
    }
    status_indicator->setStyleSheet(QString("QFrame {background-color: %1; border-radius: 8px;}").arg(new_color));
}

void ACD::TaskCard::onDeleteClick()
{
    if(is_marked_to_delete){
        return;
    }
    emit abortClick();
    emit deleteClick();
}

void ACD::TaskCard::onStopClick()
{
    if(is_marked_to_delete){
        return;
    }
    emit stopClick();
}

void ACD::TaskCard::onAbortClick()
{
    if(is_marked_to_delete){
        return;
    }
    emit abortClick();
}

void ACD::TaskCard::onProcessingPercentChange(float cur_percent)
{
    if(is_marked_to_delete){
        return;
    }
    cur_file_progress->setValue(cur_percent);
}

void ACD::TaskCard::onProcessingFilesChange(int total_files, int processed_files)
{
    if(is_marked_to_delete){
        return;
    }
    file_progress_label->setText(QString::number(processed_files)+"/"+QString::number(total_files));
}

void ACD::TaskCard::onExecutorStageChange(ACD::TaskExecutor::STAGE cur_stage)
{
    if(is_marked_to_delete){
        return;
    }
    QString new_indicator_color="black";
    switch (cur_stage)
    {
    case ACD::TaskExecutor::STAGE::SearchingFiles:
        new_indicator_color="blue";
        status_label->setText("Searching for input files");
        break;
    case ACD::TaskExecutor::STAGE::AwaitingFiles:
        new_indicator_color="cyan";
        status_label->setText("Awaiting search files delay");
        break;
    case ACD::TaskExecutor::STAGE::Processing:
        new_indicator_color="darkGreen";
        status_label->setText("Processing");
        break;
    case ACD::TaskExecutor::STAGE::Done:
        new_indicator_color="green";
        status_label->setText("Done");
        break;
    case ACD::TaskExecutor::STAGE::AwaitingNextRound:
        new_indicator_color="yellow";
        status_label->setText("Awaiting next round");
        break;
    case ACD::TaskExecutor::STAGE::Fail:
        new_indicator_color="darkRed";
        status_label->setText("Task failed");
        break;
    
    default:
        break;
    }
    UpdateStatusColor(new_indicator_color);
}

void ACD::TaskCard::onExecutorFail(QString fail_info)
{
    file_progress_label->setText(fail_info);
}

ACD::TaskCard::TaskCard(int task_id, TaskExecutor *bounded_executor, QWidget *parent) : QWidget(parent), id(task_id), my_executor(bounded_executor)
{
    is_marked_to_delete=false;
    SetupUI();
    SetTaskId(task_id);

    connect(my_executor, &TaskExecutor::executorStageChange, this, &TaskCard::onExecutorStageChange);
    connect(my_executor, &TaskExecutor::processedFilesChanged, this, &TaskCard::onProcessingFilesChange);
    connect(my_executor, &TaskExecutor::executorFailed, this, &TaskCard::onExecutorFail);
    connect(my_executor, &TaskExecutor::curentFileProcessChanged, this, &TaskCard::onProcessingPercentChange);
}

void ACD::TaskCard::SetTaskId(int new_id)
{
    id=new_id;
    id_label->setText(QString::number(new_id+1));
}

int ACD::TaskCard::GetTaskId()
{
    return id;
}

void ACD::TaskCard::MarkToDelete()
{
    is_marked_to_delete=true;
}
