#include <iostream>
#include <string>
#include <assert.h>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/status.h"
#include "leveldb/write_batch.h"
#include "leveldb/iterator.h"
using namespace std;
int main()
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    string key;
    string value;
    // open
    leveldb ::Status status = db->Open(options, "./tesdb", &db);
    assert(status.ok());
    // write
    status = db->Put(leveldb::WriteOptions(), "key_1", "guo");
    assert(status.ok());

    status = db->Get(leveldb::ReadOptions(), "key_1", &value);
    assert(status.ok());
    cout << "value:" << value << endl;
     if(status.ok())
     {
         leveldb::WriteBatch  batch;
         batch.Delete("key_1");
         batch.Put("key_2","a");
         batch.Put("key_3","b");
         batch.Put("key_4","c");
         batch.Put("key_5","d");
         batch.Put("key_6","e");
         batch.Put("key_7","f");
         batch.Put("key_8","g");
         batch.Put("key_9","o");
         batch.Put("key_10","p");
         batch.Put("key_11","q");
         batch.Put("key_12","r");

         status=db->Write(leveldb::WriteOptions(),&batch);
     }
    status = db->Get(leveldb::ReadOptions(), "key_1", &value);
    if (!status.ok())
    {
        cout << "所要查找的数据不存在" << endl;
    }
    status = db->Get(leveldb::ReadOptions(), "key_2", &value);
    cout << "value:" << value << endl;
    auto iter = db->NewIterator(leveldb::ReadOptions());
    for (iter->SeekToFirst(); iter->Valid(); iter->Next())
    {
        cout << iter->key().ToString() << " : " << iter->value().ToString() << "   ";
    }
    cout << endl;

    delete db;
    return 0;
}