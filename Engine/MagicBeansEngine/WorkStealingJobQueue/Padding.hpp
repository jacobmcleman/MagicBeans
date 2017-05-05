#pragma once


namespace Utilities
{
    /*
        Struct to create a version of a type that is a multiple of a specified size for cache-line optimizations

        T should support copy construction so that it can be placed into struct

        If compiled in debug mode, the padding bytes will be filled with 0s
        otherwise their contents is undefined
    */
    template<typename T, unsigned Size>
    struct Padded
    {
        //Ensure that the size of the object will always be a multiple of Size
        //if T is a perfect fit, this will be 0
        static constexpr unsigned PadBytes = (sizeof(T) % Size != 0) ? ((sizeof(T) / Size) + 1) * Size - sizeof(T) : 0;

        //Actual data portion
        T value;

        //Bytes to increase the size of the final struct
        const char padding[PadBytes];

        //Implicit conversion to the underlying type
        operator T() const
        {
            return value;
        }
        operator T&()
        {
            return value;
        }

        Padded(const T& data) : value(data)
        {
            //In debug mode set these bytes to 0
#ifdef _DEBUG
            std::memset(padding, 0, PadBytes);
#endif
        }
    };
}