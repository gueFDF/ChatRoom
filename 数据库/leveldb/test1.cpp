#include<iostream>
#include<string>
#include<assert.h>
#include"leveldb/db.h"
#include"leveldb/options.h"
#include"leveldb/status.h"
#include"leveldb/write_batch.h"
using namespace std;
int main()
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing=true;
    string key;
    string value;
    //open
    leveldb ::Status status =db->Open(options,"./tesdb",&db);
    assert(status.ok());
    //write
    status =db->Put(leveldb::WriteOptions(),"key_1","guo");
    assert(status.ok());
    status =db->Put(leveldb::WriteOptions(),"key_1","guo1");
    status =db->Get(leveldb::ReadOptions(),"key_1",&value);
    assert(status.ok());
    cout<<"value:"<<value<<endl;
    if(status.ok())
    {
        leveldb::WriteBatch  batch;
        batch.Delete("key_1");
        batch.Put("key_2","guo3");
        status=db->Write(leveldb::WriteOptions(),&batch);
    }
    status =db->Get(leveldb::ReadOptions(),"key_1",&value);
    if(!status.ok())
    {
        cout<<"所要查找的数据不存在"<<endl;
    }
    status=db->Get(leveldb::ReadOptions(),"key_2",&value);
    cout<<"value:"<<value<<endl;
    return 0;
}