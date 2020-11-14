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

#include "test.h"
#include "registry.h"

#include <iostream>

using namespace std;

inline void assert(bool b, int error)
{
	if (!b)
	{
        cerr << endl << "-------------------------------" << endl;
		cerr << "TEST FAILED, code " << error << endl;
        cerr << "-------------------------------" << endl << endl;
		throw error;
	}
}

inline wstring getEnvVar(wstring name)
{
    wstring tempDir;
    const int len = 32768;
    wchar_t* buffer = new wchar_t[len];
    buffer[len - 1] = 0;
    auto sz = GetEnvironmentVariableW(name.c_str(), buffer, 32768);
    if (sz >= 32768)
    {
        delete[] buffer;
        assert(false, ERROR_NO_MEMORY);
    }
    else
    {
        tempDir = wstring(buffer, sz);
        delete[] buffer;
    }
    return tempDir;
}

inline void runsystem(string cmd, int error)
{
    cout << "calling \"" << cmd << "\"" << endl;
    auto b = system(cmd.c_str());
    if (b) throw error;
}

void testRegPath()
{
    wstring key;
    HKEY hive = winreg::splitHiveFromKey(L"", key);
    assert(hive == HKEY_CURRENT_USER, ERROR_REGPATHCONVERT);
    assert(key == L"", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"\\", key);
    assert(hive == HKEY_CURRENT_USER, ERROR_REGPATHCONVERT);
    assert(key == L"", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"\\HKLM:\\Software\\whatever", key);
    assert(hive == HKEY_CURRENT_USER, ERROR_REGPATHCONVERT);
    assert(key == L"HKLM:\\Software\\whatever", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"HKLM:\\Software\\whatever", key);
    assert(hive == HKEY_LOCAL_MACHINE, ERROR_REGPATHCONVERT);
    assert(key == L"Software\\whatever", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"HKLM\\Software\\whatever", key);
    assert(hive == HKEY_LOCAL_MACHINE, ERROR_REGPATHCONVERT);
    assert(key == L"Software\\whatever", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"HKEY_USERS:\\Software\\whatever", key);
    assert(hive == HKEY_USERS, ERROR_REGPATHCONVERT);
    assert(key == L"Software\\whatever", ERROR_REGPATHCONVERT);
    hive = winreg::splitHiveFromKey(L"HKEY_USERS\\Software\\whatever", key);
    assert(hive == HKEY_USERS, ERROR_REGPATHCONVERT);
    assert(key == L"Software\\whatever", ERROR_REGPATHCONVERT);
}

void testExistance()
{
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software"), ERROR_KEYEXISTS);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software", KEY_WOW64_64KEY), ERROR_KEYEXISTS);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software", KEY_WOW64_32KEY), ERROR_KEYEXISTS);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software"), ERROR_KEYEXISTS);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software", KEY_WOW64_64KEY), ERROR_KEYEXISTS);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software", KEY_WOW64_32KEY), ERROR_KEYEXISTS);

    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\sfgsdfgsdfgsdfg"), ERROR_KEYEXISTS);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\sfgsdfgsdfgsdfg", KEY_WOW64_64KEY), ERROR_KEYEXISTS);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\sfgsdfgsdfgsdfg", KEY_WOW64_32KEY), ERROR_KEYEXISTS);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\sfgsdfgsdfgsdfg"), ERROR_KEYEXISTS);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\sfgsdfgsdfgsdfg", KEY_WOW64_64KEY), ERROR_KEYEXISTS);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\sfgsdfgsdfgsdfg", KEY_WOW64_32KEY), ERROR_KEYEXISTS);

    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string"), ERROR_PROPERTYEXISTS);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string", KEY_WOW64_64KEY), ERROR_PROPERTYEXISTS);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string", KEY_WOW64_32KEY), ERROR_PROPERTYEXISTS);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string"), ERROR_PROPERTYEXISTS);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string", KEY_WOW64_64KEY), ERROR_PROPERTYEXISTS);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string", KEY_WOW64_32KEY), ERROR_PROPERTYEXISTS);

    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test strong"), ERROR_PROPERTYEXISTS);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test strong", KEY_WOW64_64KEY), ERROR_PROPERTYEXISTS);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test strong", KEY_WOW64_32KEY), ERROR_PROPERTYEXISTS);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test strong"), ERROR_PROPERTYEXISTS);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test strong", KEY_WOW64_64KEY), ERROR_PROPERTYEXISTS);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test strong", KEY_WOW64_32KEY), ERROR_PROPERTYEXISTS);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string") == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring") == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test expand string") == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary") == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword") == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword") == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe") == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string", KEY_WOW64_64KEY) == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", KEY_WOW64_64KEY) == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test expand string", KEY_WOW64_64KEY) == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", KEY_WOW64_64KEY) == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", KEY_WOW64_64KEY) == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", KEY_WOW64_64KEY) == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", KEY_WOW64_64KEY) == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string", KEY_WOW64_32KEY) == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", KEY_WOW64_32KEY) == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test expand string", KEY_WOW64_32KEY) == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", KEY_WOW64_32KEY) == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", KEY_WOW64_32KEY) == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", KEY_WOW64_32KEY) == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", KEY_WOW64_32KEY) == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string") == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring") == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test expand string") == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary") == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword") == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword") == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe") == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string", KEY_WOW64_64KEY) == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", KEY_WOW64_64KEY) == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test expand string", KEY_WOW64_64KEY) == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", KEY_WOW64_64KEY) == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", KEY_WOW64_64KEY) == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", KEY_WOW64_64KEY) == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", KEY_WOW64_64KEY) == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test string", KEY_WOW64_32KEY) == REG_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", KEY_WOW64_32KEY) == REG_MULTI_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test expand string", KEY_WOW64_32KEY) == REG_EXPAND_SZ, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", KEY_WOW64_32KEY) == REG_BINARY, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", KEY_WOW64_32KEY) == REG_QWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", KEY_WOW64_32KEY) == REG_DWORD, ERROR_PROPERTYTYPE);
    assert(winreg::getPropertyType(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", KEY_WOW64_32KEY) == REG_DWORD_BIG_ENDIAN, ERROR_PROPERTYTYPE);

    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\p\\oiu"), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\p\\oiu"), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\p\\oiu"), ERROR_CREATEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\po\\iu", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\po\\iu", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\po\\iu", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\poi\\u", KEY_WOW64_32KEY), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\poi\\u", KEY_WOW64_32KEY), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\poi\\u", KEY_WOW64_32KEY), ERROR_CREATEKEY);

    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwe\\rty"), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwe\\rty"), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwe\\rty"), ERROR_CREATEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwer\\ty", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwer\\ty", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwer\\ty", KEY_WOW64_64KEY), ERROR_CREATEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwert\\y", KEY_WOW64_32KEY), ERROR_CREATEKEY);
    assert(winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwert\\y", KEY_WOW64_32KEY), ERROR_CREATEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest\\qwert\\y", KEY_WOW64_32KEY), ERROR_CREATEKEY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testEnumeration()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto wvec = winreg::enumerateProperties(HKEY_LOCAL_MACHINE, L"Software\\winregtest", KEY_WOW64_64KEY);

    assert(wvec.size() == 13, ERROR_CREATEKEY);
    auto wit = std::find(wvec.begin(), wvec.end(), L"");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test string");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test multistring");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test expand string");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test binary");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test qword");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test dword");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test dwordbe");
    assert(wit != wvec.end(), ERROR_ENUMERATEPROPERTIES);
    wit = std::find(wvec.begin(), wvec.end(), L"test missing");
    assert(wit == wvec.end(), ERROR_ENUMERATEPROPERTIES);

    auto vec = winreg::enumerateProperties(HKEY_LOCAL_MACHINE, "Software\\winregtest", KEY_WOW64_32KEY);

    assert(vec.size() == 8, ERROR_CREATEKEY);
    auto it = std::find(vec.begin(), vec.end(), "");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test string");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test multistring");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test expand string");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test binary");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test qword");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test dword");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test dwordbe");
    assert(it != vec.end(), ERROR_ENUMERATEPROPERTIES);
    it = std::find(vec.begin(), vec.end(), "test missing");
    assert(it == vec.end(), ERROR_ENUMERATEPROPERTIES);

    wvec = winreg::enumerateSubkeys(HKEY_LOCAL_MACHINE, L"Software\\winregtest");
    assert(wvec.size() == 0, ERROR_ENUMERATEKEYS);
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\q\\werty");
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\qw\\erty");
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\qwe\\rty");
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\qwer\\ty");
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\qwert\\y");
    winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\qwerty");

    wvec = winreg::enumerateSubkeys(HKEY_LOCAL_MACHINE, L"Software\\winregtest");
    assert(wvec.size() == 6, ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"q");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"qw");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"qwe");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"qwer");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"qwert");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"qwerty");
    assert(wit != wvec.end(), ERROR_ENUMERATEKEYS);
    wit = std::find(wvec.begin(), wvec.end(), L"test missing");
    assert(wit == wvec.end(), ERROR_ENUMERATEKEYS);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);

    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    vec = winreg::enumerateSubkeys(HKEY_LOCAL_MACHINE, "Software\\winregtest");
    assert(vec.size() == 0, ERROR_ENUMERATEKEYS);
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\p\\oiu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\po\\iu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poi\\u");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poiu");

    vec = winreg::enumerateSubkeys(HKEY_LOCAL_MACHINE, "Software\\winregtest");
    assert(vec.size() == 4, ERROR_ENUMERATEKEYS);
    it = std::find(vec.begin(), vec.end(), "p");
    assert(it != vec.end(), ERROR_ENUMERATEKEYS);
    it = std::find(vec.begin(), vec.end(), "po");
    assert(it != vec.end(), ERROR_ENUMERATEKEYS);
    it = std::find(vec.begin(), vec.end(), "poi");
    assert(it != vec.end(), ERROR_ENUMERATEKEYS);
    it = std::find(vec.begin(), vec.end(), "poiu");
    assert(it != vec.end(), ERROR_ENUMERATEKEYS);
    it = std::find(vec.begin(), vec.end(), "test missing");
    assert(it == vec.end(), ERROR_ENUMERATEKEYS);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testDelete()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string"), ERROR_DELETEPROPERTY);
    assert(winreg::deleteProperty(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string"), ERROR_DELETEPROPERTY);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test string"), ERROR_DELETEPROPERTY);
    assert(winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword"), ERROR_DELETEPROPERTY);
    assert(winreg::deleteProperty(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword"), ERROR_DELETEPROPERTY);
    assert(!winreg::propertyExists(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword"), ERROR_DELETEPROPERTY);

    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);
    assert(winreg::deleteKey(HKEY_LOCAL_MACHINE, L"Software", L"winregtest"), ERROR_DELETEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);

    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\p\\oiu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\po\\iu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poi\\u");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poiu");

    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);
    assert(!winreg::deleteKey(HKEY_LOCAL_MACHINE, L"Software", L"winregtest", false), ERROR_DELETEKEY);
    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);
    assert(winreg::deleteKey(HKEY_LOCAL_MACHINE, L"Software", L"winregtest", true), ERROR_DELETEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);

    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\p\\oiu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\po\\iu");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poi\\u");
    winreg::createKey(HKEY_LOCAL_MACHINE, "Software\\winregtest\\poiu");

    assert(winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);
    assert(winreg::killKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);
    assert(!winreg::keyExists(HKEY_LOCAL_MACHINE, L"Software\\winregtest"), ERROR_DELETEKEY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetString()
{
	runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed");
    assert(wr == L"64 qwerty äçÇàõñ", ERROR_GETSTRING);

    wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed", KEY_WOW64_64KEY);
    assert(wr == L"64 qwerty äçÇàõñ", ERROR_GETSTRING);

    wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed", KEY_WOW64_32KEY);
    assert(wr == L"32 qwerty äçÇàõñ", ERROR_GETSTRING);

    auto r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed");
    assert(wstring_from_utf8(r) == L"64 qwerty äçÇàõñ", ERROR_GETSTRING);

    r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed", KEY_WOW64_64KEY);
    assert(wstring_from_utf8(r) == L"64 qwerty äçÇàõñ", ERROR_GETSTRING);

    r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed", KEY_WOW64_32KEY);
    assert(wstring_from_utf8(r) == L"32 qwerty äçÇàõñ", ERROR_GETSTRING);
	
    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testSetString()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L""), ERROR_SETSTRING);
    auto wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed");
    assert(wr == L"", ERROR_SETSTRING);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"abç", KEY_WOW64_64KEY), ERROR_SETSTRING);
    wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed", KEY_WOW64_64KEY);
    assert(wr == L"abç", ERROR_SETSTRING);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L",.;Ç", KEY_WOW64_32KEY), ERROR_SETSTRING);
    wr = winreg::getString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test string", L"failed", KEY_WOW64_32KEY);
    assert(wr == L",.;Ç", ERROR_SETSTRING);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", utf8_from_wstring(L"ñÕõÑ")), ERROR_SETSTRING);
    auto r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed");
    assert(wstring_from_utf8(r) == L"ñÕõÑ", ERROR_SETSTRING);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", utf8_from_wstring(L"abû"), KEY_WOW64_64KEY), ERROR_SETSTRING);
    r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed", KEY_WOW64_64KEY);
    assert(wstring_from_utf8(r) == L"abû", ERROR_SETSTRING);

    assert(winreg::setString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", utf8_from_wstring(L",.;§"), KEY_WOW64_32KEY), ERROR_SETSTRING);
    r = winreg::getString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test string", "failed", KEY_WOW64_32KEY);
    assert(wstring_from_utf8(r) == L",.;§", ERROR_SETSTRING);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetExpandString()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto tempDir = getEnvVar(L"temp");
    auto appDataDir = getEnvVar(L"appdata");

    auto wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test expand string", L"failed");
    assert(wr == appDataDir, ERROR_GETEXPSTRING);

    wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"failed", KEY_WOW64_64KEY);
    assert(wr == appDataDir, ERROR_GETEXPSTRING);

    wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"failed", KEY_WOW64_32KEY);
    assert(wr == tempDir, ERROR_GETEXPSTRING);

    auto r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed");
    assert(wstring_from_utf8(r) == appDataDir, ERROR_GETEXPSTRING);

    r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed", KEY_WOW64_64KEY);
    assert(wstring_from_utf8(r) == appDataDir, ERROR_GETEXPSTRING);

    r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed", KEY_WOW64_32KEY);
    assert(wstring_from_utf8(r) == tempDir, ERROR_GETEXPSTRING);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testSetExpandString()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto tempDir = getEnvVar(L"temp");
    auto appDataDir = getEnvVar(L"appdata");

    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"64: %temp%"), ERROR_SETEXPSTRING);
    auto wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"failed");
    assert(wr == L"64: " + tempDir, ERROR_SETEXPSTRING);

    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"64: %temp%", KEY_WOW64_64KEY), ERROR_SETEXPSTRING);
    wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"failed", KEY_WOW64_64KEY);
    assert(wr == L"64: " + tempDir, ERROR_SETEXPSTRING);

    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"32: %appdata%", KEY_WOW64_32KEY), ERROR_SETEXPSTRING);
    wr = winreg::getExpandString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"Test expand string", L"failed", KEY_WOW64_32KEY);
    assert(wr == L"32: " + appDataDir, ERROR_SETEXPSTRING);


    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", utf8_from_wstring(L"64: %appdata%")), ERROR_SETEXPSTRING);
    auto r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed");
    assert(wstring_from_utf8(r) == L"64: " + appDataDir, ERROR_SETEXPSTRING);

    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", utf8_from_wstring(L"64: %appdata%"), KEY_WOW64_64KEY), ERROR_SETEXPSTRING);
    r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed", KEY_WOW64_64KEY);
    assert(wstring_from_utf8(r) == L"64: " + appDataDir, ERROR_SETEXPSTRING);

    assert(winreg::setExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", utf8_from_wstring(L"32: %temp%"), KEY_WOW64_32KEY), ERROR_SETEXPSTRING);
    r = winreg::getExpandString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "Test expand string", "failed", KEY_WOW64_32KEY);
    assert(wstring_from_utf8(r) == L"32: " + tempDir, ERROR_SETEXPSTRING);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetMultiString()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    vector<wstring> wval;

    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", wval), ERROR_GETMULTISTRING);
    assert(wval.size() == 5, ERROR_GETMULTISTRING);
    assert(wval[0] == L"64", ERROR_GETMULTISTRING);
    assert(wval[1] == L"s1", ERROR_GETMULTISTRING);
    assert(wval[2] == L"s2", ERROR_GETMULTISTRING);
    assert(wval[3] == L"s 3", ERROR_GETMULTISTRING);
    assert(wval[4] == L"s4", ERROR_GETMULTISTRING);

    wval.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", wval, KEY_WOW64_64KEY), ERROR_GETMULTISTRING);
    assert(wval.size() == 5, ERROR_GETMULTISTRING);
    assert(wval[0] == L"64", ERROR_GETMULTISTRING);
    assert(wval[1] == L"s1", ERROR_GETMULTISTRING);
    assert(wval[2] == L"s2", ERROR_GETMULTISTRING);
    assert(wval[3] == L"s 3", ERROR_GETMULTISTRING);
    assert(wval[4] == L"s4", ERROR_GETMULTISTRING);

    wval.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", wval, KEY_WOW64_32KEY), ERROR_GETMULTISTRING);
    assert(wval.size() == 5, ERROR_GETMULTISTRING);
    assert(wval[0] == L"32", ERROR_GETMULTISTRING);
    assert(wval[1] == L"s1", ERROR_GETMULTISTRING);
    assert(wval[2] == L"s2", ERROR_GETMULTISTRING);
    assert(wval[3] == L"s 3", ERROR_GETMULTISTRING);
    assert(wval[4] == L"s4", ERROR_GETMULTISTRING);

    vector<string> val;

    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", val), ERROR_GETMULTISTRING);
    assert(val.size() == 5, ERROR_GETMULTISTRING);
    assert(val[0] == "64", ERROR_GETMULTISTRING);
    assert(val[1] == "s1", ERROR_GETMULTISTRING);
    assert(val[2] == "s2", ERROR_GETMULTISTRING);
    assert(val[3] == "s 3", ERROR_GETMULTISTRING);
    assert(val[4] == "s4", ERROR_GETMULTISTRING);

    val.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", val, KEY_WOW64_64KEY), ERROR_GETMULTISTRING);
    assert(val.size() == 5, ERROR_GETMULTISTRING);
    assert(val[0] == "64", ERROR_GETMULTISTRING);
    assert(val[1] == "s1", ERROR_GETMULTISTRING);
    assert(val[2] == "s2", ERROR_GETMULTISTRING);
    assert(val[3] == "s 3", ERROR_GETMULTISTRING);
    assert(val[4] == "s4", ERROR_GETMULTISTRING);

    val.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", val, KEY_WOW64_32KEY), ERROR_GETMULTISTRING);
    assert(val.size() == 5, ERROR_GETMULTISTRING);
    assert(val[0] == "32", ERROR_GETMULTISTRING);
    assert(val[1] == "s1", ERROR_GETMULTISTRING);
    assert(val[2] == "s2", ERROR_GETMULTISTRING);
    assert(val[3] == "s 3", ERROR_GETMULTISTRING);
    assert(val[4] == "s4", ERROR_GETMULTISTRING);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testSetMultiString()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    vector<wstring> wval;
    wval.push_back(L"");
    wval.push_back(L"¢all0º");
    wval.push_back(L"");
    wval.push_back(L"çãmò");
    wval.push_back(L"¹²³");
    wval.push_back(L"abcdefghijklmnopqrstuvwxYZ");

    assert(winreg::setMultiString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", wval), ERROR_SETMULTISTRING);

    wval.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test multistring", wval), ERROR_SETMULTISTRING);
    assert(wval.size() == 4, ERROR_SETMULTISTRING);
    assert(wval[0] == L"¢all0º", ERROR_SETMULTISTRING);
    assert(wval[1] == L"çãmò", ERROR_SETMULTISTRING);
    assert(wval[2] == L"¹²³", ERROR_SETMULTISTRING);
    assert(wval[3] == L"abcdefghijklmnopqrstuvwxYZ", ERROR_SETMULTISTRING);

    vector<string> val;
    val.push_back("");
    val.push_back(utf8_from_wstring(L"¢l0º"));
    val.push_back("");
    val.push_back(utf8_from_wstring(L"çãò"));
    val.push_back(utf8_from_wstring(L"¹²³"));
    val.push_back("");

    assert(winreg::setMultiString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", val), ERROR_SETMULTISTRING);

    val.clear();
    assert(winreg::getMultiString(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test multistring", val), ERROR_SETMULTISTRING);
    assert(val.size() == 3, ERROR_SETMULTISTRING);
    assert(wstring_from_utf8(val[0]) == L"¢l0º", ERROR_SETMULTISTRING);
    assert(wstring_from_utf8(val[1]) == L"çãò", ERROR_SETMULTISTRING);
    assert(wstring_from_utf8(val[2]) == L"¹²³", ERROR_SETMULTISTRING);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetDWORD()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0);
    assert(dw == 0x64fe876b, ERROR_GETDWORD);

    dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0, KEY_WOW64_64KEY);
    assert(dw == 0x64fe876b, ERROR_GETDWORD);

    dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0, KEY_WOW64_32KEY);
    assert(dw == 0x32fe876b, ERROR_GETDWORD);

    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0);
    assert(dw == 0x64fe876b, ERROR_GETDWORD);

    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0, KEY_WOW64_64KEY);
    assert(dw == 0x64fe876b, ERROR_GETDWORD);

    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0, KEY_WOW64_32KEY);
    assert(dw == 0x32fe876b, ERROR_GETDWORD);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}
void testSetDWORD()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::setDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 123), ERROR_SETDWORD);
    auto dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0);
    assert(dw == 123, ERROR_SETDWORD);
    
    assert(winreg::setDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 124, KEY_WOW64_64KEY), ERROR_SETDWORD);
    dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0, KEY_WOW64_64KEY);
    assert(dw == 124, ERROR_SETDWORD);

    assert(winreg::setDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 125, KEY_WOW64_32KEY), ERROR_SETDWORD);
    dw = winreg::getDword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dword", 0, KEY_WOW64_32KEY);
    assert(dw == 125, ERROR_SETDWORD);

    assert(winreg::setDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 123), ERROR_SETDWORD);
    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0);
    assert(dw == 123, ERROR_SETDWORD);

    assert(winreg::setDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 124, KEY_WOW64_64KEY), ERROR_SETDWORD);
    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0, KEY_WOW64_64KEY);
    assert(dw == 124, ERROR_SETDWORD);

    assert(winreg::setDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 125, KEY_WOW64_32KEY), ERROR_SETDWORD);
    dw = winreg::getDword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dword", 0, KEY_WOW64_32KEY);
    assert(dw == 125, ERROR_SETDWORD);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetDWORDBE()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0);
    assert(dw == 222, ERROR_GETDWORDBE);

    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0, KEY_WOW64_64KEY);
    assert(dw == 222, ERROR_GETDWORDBE);

    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0, KEY_WOW64_32KEY);
    assert(dw == 2222, ERROR_GETDWORDBE);

    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0);
    assert(dw == 222, ERROR_GETDWORDBE);

    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0, KEY_WOW64_64KEY);
    assert(dw == 222, ERROR_GETDWORDBE);

    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0, KEY_WOW64_32KEY);
    assert(dw == 2222, ERROR_GETDWORDBE);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}
void testSetDWORDBE()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 333), ERROR_SETDWORDBE);
    auto dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0);
    assert(dw == 333, ERROR_SETDWORDBE);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 444, KEY_WOW64_64KEY), ERROR_SETDWORDBE);
    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0, KEY_WOW64_64KEY);
    assert(dw == 444, ERROR_SETDWORDBE);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 555, KEY_WOW64_32KEY), ERROR_SETDWORDBE);
    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test dwordbe", 0, KEY_WOW64_32KEY);
    assert(dw == 555, ERROR_SETDWORDBE);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 666), ERROR_SETDWORDBE);
    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0);
    assert(dw == 666, ERROR_SETDWORDBE);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 777, KEY_WOW64_64KEY), ERROR_SETDWORDBE);
    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0, KEY_WOW64_64KEY);
    assert(dw == 777, ERROR_SETDWORDBE);

    assert(winreg::setDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 888, KEY_WOW64_32KEY), ERROR_SETDWORDBE);
    dw = winreg::getDwordBE(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test dwordbe", 0, KEY_WOW64_32KEY);
    assert(dw == 888, ERROR_SETDWORDBE);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetQWORD()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0);
    assert(dw == 0x6445069fd4d32c, ERROR_GETQWORD);

    dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0, KEY_WOW64_64KEY);
    assert(dw == 0x6445069fd4d32c, ERROR_GETQWORD);

    dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0, KEY_WOW64_32KEY);
    assert(dw == 0x3245069fd4d32c, ERROR_GETQWORD);

    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0);
    assert(dw == 0x6445069fd4d32c, ERROR_GETQWORD);

    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0, KEY_WOW64_64KEY);
    assert(dw == 0x6445069fd4d32c, ERROR_GETQWORD);

    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0, KEY_WOW64_32KEY);
    assert(dw == 0x3245069fd4d32c, ERROR_GETQWORD);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}
void testSetQWORD()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0xF12ED34CB5), ERROR_SETQWORD);
    auto dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0);
    assert(dw == 0xF12ED34CB5, ERROR_SETQWORD);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0xF12EDF34CB5, KEY_WOW64_64KEY), ERROR_SETQWORD);
    dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0, KEY_WOW64_64KEY);
    assert(dw == 0xF12EDF34CB5, ERROR_SETQWORD);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0xF12EDF34CB57, KEY_WOW64_32KEY), ERROR_SETQWORD);
    dw = winreg::getQword(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test qword", 0, KEY_WOW64_32KEY);
    assert(dw == 0xF12EDF34CB57, ERROR_SETQWORD);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0xF12EDF34CB57A), ERROR_SETQWORD);
    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0);
    assert(dw == 0xF12EDF34CB57A, ERROR_SETQWORD);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0xF12EDF34CB57A8, KEY_WOW64_64KEY), ERROR_SETQWORD);
    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0, KEY_WOW64_64KEY);
    assert(dw == 0xF12EDF34CB57A8, ERROR_SETQWORD);

    assert(winreg::setQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0xF12EDF34CB57A80, KEY_WOW64_32KEY), ERROR_SETQWORD);
    dw = winreg::getQword(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test qword", 0, KEY_WOW64_32KEY);
    assert(dw == 0xF12EDF34CB57A80, ERROR_SETQWORD);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testGetBinary()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    auto s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "");
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "", KEY_WOW64_64KEY);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "", KEY_WOW64_32KEY);
    assert(s == "MgEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "");
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "", KEY_WOW64_64KEY);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "", KEY_WOW64_32KEY);
    assert(s == "MgEjRWeJq83v//7cuph2VDIQ", ERROR_GETBINARY);

    string binary;
    unsigned char expected[] = { 0x64, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xff, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
    assert(winreg::getBinary(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", binary, KEY_WOW64_64KEY), ERROR_GETBINARY);
    assert(binary.length() == 18, ERROR_GETBINARY);
    for (int i = 0; i < binary.length(); ++i) assert((char)expected[i] == binary[i], ERROR_GETBINARY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testSetBinary()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "37hH/Q=="), ERROR_SETBINARY);
    auto s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "");
    assert(s == "37hH/Q==", ERROR_SETBINARY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "3gWYed52Q5hHUJhTaZ3QNA==", KEY_WOW64_64KEY), ERROR_SETBINARY);
    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "", KEY_WOW64_64KEY);
    assert(s == "3gWYed52Q5hHUJhTaZ3QNA==", ERROR_SETBINARY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "n0j8OYyP79+DcJs=", KEY_WOW64_32KEY), ERROR_SETBINARY);
    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test binary", "", KEY_WOW64_32KEY);
    assert(s == "n0j8OYyP79+DcJs=", ERROR_SETBINARY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "3eBZh53nZDk="), ERROR_SETBINARY);
    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "");
    assert(s == "3eBZh53nZDk=", ERROR_SETBINARY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "7vyJ80+vhzT/37o/pEh2Tv39/oA=", KEY_WOW64_64KEY), ERROR_SETBINARY);
    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "", KEY_WOW64_64KEY);
    assert(s == "7vyJ80+vhzT/37o/pEh2Tv39/oA=", ERROR_SETBINARY);

    assert(winreg::setBinaryFromBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "j964T9/Y7fzY7bmI7t36", KEY_WOW64_32KEY), ERROR_SETBINARY);
    s = winreg::getBinaryAsBase64(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", "", KEY_WOW64_32KEY);
    assert(s == "j964T9/Y7fzY7bmI7t36", ERROR_SETBINARY);

    string binary, binary2;
    unsigned char data[] = { 0x64, 0x01, 0x23, 0x00, 0xab, 0xcd, 0xef, 0xff, 0x00, 0xdc, 0x10 };
    for (int i = 0; i < 11; ++i) binary.push_back(data[i]);
    assert(binary.length() == 11, ERROR_SETBINARY);
    assert(winreg::setBinary(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", binary.c_str(), binary.length(), KEY_WOW64_64KEY), ERROR_SETBINARY);
    assert(winreg::getBinary(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test binary", binary2, KEY_WOW64_64KEY), ERROR_SETBINARY);
    assert(binary2.length() == 11, ERROR_SETBINARY);
    for (int i = 0; i < binary2.length(); ++i) assert((char)data[i] == binary2[i], ERROR_SETBINARY);
    assert(binary == binary2, ERROR_SETBINARY);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}



void testGetOther()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);
    unsigned long type;
    auto s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test reg_none", "", type, KEY_WOW64_64KEY);
    assert(type == REG_NONE, ERROR_GETOTHER);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETOTHER);

    s.clear();
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test reg_link", "", type, KEY_WOW64_64KEY);
    assert(type == REG_LINK, ERROR_GETOTHER);
    assert(s == "ZAAjAFQyEA==", ERROR_GETOTHER);

    s.clear();
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_LIST", "", type, KEY_WOW64_64KEY);
    assert(type == REG_RESOURCE_LIST, ERROR_GETOTHER);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETOTHER);

    s.clear();
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_FULL_RESOURCE_DESCRIPTOR", "", type, KEY_WOW64_64KEY);
    assert(type == REG_FULL_RESOURCE_DESCRIPTOR, ERROR_GETOTHER);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETOTHER);

    s.clear();
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_REQUIREMENTS_LIST", "", type, KEY_WOW64_64KEY);
    assert(type == REG_RESOURCE_REQUIREMENTS_LIST, ERROR_GETOTHER);
    assert(s == "ZAEjRWeJq83v//7cuph2VDIQ", ERROR_GETOTHER);

    s.clear();
    unsigned char expected[] = { 0x64, 0x00, 0x23, 0x00, 0x54, 0x32, 0x10 };
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, "Software\\winregtest", "test reg_link", s, type, KEY_WOW64_64KEY), ERROR_GETOTHER);
    assert(type == REG_LINK, ERROR_GETOTHER);
    assert(s.length() == 7, ERROR_GETOTHER);
    for (int i = 0; i < s.length(); ++i) assert((char)expected[i] == s[i], ERROR_GETOTHER);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testSetOther()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    string s;
    unsigned long type;
    unsigned char expected[] = { 0xdf, 0xb8, 0x47, 0xfd };
    string sexpected((char*)expected, 4);
    assert(winreg::setByteArrayFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_FULL_RESOURCE_DESCRIPTOR", "37hH/Q==", REG_FULL_RESOURCE_DESCRIPTOR), ERROR_SETOTHER);
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_FULL_RESOURCE_DESCRIPTOR", s, type), ERROR_SETOTHER);
    assert(type == REG_FULL_RESOURCE_DESCRIPTOR, ERROR_SETOTHER);
    assert(s.length() == 4, ERROR_SETOTHER);
    assert(s == sexpected, ERROR_SETOTHER);
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_FULL_RESOURCE_DESCRIPTOR", "", type);
    assert(s == "37hH/Q==", ERROR_SETOTHER);

    s.clear();
    assert(winreg::setByteArrayFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_REQUIREMENTS_LIST", "37hH/Q==", REG_RESOURCE_REQUIREMENTS_LIST), ERROR_SETOTHER);
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_REQUIREMENTS_LIST", s, type), ERROR_SETOTHER);
    assert(type == REG_RESOURCE_REQUIREMENTS_LIST, ERROR_SETOTHER);
    assert(s.length() == 4, ERROR_SETOTHER);
    assert(s == sexpected, ERROR_SETOTHER);
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_REQUIREMENTS_LIST", "", type);
    assert(s == "37hH/Q==", ERROR_SETOTHER);

    s.clear();
    assert(winreg::setByteArrayFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_LINK", "37hH/Q==", REG_LINK), ERROR_SETOTHER);
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_LINK", s, type), ERROR_SETOTHER);
    assert(type == REG_LINK, ERROR_SETOTHER);
    assert(s.length() == 4, ERROR_SETOTHER);
    assert(s == sexpected, ERROR_SETOTHER);
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_LINK", "", type);
    assert(s == "37hH/Q==", ERROR_SETOTHER);

    s.clear();
    assert(winreg::setByteArrayFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_NONE", "37hH/Q==", REG_NONE), ERROR_SETOTHER);
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_NONE", s, type), ERROR_SETOTHER);
    assert(type == REG_NONE, ERROR_SETOTHER);
    assert(s.length() == 4, ERROR_SETOTHER);
    assert(s == sexpected, ERROR_SETOTHER);
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_NONE", "", type);
    assert(s == "37hH/Q==", ERROR_SETOTHER);

    s.clear();
    assert(winreg::setByteArrayFromBase64(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_LIST", "37hH/Q==", REG_RESOURCE_LIST), ERROR_SETOTHER);
    assert(winreg::getAsByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_LIST", s, type), ERROR_SETOTHER);
    assert(type == REG_RESOURCE_LIST, ERROR_SETOTHER);
    assert(s.length() == 4, ERROR_SETOTHER);
    assert(s == sexpected, ERROR_SETOTHER);
    s = winreg::getAsBase64ByteArray(HKEY_LOCAL_MACHINE, L"Software\\winregtest", L"test REG_RESOURCE_LIST", "", type);
    assert(s == "37hH/Q==", ERROR_SETOTHER);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

void testCopyTree()
{
    runsystem("reg import addtest.reg", ERROR_IMPORT_TESTREGISTRY);

    assert(winreg::createKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest\\copy", KEY_WOW64_32KEY), ERROR_COPYTREE);
    assert(winreg::copyKey(HKEY_LOCAL_MACHINE, L"Software\\winregtest", HKEY_LOCAL_MACHINE, L"Software\\winregtest\\copy", KEY_WOW64_64KEY, KEY_WOW64_32KEY), ERROR_COPYTREE);

    runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
}

int testAll()
{
    int ret = 0;
    try
    {
        testRegPath();

        testExistance();

        testEnumeration();

        testDelete();

        testCopyTree();

        testGetString();
        testSetString();

        testGetExpandString();
        testSetExpandString();

        testGetMultiString();
        testSetMultiString();

        testGetDWORD();
        testSetDWORD();

        testGetDWORDBE();
        testSetDWORDBE();

        testGetQWORD();
        testSetQWORD();

        testGetBinary();
        testSetBinary();

        testGetOther();
        testSetOther();
    }
    catch (int e)
    {
        cout << "caught an integer error: " << e << endl;
        ret = e;
    }
    catch (exception& ex)
    {
        cout << "caught exception: " << ex.what() << endl;
        ret = -1;
    }
    catch (...)
    {
        cout << "unknown error" << endl;
        ret = -2;
    }

    try
    {
        runsystem("reg import deltest.reg", ERROR_REMOVE_TESTREGISTRY);
    }
    catch (...) {}
    
    return ret;
}
