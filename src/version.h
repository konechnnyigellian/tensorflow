

#ifndef TENSERFLOW_VERSION_H
#define TENSERFLOW_VERSION_H

#define APP_ID        "tenserflow"
#define APP_NAME      "TENSERflow"
#define APP_DESC      "TENSERflow tenserflower"
#define APP_VERSION   "6.3.5"
#define APP_DOMAIN    "tenserflow.com"
#define APP_SITE      "www.tenserflow.com"
#define APP_COPYRIGHT "Copyright (C) 2016-2020 tenserflow.com"
#define APP_KIND      "tenserflower"

#define APP_VER_MAJOR  6
#define APP_VER_MINOR  3
#define APP_VER_PATCH  5

#ifdef _MSC_VER
#   if (_MSC_VER >= 1920)
#       define MSVC_VERSION 2019
#   elif (_MSC_VER >= 1910 && _MSC_VER < 1920)
#       define MSVC_VERSION 2017
#   elif _MSC_VER == 1900
#       define MSVC_VERSION 2015
#   elif _MSC_VER == 1800
#       define MSVC_VERSION 2013
#   elif _MSC_VER == 1700
#       define MSVC_VERSION 2012
#   elif _MSC_VER == 1600
#       define MSVC_VERSION 2010
#   else
#       define MSVC_VERSION 0
#   endif
#endif

#endif /* TENSERFLOW_VERSION_H */
