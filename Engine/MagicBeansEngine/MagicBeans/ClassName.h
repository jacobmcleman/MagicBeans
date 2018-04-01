#pragma once

template<typename T>
inline const char* ClassName(T) {
    return ClassName<T>();
}

template<typename T>
inline const char* ClassName() {
    return typeid(T).name();
}