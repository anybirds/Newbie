#ifndef CUSTOM_GLOBAL_H
#define CUSTOM_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT     __attribute__((visibility("default")))
#  define DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(CUSTOM_LIBRARY)
#  define CUSTOM_EXPORT DECL_EXPORT
#else
#  define CUSTOM_EXPORT DECL_IMPORT
#endif

#endif // CUSTOM_GLOBAL_H
