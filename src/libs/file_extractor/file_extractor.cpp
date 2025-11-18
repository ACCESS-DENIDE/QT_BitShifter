#include "file_extractor.hpp"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

#ifdef DEBUG_BIG_WORK_TIME_FILE_EXECUTER
#include <QThread>
#endif

void StepDeeper(QStringList& files_list, QStringList& entries, QString cur_path, uint depth);

QStringList ACD::GetAllFileByMask(const QString &mask)
{
    QDir probe_dir(mask);
    QStringList all_entries;
    
    QString demo=probe_dir.absolutePath();

    all_entries=probe_dir.absolutePath().split(QDir::separator());
    
    if(all_entries[0]==""){
        all_entries.removeFirst();
    }

    QStringList ret;


    StepDeeper(ret, all_entries, "", 0);
    
#ifdef DEBUG_BIG_WORK_TIME_FILE_EXECUTER
    QThread::msleep(DEBUG_BIG_WORK_TIME_FILE_EXECUTER);
#endif


    return ret;
}

QStringList ACD::GetAllFileByConfig(const QString &config_string)
{
    if(!config_string.contains(';')){
        return GetAllFileByMask(config_string);
    }

    QStringList mask_list=config_string.split(';');
    QStringList output_list;
    for (QStringList::iterator it =mask_list.begin(); it!=mask_list.end(); it++){

        if((*it)==" "){
            continue;
        }
        (*it)=(*it).trimmed();
        output_list.append(GetAllFileByMask(*it));

    }

    return output_list;
}

void StepDeeper(QStringList& files_list, QStringList& entries, QString cur_path, uint depth){
    if(entries.size()<=depth+1){
        if(entries[depth].contains('*')){
            QDir cur_dir(cur_path);
            QStringList availib_entries=cur_dir.entryList(QDir::Files);

            QFileInfo wildcard_file(entries[depth]);
            QString expected_suffix=wildcard_file.completeSuffix();

            for (QStringList::iterator it =availib_entries.begin(); it!=availib_entries.end(); it++){

                QFileInfo probe_file(*it);
                if(probe_file.completeSuffix()==expected_suffix){
                    files_list.append(cur_path+QDir::separator()+(*it));
                }
                

            }

        }else{
            cur_path.append(QDir::separator());
            cur_path.append(entries[depth]);

            QFileInfo temp_file(cur_path);

            if(temp_file.exists()){
                files_list.append(cur_path);
                return;
            }
            return;

        }
    }
    else{
        QDir cur_dir(cur_path);
        if(!cur_dir.exists()){
            return;
        }
        if(entries[depth]=="*"){
            QStringList availib_entries=cur_dir.entryList(QDir::Dirs);
            for (QStringList::iterator it =availib_entries.begin(); it!=availib_entries.end(); it++){

                StepDeeper(files_list, entries, cur_path+QDir::separator()+(*it), depth+1);

            }
        }else if(entries[depth]==".."){
            while(cur_path[cur_path.size()-1]!=QDir::separator()){
                cur_path.removeLast();
            }
            cur_path.removeLast();

            StepDeeper(files_list, entries, cur_path, depth+1);

        }else{
            cur_path.append(QDir::separator());
            cur_path.append(entries[depth]);
            StepDeeper(files_list, entries, cur_path, depth+1);
        }
    }
}