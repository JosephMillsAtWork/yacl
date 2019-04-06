#include <QDebug>
#include <QMetaType>

#ifndef ERROR_H
#define ERROR_H
enum class Error {
    None = 0,
    Canceled = 1,
    Parse = 2,

    System = 10,
    IO = 11,
    Permissions = 12,
    Network = 13,
    StorageSpace = 14,
    DBus = 15,

    Cryptography = 20,

    Archive = 30,

    Package = 40,
    Locked = 41,
    NotInstalled = 42,
    AlreadyInstalled = 43,

    MediumNotAvailable = 50,
    WrongMedium = 51,
};
Q_DECLARE_METATYPE(Error)
inline QDebug &operator<<(QDebug &debug, Error error)
{
    return debug << int(error);
}
#endif
