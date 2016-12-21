/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef PLUGINDEFS_H
#define PLUGINDEFS_H

#ifdef WIN32
# define WT_PLUGIN_EXPORT __declspec(dllexport)
# define WT_APICALL __cdecl
#else
# define WT_APICALL
# define WT_PLUGIN_EXPORT // empty
#endif

#endif // PLUGINDEFS_H
