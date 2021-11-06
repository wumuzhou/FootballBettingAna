//
// Created by wmz on 2018/9/17.
//

#ifndef THONLINEEPANET_THLOG22_H
#define THONLINEEPANET_THLOG22_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/core/record.hpp>
#include <boost/log/attributes/value_visitation.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
using namespace std;

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
using boost::shared_ptr;

enum severity_level {
    normal,
    notification,
    info,
    warning,
    error,
    critical
};


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger, src::severity_logger<>)

#define LOG(severity, type_string) BOOST_LOG_SEV(logger::get(),severity)<<"["<<type_string<<"] "

#define LOG_TRACE   LOG(normal,"TRACE")
#define LOG_DEBUG   LOG(notification, "DEBUG")
#define LOG_INFO   LOG(info, "INFO")
#define LOG_WARNING LOG(warning, "WARNING")
#define LOG_ERROR   LOG(error, "ERROR")
#define LOG_FATAL   LOG(critical, "FATAL")
#define LOG_I(severity) BOOST_LOG_SEV(logger::get(),severity)<<"[IMPORTANT] "
#define LOG_IMPORTANT   LOG_I(critical)

static int init_log_conf(const std::string &FilePath) {
    std::ifstream settings(FilePath);
    cout << "FilePath===>" << FilePath << endl;
    if (!settings.is_open()) {
        BOOST_LOG_SEV(logger::get(), 5) << "Could not open log settings.txt file";
        return 1;
    }
    // Read the settings and initialize logging library
    logging::init_from_stream(settings);
    // Add some attributes
    logging::add_common_attributes();
    return 0;
}

#endif //THONLINEEPANET_THLOG_H
