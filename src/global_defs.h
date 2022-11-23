#ifndef GLOBAL_DEFS_H
#define GLOBAL_DEFS_H

#include <QObject>
#include <QDebug>

namespace ResultCode
{
    const int UNDEF           = -1;
    const int ALL_RIGHT       = 0;
    const int EXEC_ERROR      = 1;
    const int FILE_NOT_FOUND  = 2;
    const int INIT_ERROR      = 3;
    const int DB_OPEN_ERROR   = 4;
    const int DB_QUERY_ERROR  = 5;
    const int OPEN_FILE_ERROR = 6;
    const int UNIT_NOT_FOUND  = 7;
}

enum class MsgType : quint8
{
    UNDEF   = 0,
    INFO    = 1,
    ERROR   = 2,
    WARNING = 3,
    DEBUG   = 4,
};

enum class OpType : int
{
    CREATE     = 0,
    CHANGE     = 1,
    DELETE     = 2,
    APPEND     = 3,
    INSERT     = 4,
    DELETE_ROW = 5,
};

enum class PropertyType : int
{
    UNDEF  = 0,
    TYPE_1 = 1,
    TYPE_2 = 2,
    TYPE_3 = 3,
    TYPE_4 = 4,
    TYPE_5 = 5,
};

#endif // GLOBAL_DEFS_H
