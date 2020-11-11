/*
Copyright (c) 2016 Alex Vargas

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#define ERROR_NO_MEMORY 1

#define ERROR_IMPORT_TESTREGISTRY 100
#define ERROR_REMOVE_TESTREGISTRY 101
#define ERROR_KEYEXISTS 102
#define ERROR_CREATEKEY 103
#define ERROR_PROPERTYTYPE 104
#define ERROR_PROPERTYEXISTS 105
#define ERROR_ENUMERATEKEYS 106
#define ERROR_ENUMERATEPROPERTIES 107
#define ERROR_DELETEKEY 108
#define ERROR_DELETEPROPERTY 109
#define ERROR_REGPATHCONVERT 110
#define ERROR_COPYTREE 111

#define ERROR_GETSTRING 200
#define ERROR_SETSTRING 201
#define ERROR_GETEXPSTRING 202
#define ERROR_SETEXPSTRING 203
#define ERROR_GETMULTISTRING 204
#define ERROR_SETMULTISTRING 205

#define ERROR_GETDWORD 300
#define ERROR_SETDWORD 301
#define ERROR_GETDWORDBE 302
#define ERROR_SETDWORDBE 303
#define ERROR_GETQWORD 304
#define ERROR_SETQWORD 305
#define ERROR_GETBINARY 306
#define ERROR_SETBINARY 307

int testAll();