//
// Created by wmz on 9/5/18.
//

#ifndef TEMP3_CPP_FOR_REDIS_THUTILS_H
#define TEMP3_CPP_FOR_REDIS_THUTILS_H

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "boost/lexical_cast.hpp"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace Common {
    static inline int str2int(std::string des) {
        int i = 0;
        try {
            i = lexical_cast<int>(des);
        }
        catch (bad_lexical_cast &)
        {
            i = 0;
        }
        return i;
    }

    static inline double str2double(std::string des) {
        double i = 0;
        try {
            boost::algorithm::replace_all(des, "\r", "");
            i = lexical_cast<double>(des);
        }
        catch (bad_lexical_cast &)
        {
            i = 0.0;
            cout<<"bad_lexical_cast Error===>"<<des<<endl;
            throw ;
        }
        return i;
    }

    static inline string GetUuid() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        std::string UuidStr = boost::uuids::to_string(uuid);
        return UuidStr;
    }

    static inline void
    SplitString(const std::string &str, const std::string &delimiter, std::vector<std::string> &GetVec) {
        boost::algorithm::split(GetVec, str, boost::algorithm::is_any_of(delimiter));
    }

//    static inline std::string int2str(int v) {
//        std::stringstream ss;
//        ss << v;
//        return ss.str();
//    }

    static inline string doubleToString_2_decimal(double num) {
        char str[256];
//        sprintf(str, "%lf", num);
        sprintf(str, "%.2f", num);
        string result = str;
        return result;
    }

    static inline string doubleTostr(double des) {
        string res;
        try {
            res = lexical_cast<string>(des);
        }
        catch (bad_lexical_cast &)
        {
            res = "";
        }
        return res;
    }

    static inline std::string intTostr(int des) {
        std::string res;
        try {
            res = lexical_cast<std::string>(des);
        }
        catch (bad_lexical_cast &)
        {
            res = "";
        }
        return res;
    }
}
#endif //TEMP3_CPP_FOR_REDIS_THUTILS_H
