#include <stdio.h>
class DataElement
{
    friend class DataElementEx;
public:
    DataElement() { count_++;}

    DataElement (int data) : data_(data) { count_++;}

    DataElement (const DataElement& e)
    {
        data_ = e.getData();
        count_++;
    }

    DataElement & operator= (const DataElement& e)
    {
        data_ = e.getData();
        return *this;
    }

    bool operator== (const DataElement& e)
    {
        return this->data_ == e.data_;
    }

    ~DataElement () {count_--;}

    int getData(void) const {return data_; }
    void setData( int val) {data_ = val; }
     int numOfActiveObjects (void) {return count_;}
     void clear(void) {count_ = 0;}
    
public:
    int data_;
     int count_;
};

