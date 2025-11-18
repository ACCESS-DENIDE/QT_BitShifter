
#include <QCoreApplication>
#include <QApplication>
#include <QMainWindow>


#include "ui_elements/task_creation_dialog/task_creation_dialog.hpp"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QVector>

/**
 * @brief Vector contains pointers to all running executors
 */
QVector<ACD::TaskExecutor*> executors;

/**
 * @brief Vector contains pointers to all displayed TaskCards
 */
QVector<ACD::TaskCard*> cards;

/**
 * @brief Calls for ACD::TaskCreationDialog to create and add new active task.
 * 
 * @param task_card_holder Pointer to TaskCard-s holder
 * 
 */
void AddNewTask(QVBoxLayout* task_card_holder);

/**
 * @brief Handles deletion of tasks from active list and display
 * 
 * @param task_card_holder Pointer to TaskCard-s holder
 * @param pos Position of task to delete
 *  
 */
void DeleteTaskOnPosition(QVBoxLayout* task_card_holder, int pos);

int main(int argc, char *argv[])
{   
    //Main application
    QApplication app(argc, argv);

    //Main window of application
    QMainWindow window;
    //Main window's central widget
    QWidget* central_widget=new QWidget();
    //Main window's layout
    QVBoxLayout* layout=new QVBoxLayout(central_widget);

    //TaskCard holder's scroll area
    QScrollArea* tasks_scroll=new QScrollArea();
    //TaskCard holder's scroll content viewer
    QWidget* scroll_content=new QWidget();
    //TaskCard holder
    QVBoxLayout* tasks_layout=new QVBoxLayout(scroll_content);

    tasks_layout->addStretch();

    tasks_scroll->setWidget(scroll_content);
    tasks_scroll->setWidgetResizable(true);

    //Add Task button
    QPushButton* add_task_btn=new QPushButton("Add Task");
    layout->addWidget(tasks_scroll);
    layout->addWidget(add_task_btn);
    QObject::connect(add_task_btn, &QPushButton::clicked, [&](){
        AddNewTask(tasks_layout);
    });

    window.setWindowTitle("Bit modifier");
    window.setCentralWidget(central_widget);
    window.resize(800, 400);
    window.show();

    return app.exec();
}


void AddNewTask(QVBoxLayout* task_card_holder){
    //Task creation dialog
    ACD::TaskCreationDialog dialog;

    //New task executor
    ACD::TaskExecutor* new_executor=nullptr;

    if(dialog.exec()==QDialog::Accepted){
        new_executor=new ACD::TaskExecutor(dialog.GetCreatedConfig());
        executors.append(new_executor);
        //New task executor's card (TaskCard)
        ACD::TaskCard* new_executor_card=new ACD::TaskCard(executors.size()-1, new_executor);
        task_card_holder->addWidget(new_executor_card);
        cards.append(new_executor_card);
        new_executor->StartProcessing();
        QObject::connect(new_executor_card, &ACD::TaskCard::abortClick, new_executor, &ACD::TaskExecutor::onAbortExecution);
        QObject::connect(new_executor_card, &ACD::TaskCard::stopClick, new_executor, &ACD::TaskExecutor::onStopTask);
        QObject::connect(new_executor_card, &ACD::TaskCard::deleteClick, [=](){
            QThread* deleter_thread=QThread::create(DeleteTaskOnPosition, task_card_holder, new_executor_card->GetTaskId());
            QObject::connect(deleter_thread, &QThread::finished, deleter_thread, &QThread::deleteLater);
            deleter_thread->start();
        });
        
    }
}


void DeleteTaskOnPosition(QVBoxLayout* task_card_holder, int pos){

    cards[pos]->MarkToDelete();
    delete executors[pos];
    task_card_holder->removeWidget(cards[pos]);
    delete cards[pos];

    cards.removeAt(pos);
    executors.removeAt(pos);

    for (int i=pos; i<cards.size(); i++){
        cards[i]->SetTaskId(cards[i]->GetTaskId()-1);
    }

}