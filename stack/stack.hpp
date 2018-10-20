/*! \file */

#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <typeinfo>

#define NO_TRACE nullptr
#define NO_MESSAGE nullptr

/*! ---------------------------------
* @brief Safety stack with some 
*        security levels
*
* -----------------------------------
*/
#ifdef private
# undef private
#endif
template<class T>
class Stack{

    /*! ---------------------------------
    * @brief Enum of possible methods errors
    *
    * -----------------------------------
    */
    enum Error{
        OK = 0,
        MAX_SIZE_EXCEEDED = -1,
        ALLOCATION_FAILED = -2,
        MEMORY_CORRUPTED = -3
    };

  private:
    char metaDataCanaryLeft[8] = "\0";
  
    ///Capacity cannot be more than MAX_SIZE
    static const int MAX_SIZE = (1 << 20) - 1;
    ///Koefficient of changing of capacity
    static const int GROWTH_FACTOR = 2;
    ///Object comparing with canaries
    char const *CANARY_OBJECT = "_canary\0";
    ///Hasher
    std::hash<T> hasher;
    
    int capacity = 0;
    int size = 0;
    
    T* array;
    
    long long hash = 0;
    
    char metaDataCanaryRight[8] = "\0";
    
    static void PrintError(char const* description) {
        if (description != nullptr) {
            fprintf(stderr, "\033[31mError:\033[0m %s\n", description);
        }
    }
    
    /*! ---------------------------------
    * @brief Print all main information
    *        about exemplar of class
    *
    * @param[in] message  Message to explain reason of causing
    *
    * -----------------------------------
    */
    void Dump(const char* message = NO_MESSAGE, char const* trace = NO_TRACE) const {
        if (trace != NO_TRACE) {
            fprintf(stderr, "\033[34mDump from:\033[0m %s\n", trace);
        }
        
        PrintError(message);
        
        fprintf(stderr, "\033[33msize:\033[0m %i\n", size);
        fprintf(stderr, "\033[33mcapacity:\033[0m %i\n", capacity);
        
        fprintf(stderr, "\033[33marray:\033[0m ");
        for (int i = 0; i < size; ++i) {
           std::cout << array[i] << " ";
        }
        
        std::cout << std::endl;
    }
    
    /*! ---------------------------------
    * @brief Check canaries on validity
    *
    * @param[in] toComment Whether the method should 
    *                      comment on detected errors.
    *
    * @returns  True of valid and false else
    *
    * -----------------------------------
    */
    bool TestCanaryOnValidity(bool toComment) const {
        bool leftValidity = (strcmp(metaDataCanaryLeft, CANARY_OBJECT) == 0);
        bool rightValidity = (strcmp(metaDataCanaryRight, CANARY_OBJECT) == 0);
        
        if (!leftValidity || !rightValidity) {
            if (toComment) {
                if (!leftValidity && !rightValidity) {
                    Dump("Left and right bounds of metadata are corrupted", __PRETTY_FUNCTION__);
                } else if (!leftValidity) {
                    Dump("Left bound of data is corrupted", __PRETTY_FUNCTION__);
                } else {
                    Dump("Right bound of data is corrupted", __PRETTY_FUNCTION__);
                }
            }
            
            return false;
        }
        
        return true;
    }
    
    /*! ---------------------------------
    * @brief Get hash of main part of array (without canaries)
    *
    * @returns  Hash of main part of array
    *
    * -----------------------------------
    */
    long long GetHash() const {
        const long long mod = 1e9 + 7;
        const long long p = 37;
        
        long long result = 0;
        
        long long currentKoefficient = 1;
        
        for (int i = 0; i < capacity; ++i) {
            result += currentKoefficient * (hasher(array[i]) % mod);
            result %= mod;
            
            currentKoefficient *= p;
            currentKoefficient %= mod;
        }
        
        result += currentKoefficient * capacity;
        result %= mod;
        currentKoefficient *= p;
        currentKoefficient %= mod;
            
        result += currentKoefficient * size;
        result %= mod;
        
        return result;
    }
    
    /*! ---------------------------------
    * @brief Chech all the structure on validity
    *  
    * @param[in] toComment Whether the method should 
    *                      comment on detected errors.
    *
    * @returns  Enum Error
    *
    * -----------------------------------
    */
    Error Ok(bool toComment, char const* trace = NO_TRACE) const {
        if (!TestCanaryOnValidity(toComment)) {
            return Error(MEMORY_CORRUPTED);
        }
        if (hash != GetHash()) {
            if (toComment) {
                Dump("Hash does not match the valid hash", trace);
                exit(0);
            }
            return Error(MEMORY_CORRUPTED);
        }
        return Error(OK); 
    }
    
    /*! ---------------------------------
    * @brief Attempt to increase or resuce buffer
    *
    * @param[in] toIncrease  If true then buffer will be
    *                        increased and if false then reduced
    *
    * @param[in] toComment Whether the method should 
    *                      comment on detected errors.
    *
    * @returns  Enum Error
    *
    * @note Not rehash stack, do it after call
    *
    * -----------------------------------
    */
    Error TryToResizeBuffer(bool toIncrease, bool toComment) {
        int tempCapacity = toIncrease ? capacity * GROWTH_FACTOR : capacity / GROWTH_FACTOR;
        if (tempCapacity > MAX_SIZE) {
            if (toComment) {
                fprintf(stderr, "Max size exceeded (%i elements)\n", capacity);
            }
            return Error(MAX_SIZE_EXCEEDED);
        }
        
        if (tempCapacity == 0) {
            return Error(OK);
        }
        
        T* tempArray = new T[tempCapacity];
        if (tempArray == nullptr) {
            if (toComment) {
                fprintf(stderr, "On %s:", toIncrease ? "increasing of buffer" : "reducing of buffer");
                Dump("Can not change buffer: allocation failed", __PRETTY_FUNCTION__);
            }
            return Error(ALLOCATION_FAILED);
        }
        
        for (int i = 0; i < (toIncrease ? capacity : tempCapacity); ++i) {
            tempArray[i] = std::move(array[i]);
        }
        
        capacity = tempCapacity;
        
        delete[] array;
        array = tempArray;
        
        return Error(OK);
    }
    
    void CopyFrom(const Stack<T>& other) {
        delete[] array;
        
        size = other.size;
        capacity = other.capacity;
        
        array = new T[capacity]{T()};

        if (array == nullptr) {
            Dump("Can not create buffer: allocation has failed", __PRETTY_FUNCTION__);
        }
        
        for (int i = 0; i < size; ++i) {
            array[i] = other.array[i];
        }
        
        hash = GetHash();
    }
    
  public:
    Stack() {
        size = 0;
        capacity = 1;
        array = new T[capacity]{T()};
        hash = GetHash();
        
        if (array == nullptr) {
            Dump("Can not create buffer: allocation has failed", __PRETTY_FUNCTION__);
            return;
        }
        
        metaDataCanaryLeft[0] = '\0';
        metaDataCanaryRight[0] = '\0';
        strcat(metaDataCanaryLeft, CANARY_OBJECT);
        strcat(metaDataCanaryRight, CANARY_OBJECT);
    }
        
    Stack(const Stack<T>& other) {
        CopyFrom(other);
    }
    
    ~Stack() {
        delete[] array;
    }
    
    /*! ---------------------------------
    * @brief Pushes element on top of stack
    *
    * @param[in] element  Pushing element
    *
    * @returns  True if no errors and false else
    *
    * -----------------------------------
    */
    bool Push(T element) {
        if (Ok(true, __PRETTY_FUNCTION__) != Error::OK) {
            return false;
        }
        
        if (size == capacity) {
            Error errorCode = TryToResizeBuffer(true, true);
            if (errorCode != Error::OK) {
                return false;
            }
        }
        
        array[size] = element;
        ++size;
        hash = GetHash();
        
        return true;
    }

    /*! ---------------------------------
    * @brief Delete element on top of stack
    *
    * @returns  True if no errors and false else
    *
    * -----------------------------------
    */
    bool Pop() {
        if (Ok(true, __PRETTY_FUNCTION__) != Error::OK) {
            return false;
        }
        
        if (size == 0) {
            PrintError("Attempt to pop value from empty stack");
            return false;
        }
        
        --size;
        
        if (size * GROWTH_FACTOR < capacity) {
            TryToResizeBuffer(false, true);
        }
        
        hash = GetHash();
    
        return true;
    }

    /*! ---------------------------------
    * @brief  Get element on top of stack
    *
    * @param[in] result  Where to write top element
    *
    * @returns  True if no errors and false else
    *
    * @note  Don't deletes any element
    *
    * -----------------------------------
    */
    bool GetTop(T* result) const {
        if (result == nullptr) {
            return false;
        }
        if (Ok(true, __PRETTY_FUNCTION__) != Error::OK) {
            return false;
        }
        
        if (size != 0) {
            *result = array[size - 1];
            return true;
        } else {
            PrintError("Attempt to get value from empty stack");
            return false;
        }
    }

    /*! ---------------------------------
    * @brief Get current size of structure
    *
    * @returns  Size of structure
    *
    * -----------------------------------
    */
    int GetSize() const {
        return size;
    }

    /*! ---------------------------------
    * @brief Clear buffer
    *
    * -----------------------------------
    */
    void Clear() {
        if (Ok(true, __PRETTY_FUNCTION__) != Error::OK) {
            return;
        }
    
        size = 0;
        
        if (size * GROWTH_FACTOR < capacity) {
            TryToResizeBuffer(false, true);
        }
        
        hash = GetHash();
    }

    /*! ---------------------------------
    * @brief Print all main information
    *        about exemplar of class
    *
    * -----------------------------------
    */
    void Print() const {
        Dump(NO_MESSAGE, NO_TRACE);
    }

    Stack& operator=(const Stack<T>& other) {
        if (this == &other) {
            return *this;
        }
    
        if (Ok(true, __PRETTY_FUNCTION__) != Error::OK) {
            return *this;
        }
        
        if (other.Ok(false) != Error::OK) {
            PrintError("Right operand of assignment is broken, assignment canceled");
            return *this;
        }
        
        CopyFrom(other);
        
        return *this;
    }
};

