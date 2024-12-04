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

    // Забороняємо копіювання
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    // Дозволяємо переміщення
    MyUniquePtr(MyUniquePtr&& other) : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }
    MyUniquePtr& operator=(MyUniquePtr&& other) 
    {
        if (this != &other) 
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Оператор розіменування та доступу
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // Перевірка на порожній вказівник
    bool isNull() const { return ptr == nullptr; }

    // Звільнення ресурсу
    void reset(T* resource) 
    {
        delete ptr;
        ptr = resource;
    }
    void reset()
    {
        delete ptr;
        ptr = nullptr;
    }
    // Деструктор
    ~MyUniquePtr() { delete ptr; }
};

int main() 
{
    MyUniquePtr<int> uptr(new int(42));
    cout << *uptr << "\n";

    MyUniquePtr<int> uptr2 = move(uptr); // Переміщення
    if (uptr.isNull()) cout << "uptr is null\n";
    cout << *uptr2 << "\n";
    uptr.reset(new int(100));
    cout << *uptr << "\n";
    return 0;
}