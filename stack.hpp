#include <iostream>
#include <cstdio>
#include <cstring>
#include <typeinfo>

template<typename T>
constexpr bool isUseblaType = std::is_same<T, int>::value 
  || std::is_same<T, char>::value 
  || std::is_same<T, bool>::value;

#ifdef private
# undef private
#endif
template<class T>
class Stack{
  
  static_assert(isUseblaType<T>, "not usable type. Int, char, bool only.");
  
  public:
    enum Error{
        OK = 0,
        MAX_SIZE_EXCEEDED = -1,
        ALLOCATION_FAILED = -2,
        MEMORY_CORRUPTED = -3
    };

  private:    
    static const int MAX_SIZE = (1 << 20) - 1;
    static const int GROWTH_FACTOR = 2;
    static const int ARRAY_CANARY_SIZE = 2;
    char const *CANARY_OBJECT = "_canary\0";

    T* array;

    char metaDataCanaryLeft[8] = "\0";
    
    int capacity = 0;
    int size = 0;
    
    long long arrayHash = 0;
    
    char metaDataCanaryRight[8] = "\0";
    
    void Dump(const char* message) const {
        if (message != nullptr) {
            printf("%s\n", message);
        }
        
        printf("size: %i\n", size);
        printf("capacity: %i\n", capacity);
        
        for (int i = 0; i < capacity + ARRAY_CANARY_SIZE * 2; ++i) {
           std::cout << array[i] << " ";
        }
        
        printf("\n");
    }
    
    bool IsArrayCanaryValid(int indexInArray, const char* descriptionOfPosition) const {
        if (memcmp(array + indexInArray, CANARY_OBJECT, std::min(sizeof(T), strlen(CANARY_OBJECT))) != 0) {
            printf("Memory on %s is corrupted.\n", descriptionOfPosition);
            Dump(nullptr);
            
            return false;
        }
        return true;
    }
    
    bool IsDataCanaryValid() const {
        bool leftValidity = (strcmp(metaDataCanaryLeft, CANARY_OBJECT) == 0);
        bool rightValidity = (strcmp(metaDataCanaryRight, CANARY_OBJECT) == 0);
        
        if (!leftValidity || !rightValidity) {
            if (!leftValidity && !rightValidity) {
                printf("Left and right bounds of metadata are corrupted.\n");
            } else if (!leftValidity) {
                printf("Left bound of metadata is corrupted.\n");
            } else {
                printf("Right bound of metadata is corrupted.\n");
            }
            Dump(nullptr);
            
            return false;
        }
        
        return true;
    }
    
    long long GetArrayHash() const {
        const long long mod = 1e9 + 7;
        const long long p = 37;
        
        long long result = 0;
        
        long long currentKoefficient = 1;
        
        for (int i = ARRAY_CANARY_SIZE; i < ARRAY_CANARY_SIZE + capacity; ++i) {
            result += currentKoefficient * (array[i] + 1);
            result %= mod;
            
            currentKoefficient *= p;
            currentKoefficient %= mod;
        }
        
        return result;
    }
    
    Error Ok() const {
        for (int i = 0; i < ARRAY_CANARY_SIZE; ++i) {
            if (!IsArrayCanaryValid(i, "left side of buffer") || 
                !IsArrayCanaryValid(ARRAY_CANARY_SIZE + capacity + i, "right side of buffer")
            ) {
                return Error(MEMORY_CORRUPTED);
            }
        }
        
        if (!IsDataCanaryValid()) {
            return Error(MEMORY_CORRUPTED);
        }
        
        if (arrayHash != GetArrayHash()) {
            return Error(MEMORY_CORRUPTED);
        }
        
        return Error(OK); 
    }
    
    Error TryToResizeBuffer(bool toIncrease = true) {
        int tempCapacity = toIncrease ? capacity * GROWTH_FACTOR : capacity / GROWTH_FACTOR;
        if (tempCapacity > MAX_SIZE) {
            printf("Max size exceeded (%i elements).\n", capacity);
            return Error(MAX_SIZE_EXCEEDED);
        }
        
        if (tempCapacity == 0) {
            return Error(OK);
        }
        
        T* tempArray = new T[tempCapacity + ARRAY_CANARY_SIZE * 2];
        if (tempArray == nullptr) {
            printf("On %s:", toIncrease ? "increasing of buffer" : "reducing of buffer");
            Dump("Can not change buffer: allocation failed");
            return Error(ALLOCATION_FAILED);
        }
        
        for (int i = ARRAY_CANARY_SIZE; i < ARRAY_CANARY_SIZE + (toIncrease ? capacity : tempCapacity); ++i) {
            tempArray[i] = array[i];
        }
        
        for (int i = 0; i < ARRAY_CANARY_SIZE; ++i) {
            tempArray[i] = array[i];
            tempArray[ARRAY_CANARY_SIZE + tempCapacity + i] = array[ARRAY_CANARY_SIZE + capacity + i];
        }
        
        capacity = tempCapacity;
        
        delete[] array;
        array = tempArray;
        
        return Error(OK);
    }
    
  public:
    Stack() {
        size = 0;
        capacity = 1;
        arrayHash = 1;
        array = new T[capacity + ARRAY_CANARY_SIZE * 2];
        
        if (array == nullptr) {
            Dump("Can not create buffer: allocation has failed");
            return;
        }
        
        for (int i = 0; i < ARRAY_CANARY_SIZE; ++i) {
            memcpy(array + i, CANARY_OBJECT, std::min(sizeof(T), strlen(CANARY_OBJECT)));
            memcpy(array + ARRAY_CANARY_SIZE + capacity + i, CANARY_OBJECT, std::min(sizeof(T), strlen(CANARY_OBJECT)));
        }
        
        //arrayHash = GetArrayHash();
        
        metaDataCanaryLeft[0] = '\0';
        metaDataCanaryRight[0] = '\0';
        strcat(metaDataCanaryLeft, CANARY_OBJECT);
        strcat(metaDataCanaryRight, CANARY_OBJECT);
    }
    
    ~Stack() {
        delete[] array;
    }
    
    void Push(T element) {
        if (Ok() != Error::OK) {
            return;
        }
        
        if (size == capacity) {
            Error errorCode = TryToResizeBuffer(true);
            if (errorCode != Error::OK) {
                return;
            }
        }
        
        array[ARRAY_CANARY_SIZE + size] = element;
        ++size;
    
        arrayHash = GetArrayHash();
    }

    void Pop() {
        if (size == 0) {
            return;
        }
        --size;
        
        if (size * GROWTH_FACTOR < capacity) {
            TryToResizeBuffer(false);
        }
        
        arrayHash = GetArrayHash();
    }

    T GetTop() const {
        if (Ok() != Error::OK) {
            return array[0];
        }
        
        if (size != 0) {
            return array[ARRAY_CANARY_SIZE + size - 1];
        } else {
            printf("Attempt to get value from empty stack. Trash value was returned.\n");
            return array[0];
        }
    }
    
    void Print() const {
        Dump(nullptr);
    }
};

