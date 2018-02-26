#pragma once
#ifndef NATIVE_EXPORT_H
#define NATIVE_EXPORT_H

#ifdef _WIN32
#  define M_EXPORT __declspec(dllexport)
#endif


#endif // NATIVE_EXPORT_H