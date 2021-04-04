#include "runlinuxcommands.h"
#include <QProcess>
RunLinuxCommands::RunLinuxCommands()
{

}

RunLinuxCommandsReturn RunLinuxCommands::run(const QString &command, const QStringList &args)
{
    QProcess process;
    process.start(command, args, QIODevice::ReadOnly);
    process.waitForFinished();
    QString out = process.readAllStandardOutput();
    QString error = process.readAllStandardError();
    RunLinuxCommandsReturn ret;
    ret.error = error;
    ret.output = out;
    return ret;
}
