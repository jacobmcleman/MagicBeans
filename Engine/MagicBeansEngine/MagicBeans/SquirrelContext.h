#pragma once
#include <string>

struct SQVM;

enum struct SqType
{
    Null, Integer, Float, Bool,
    String, Table, Array, Userdata,
    Closure, NativeClosure, Generator,
    UserPointer, Thread, Class, Instance,
    Reference, 
    UNKNOWN
};

class SquirrelContext
{
public:
    SquirrelContext(unsigned aStackSize = 2048);
    SquirrelContext(const SquirrelContext&) = delete;
    ~SquirrelContext();

    /*
        Basic stack interaction/manipulation operations
    */
    void MoveToTop(long long aIndex);
    void Pop(unsigned aNumElements = 1);
    void Remove(long long aIndex);
    long long GetStackTopIndex() const;
    void ResizeStack(long long aNewTopIndex);
    SqType GetType(long long aIndex);

    /*
        Add values of types to stack
    */
    void Push(const std::string& aString);
    void Push(float aFloat);
    void Push(long long aInteger);
    void Push(void* aPointer);
    void Push(bool aBool);
    void PushNull();

    /*
        Sets the function at the top of the stack
        to be squirrels error callback. 

        Callback Function should accept 2 parameters -
            Environment object (this)
            An object (any squirrel type)
    */
    void SetStackTopAsErrorHandler();

    /*
        Compile a file
    */
    bool CompileFile(const std::string& filename);

    template<typename... Args>
    void Call(const std::string& aFunctionName, Args... arguments);

    template<typename T, typename ...Values>
    void Push(T value, Values... values);

private:
    SQVM* vm_;

    void PushRootTable();
};

template<typename ...Args>
inline void SquirrelContext::Call(const std::string& aFunctionName, Args ... arguments)
{
    PushRootTable();
    Push(aFunctionName);
    sq_get(vm_, -2);

    PushRootTable();
    Push(arguments);

    sq_call(vm_, sizeof...(Args), false);

    //Pop the function and root table
    Pop(2);
}

template<typename T, typename ...Values>
inline void SquirrelContext::Push(T value, Values ...values)
{
    Push(value);
    Push(values...);
}
