#ifndef VEC_H
#define VEC_H

#define DEFAULT_SIZE 32

template<typename T>
class Vector {
public:
    Vector() : Vector(DEFAULT_SIZE) {}
    Vector(int length);
    Vector(const T& ref, int count);
    Vector(const Vector& other) { *this = other; }
    Vector(Vector&& other) { *this = other; }
    
    ~Vector();
    
    Vector<T>& operator=(const Vector<T>& other);
    Vector<T>& operator=(Vector<T>&& other);
    
    int size() const { return this->used; }
    int get_capacity() const { return allocated; }
    operator const T*() const { return this->data; }
    T& operator[](int pos) { return data[pos]; }
    const T& operator[](int pos) const { return data[pos]; }
    
    void clear();    
    void push_back(const T& elem) { insert(elem, used); }
    void insert(const T& elem, int pos);
    void pop_back() { erase(used); }
    void erase(int pos);
    
private:
    void realloc(int size);
    
    T* data;
    int allocated;
    int used;
};

#include "vec.impl"

#endif // VEC_H
