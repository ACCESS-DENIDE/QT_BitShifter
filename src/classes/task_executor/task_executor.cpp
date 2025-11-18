#include "task_executor.hpp"



void ACD::TaskExecutor::Process()
{
    QStringList processing_files;
    do{
        emit processedFilesChanged(0, 0);
        do{

            emit executorStageChange(STAGE::SearchingFiles);
            processing_files=ACD::GetAllFileByConfig(input_files_config);
            
            emit executorStageChange(STAGE::AwaitingFiles);
            if(processing_files.size()==0 && files_wait_interval_msec>0){
                QThread::msleep(files_wait_interval_msec);
            }
        }while(processing_files.size()==0);


        emit executorStageChange(STAGE::Processing);
        emit processedFilesChanged(processing_files.size(), 0);
        
        processed_files=0;

        for (QStringList::iterator it =processing_files.begin(); it!=processing_files.end(); it++){

            if(is_marked_to_abort){
                break;
            }

            ACD::BitModifier::ERROR_CODES err=my_executor.ApplyToFile((*it));


            if(err!=ACD::BitModifier::ERROR_CODES::Ok){
                emit executorStageChange(STAGE::Fail);
                switch (err)
                {
                    case ACD::BitModifier::ERROR_CODES::FileInaccessable:
                        fail_info="File: ";
                        fail_info.append((*it));
                        fail_info.append("\nFail type: Can't access input file.");
                        emit executorFailed(fail_info);
                        break;
                    case ACD::BitModifier::ERROR_CODES::OutputDirectoryInaccessable:
                        fail_info="File: ";
                        fail_info.append((*it));
                        fail_info.append("\nFail type: Can't access output file.");
                        emit executorFailed(fail_info);
                        break;
                    
                    default:
                        fail_info="File: ";
                        fail_info.append((*it));
                        fail_info.append("\nFail type: Unknown error.");
                        emit executorFailed(fail_info);
                        break;
                }
                return;
            }
            processed_files++;

            emit processedFilesChanged(processing_files.size(), processed_files);

        }

        emit executorStageChange(STAGE::AwaitingNextRound);

        if(!is_marked_to_stop && round_wait_interval_msec>0){
            QThread::msleep(round_wait_interval_msec);
        }

    }while(!is_marked_to_stop);


    emit executorStageChange(STAGE::Done);
    

}

void ACD::TaskExecutor::onStopTask()
{
    is_marked_to_stop=true;
}

void ACD::TaskExecutor::onAbortExecution()
{
    is_marked_to_stop=true;
    is_marked_to_abort=true;
}

void ACD::TaskExecutor::onProcessingPercentChange(float cur_percent)
{
    emit curentFileProcessChanged(cur_percent);
}

ACD::TaskExecutor::TaskExecutor(const ACD::TaskExecutor::TaskConfig &task_conf)
{
    working_thread=nullptr;

    my_executor.IsOverrideExisting(task_conf.is_override);
    my_executor.IsKeepInputFiles(task_conf.is_keep_input_files);
    my_executor.SetOutputDirectory(task_conf.output_directory);
    my_executor.SetBitMask(task_conf.task_mask);

    connect(&my_executor, &ACD::BitModifier::processingPercentChange, this, &ACD::TaskExecutor::onProcessingPercentChange);

    input_files_config=task_conf.input_file_config;


    files_wait_interval_msec=task_conf.files_wait_interval_msec;
    round_wait_interval_msec=task_conf.round_wait_interval_msec;



    if(task_conf.round_wait_interval_msec==-1){
        is_marked_to_stop=true;
    }
    else{
        is_marked_to_stop=false;
    }

    working_thread=QThread::create([this](){
        this->Process();
    });

    emit executorStageChange(STAGE::AwaitingFiles);
    is_marked_to_stop=false;
    is_marked_to_abort=false;

    fail_info="";

}

void ACD::TaskExecutor::StartProcessing()
{
    working_thread->start();
}


QString ACD::TaskExecutor::GetFailInfo()
{
    return fail_info;
}

ACD::TaskExecutor::~TaskExecutor()
{
    is_marked_to_abort=true;
    is_marked_to_stop=true;

    if(working_thread!=nullptr){
        working_thread->wait();
        delete working_thread;
    }
    
}