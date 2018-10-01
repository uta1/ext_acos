#include <iostream>
#include <cstdio>
#include <cstring>

#ifdef private
# undef private
#endif
template<class T>
class Stack{
  public:
    enum Error{
        OK = 0,
        MAX_SIZE_EXCEEDED = -1,
        ALLOCATION_FAILED = -2,
        MEMORY_CORRUPTED = -3
    };

  private:
    T CANARY_OBJECT = T();
    
    static const int MAX_SIZE = (1 << 20) - 1;
    static const int GROWTH_FACTOR = 2;
    static const int CANARY_SIZE = 2;
    
    T* canaryLeft = &CANARY_OBJECT;
    
    T* array;
    int capacity;
    int size;
    
    T* canaryRight = &CANARY_OBJECT;
    
    void Dump(const char* message) const {
        std::cout << "size: " << size << std::endl;
        std::cout << "capacity: " << capacity << std::endl;
        
        for (int i = 0; i < capacity; ++i) {
           std::cout << array[i] << " "; 
        }
        
        std::cout << std::endl << message << std::endl;
    }
    
    static int CraftCapacityFromSize(int size_) {
        if (size_ > MAX_SIZE) {
            return Error(MAX_SIZE_EXCEEDED);
        }
        
        int result = 1;
        for (result = 1; result < size_; result <<= 1);
        return result;
    }
    
    Error CheckForValidityOfCanary(T canary, const char* descriptionOfPosition) const {
        if (canary != CANARY_OBJECT) {
            const char* messagePrefix = "Memory is corrupted: ";
            char* dumpMessage = (char*)malloc(strlen(messagePrefix) + strlen(descriptionOfPosition));
            if (dumpMessage == nullptr) {
                Dump("On validation of canary: allocation failed. Somewhere memory is corrupted.");
                return Error(MEMORY_CORRUPTED);
            }
            strcat(dumpMessage, descriptionOfPosition);
            Dump(dumpMessage);
            delete dumpMessage;
            
            return Error(MEMORY_CORRUPTED);
        }
    }
    
    Error Ok() const {
        for (int i = 0; i < CANARY_SIZE; ++i) {
            if (false
                //CheckForValidityOfCanary(canaryLeft, "left side of buffer") || 
                //CheckForValidityOfCanary(canaryRight, "right side of buffer")
            ) {
                return Error(MEMORY_CORRUPTED);
            }
        }
        return Error(OK); 
    }
    
    Error TryToIncreaseBuffer() {
        int tempCapacity = capacity * GROWTH_FACTOR;
        if (tempCapacity > MAX_SIZE) {
            Dump("Can not increase buffer because in this case the maximum stack size will be exceeded");
            return Error(MAX_SIZE_EXCEEDED);
        }
        
        T* tempArray = new T[tempCapacity];
        if (tempArray == nullptr) {
            Dump("Can not increase buffer: allocation failed");
            return Error(ALLOCATION_FAILED);
        }
        
        for (int i = 0; i < capacity; ++i) {
            tempArray[i] = array[i];
        }
        
        /*for (int i = 0; i < CANARY_SIZE; ++i) {
            tempArray[i] = array[i];
            tempArray[i + tempCapacity] = array[i + capacity];
        }*/
        
        capacity = tempCapacity;
        
        delete array;
        array = tempArray;
        
        return Error(OK);
    }
    
  public:
    Stack() {
        size = 0;
        capacity = CraftCapacityFromSize(size);
        array = new T[capacity];

        if (!Ok()) {
            Dump("Can not create buffer: allocation has failed");
        }
    }
    
    void Push(T element) {
        if (size < capacity) {
            array[size] = element;
            ++size;
        } else {
            if (!Ok()) {
                Dump("");
            }
            Error errorCode = TryToIncreaseBuffer();
            if (errorCode != Error::OK) {}
        }
        Dump("");
    }

    void Pop() {
        if (size == 0) {
            return;
        }
        --size;
    }
};

int main () {
    Stack<int> s;
    for (int i = 0; i < 20;++i) {
        s.Push(rand()%10);
    }
    return 0;
}
