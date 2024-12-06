#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class MyUniquePtr 
{
private:
    T* ptr; 

public:
    MyUniquePtr() : ptr(nullptr) {}
    MyUniquePtr(T* resource) : ptr(resource) {}

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

   
    MyUniquePtr& Move(MyUniquePtr& other) 
    {
        if (this != &other) 
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* Get() const { return ptr; }

    bool isNull() const { return ptr == nullptr; }

    void reset()
    {
        delete ptr;
        ptr = nullptr;
    }
 
    ~MyUniquePtr() { delete ptr; }
};



template <typename T>
class MySharedPtr 
{
private:
    T* ptr;
    int* count;

public:
    MySharedPtr() : ptr(nullptr), count(nullptr) {}
    MySharedPtr(T* resource) : ptr(resource), count(new int(1)) {}

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) 
    {
        if (count) ++(*count);
    }

    MySharedPtr& operator=(const MySharedPtr& other) 
    {
        if (this != &other) 
        {
            reset(); 
            ptr = other.ptr;
            count = other.count;
            if (count) ++(*count);
        }
        return *this;
    }

    ~MySharedPtr() 
    {
        reset();
    }

    T& operator*() const { return *ptr; }
    T* Get() const { return ptr; }

    void reset() 
    {
        if (count)
        {
            --(*count);
            if (*count == 0)
            {
                delete ptr;
                delete count;
            }
        }
        ptr = nullptr;
        count = nullptr;
    }

    int Count() const 
    {
        if (count) return *count;
        else { return 0; }
    }

    bool isNull() const { return ptr == nullptr; }
};


int main() 
{
    MyUniquePtr<int> uptr(new int(100));
    cout << *uptr << "\n"; 

    MyUniquePtr<int> uptr2;
    uptr2.Move(uptr); 
    if (uptr.isNull()) cout << "uptr is null\n"; 
    cout << *uptr2 << "\n"; 

    uptr2.reset();
    if (uptr2.isNull()) cout << "uptr2 has been reset\n"; 



    MySharedPtr<int> sp1(new int(100));
    cout << "Value: " << *sp1 << ", sp1 count: " << sp1.Count() << "\n";

    MySharedPtr<int> sp2 = sp1;
    cout << "After sp2 copy, sp1 count: " << sp1.Count() << "\n";

    sp2.reset();
    if (sp2.isNull()) cout << "sp2 is null, sp2 count : " << sp2.Count() << "\n";

 
    cout << "After sp2 reset, sp1 count: " << sp1.Count() << "\n";

    sp1.reset(); 
    cout << "After sp1 reset, sp1 count: " << sp1.Count() << "\n";
    return 0;
}