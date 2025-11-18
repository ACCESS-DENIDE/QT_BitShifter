#include "bit_modifier.hpp"

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QList>
#include <QTextStream>

#ifdef DEBUG_BIG_WORK_TIME_BIT_MOD
#include <QThread>
#endif

char ACD::BitModifier::XOR(const char a, const char b)
{
    return a^b;
}

ACD::BitModifier::BitModifier()
{

}

void ACD::BitModifier::SetBitMask(const BitMask &new_mask)
{
    for(int i=0; i<MASK_SIZE_BYTES; i++){
        applicable_mask[i]=new_mask[i];
    }
}

void ACD::BitModifier::SetOutputDirectory(const QString &new_output_dir)
{
    output_dir=new_output_dir;
}

ACD::BitModifier::ERROR_CODES ACD::BitModifier::ApplyToFile(const QString &input_file_path)
{
    int total_size_bytes=0;
    int processed_size=0;
    QFileInfo input_file_info(input_file_path);

    total_size_bytes=input_file_info.size();

    QDir output_file_dir(output_dir);
    QString output_file_path=output_dir;

    if(!is_override_existing){
        QString u_outp_filename(input_file_info.baseName());
        int cou=0;
        while(output_file_dir.exists(u_outp_filename+"."+input_file_info.completeSuffix())){
            cou++;
            u_outp_filename=input_file_info.baseName();
            u_outp_filename.append('_');
            u_outp_filename.append(QString::number(cou));
        }
        output_file_path.push_back(QDir::separator());
        output_file_path.append(u_outp_filename);
    }
    else{
        output_file_path.push_back(QDir::separator());
        output_file_path.append(input_file_info.baseName());
    }
    output_file_path.append('.');
    output_file_path.append(input_file_info.completeSuffix());

    QFile input_file_access(input_file_path);
    QFile output_file_access(output_file_path);
    if(!input_file_access.open(QIODeviceBase::ReadOnly)){
        return ERROR_CODES::FileInaccessable;
    }

    if(input_file_path==output_file_path){
        
        if(!output_file_access.open(QIODeviceBase::ReadWrite)){
            input_file_access.close();
            return ERROR_CODES::OutputDirectoryInaccessable;
        }

    }else{

        if(!output_file_access.open(QIODeviceBase::WriteOnly)){
            input_file_access.close();
            return ERROR_CODES::OutputDirectoryInaccessable;
        }

    }
    
    QTextStream in(&input_file_access);
    output_file_access.seek(0);
    QTextStream out(&output_file_access);
    

    QString inp_str="";
    QString out_string="";
    int mask_cou=0;
    while(!in.atEnd()){
        inp_str=in.read(MAX_CHUNK_SIZE*MASK_SIZE_BYTES);

#ifdef DEBUG_BIG_WORK_TIME_BIT_MOD
        QThread::msleep(DEBUG_BIG_WORK_TIME_BIT_MOD);
#endif
        

        out_string="";
        for(int i=0; i<inp_str.size(); i++){
            out_string.push_back(XOR(inp_str[i].toLatin1(), applicable_mask[mask_cou]));
            mask_cou++;
            if(mask_cou>=MASK_SIZE_BYTES){
                mask_cou=0;
            }
        }
        out<<out_string;
        processed_size+=MAX_CHUNK_SIZE*MASK_SIZE_BYTES;
        if(processed_size>total_size_bytes){
            processed_size=total_size_bytes;
        }
        emit processingPercentChange(float(processed_size)/(float(total_size_bytes)/100.0f));
    }
    
    in.flush();
    input_file_access.close();
    out.flush();
    output_file_access.flush();

    if(!is_keep_input){
        QFile::remove(input_file_path);
    }

    return ERROR_CODES::Ok;
}

void ACD::BitModifier::IsOverrideExisting(bool is_override)
{
    is_override_existing=is_override;
}

void ACD::BitModifier::IsKeepInputFiles(bool is_keep)
{
    is_keep_input=is_keep;
}

ACD::BitModifier::~BitModifier()
{

}