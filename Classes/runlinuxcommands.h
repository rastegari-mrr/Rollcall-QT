#ifndef RUNLINUXCOMMANDS_H
#define RUNLINUXCOMMANDS_H
#include <QObject>
struct RunLinuxCommandsReturn {
    QString error;
    QString output;
};

class RunLinuxCommands
{
public:
    RunLinuxCommands();

    static RunLinuxCommandsReturn run(const QString &command, const QStringList & args);
};

#endif // RUNLINUXCOMMANDS_H
