#include <fstream>
#include <squirrel.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>

#include "Logger.h"
#include "SquirrelContext.h"


SQInteger ReadFunction(void* aFile)
{
    int ret;
    char c;

    ret = fread(&c, sizeof(c), 1, reinterpret_cast<FILE*>(aFile)) > 0;

    if (ret) return c;
    else return 0;
}

void CompilerErrorHandler(HSQUIRRELVM aVM, const SQChar* aDesc, const SQChar* aScr, SQInteger aLineNum, SQInteger aCol)
{
    (void)aVM;

    std::string message = "Squirrel Script [";
    message += aScr;
    message += "](";
    message += std::to_string(aLineNum);
    message += ", ";
    message += std::to_string(aCol);
    message += ") - ERROR: ";
    message += aDesc;
    LOG(message);
}

SquirrelContext::SquirrelContext(unsigned aStackSize)
{
    vm_ = sq_open(aStackSize);
    sq_setcompilererrorhandler(vm_, CompilerErrorHandler);

    //Register standard library to root table
    PushRootTable();
    sqstd_register_bloblib(vm_);
    sqstd_register_iolib(vm_);
    sqstd_register_systemlib(vm_);
    sqstd_register_mathlib(vm_);
    sqstd_register_stringlib(vm_);
}

SquirrelContext::~SquirrelContext()
{
    sq_close(vm_);
}

void SquirrelContext::MoveToTop(long long aIndex)
{
    sq_push(vm_, aIndex);
}

void SquirrelContext::Pop(unsigned aNumElements)
{
    sq_pop(vm_, aNumElements);
}

void SquirrelContext::Remove(long long aIndex)
{
    sq_remove(vm_, aIndex);
}

long long SquirrelContext::GetStackTopIndex() const
{
    return sq_gettop(vm_);
}

void SquirrelContext::ResizeStack(long long aNewTopIndex)
{
    sq_settop(vm_, aNewTopIndex);
}

void SquirrelContext::Push(const std::string& aString)
{
    sq_pushstring(vm_, aString.data(), aString.size());
}

void SquirrelContext::Push(float aFloat)
{
    sq_pushfloat(vm_, aFloat);
}

void SquirrelContext::Push(long long aInteger)
{
    sq_pushinteger(vm_, aInteger);
}

void SquirrelContext::Push(void * aPointer)
{
    sq_pushuserpointer(vm_, aPointer);
}

void SquirrelContext::Push(bool aBool)
{
    sq_pushbool(vm_, aBool);
}

void SquirrelContext::PushNull()
{
    sq_pushnull(vm_);
}

void SquirrelContext::SetStackTopAsErrorHandler()
{
    sq_seterrorhandler(vm_);
}

bool SquirrelContext::CompileFile(const std::string & filename)
{
    FILE *f;
    errno_t error = fopen_s(&f, filename.c_str(), "rb");

    if (!error)
    {
        sq_compile(vm_, ReadFunction, f, filename.c_str(), 1);
        fclose(f);
        return true;
    }
    else
    {
        return false;
    }
}

void SquirrelContext::PushRootTable()
{
    sq_pushroottable(vm_);
}

SqType TypeConvert(SQObjectType type)
{
    switch (type)
    {
    case OT_NULL: return SqType::Null;
    case OT_INTEGER: return SqType::Integer;
    case OT_FLOAT: return SqType::Float;
    case OT_BOOL: return SqType::Bool;
    case OT_STRING: return SqType::String;
    case OT_TABLE: return SqType::Table;
    case OT_ARRAY: return SqType::Array;
    case OT_USERDATA: return SqType::Userdata;
    case OT_CLOSURE: return SqType::Closure;
    case OT_NATIVECLOSURE: return SqType::NativeClosure;
    case OT_GENERATOR: return SqType::Generator;
    case OT_USERPOINTER: return SqType::UserPointer;
    case OT_THREAD: return SqType::Thread;
    case OT_CLASS: return SqType::Class;
    case OT_INSTANCE: return SqType::Instance;
    case OT_WEAKREF: return SqType::Reference;
    default: return SqType::UNKNOWN;
    }
}

SqType SquirrelContext::GetType(long long aIndex)
{
    return TypeConvert(sq_gettype(vm_, aIndex));
}
