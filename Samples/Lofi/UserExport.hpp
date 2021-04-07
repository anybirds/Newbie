#pragma once

/* disable dll-interface warning */
#pragma warning( disable : 4251 ) 

/* disable attribute recognition warning */
#pragma warning( disable : 5030 )

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
extern "C" {
    void __declspec(dllexport) type_init();
    void __declspec(dllexport) type_clear();
}
#else
#  define DECL_EXPORT     __attribute__((visibility("default")))
#  define DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(USER_LIBRARY)
#  define USER_EXPORT DECL_EXPORT
#else
#  define USER_EXPORT DECL_IMPORT
#endif