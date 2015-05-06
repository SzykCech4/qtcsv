#include "include/writer.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "include/abstractdata.h"
#include "filechecker.h"

using namespace QtCSV;

// Write data to .csv file
// @input:
// - filePath - string with absolute path to csv file
// - data - not empty data that should be written to .csv file
// - separator - string or character separating columns
// - mode - write mode of the file: rewrite file or append data to the end
// - header - strings that will be written at the beginning of the file in
// one line. separator will be used as delimiter character.
// - footer - strings that will be written at the end of the file in
// one line. separator will be used as delimiter character.
// @output:
// - bool - True if data was written to the file, otherwise False
bool Writer::write(const QString &filePath,
                   const AbstractData &data,
                   const QString &separator,
                   const WriteMode &mode,
                   const QStringList &header,
                   const QStringList &footer)
{
    if ( true == filePath.isEmpty() ||
         true == data.isEmpty() )
    {
        qDebug() << __func__ << "Error - invalid arguments";
        return false;
    }

    if ( false == CheckFile(filePath) )
    {
        qDebug() << __func__ << "Error - wrong file path/name:" << filePath;
        return false;
    }

    // Prepare data that would be written to file
    QStringList textLines;
    if ( false == header.isEmpty() )
    {
        textLines << header.join(separator);
    }

    const int rowsNum = data.getNumberOfRows();
    for (int i = 0; i < rowsNum; ++i)
    {
        textLines << data.getRowValues(i).join(separator);
    }

    if ( false == footer.isEmpty() )
    {
        textLines << footer.join(separator);
    }

    // Write prepaired data to file
    QFile csvFile(filePath);
    if ( false == csvFile.open(GetMode(mode) | QIODevice::Text) )
    {
        qDebug() << __func__ << "Error - can't open file:" << filePath;
        return false;
    }

    QTextStream stream;
    stream.setDevice(&csvFile);
    stream << textLines.join("\n");
    stream.flush();

    csvFile.close();

    return true;
}

// Get QIODevice mode
// @input:
// - mode - write mode
// @output:
// - QIODevice::OpenMode - corresponding QIODevice::OpenMode
QIODevice::OpenMode Writer::GetMode(const WriteMode &mode)
{
    switch (mode)
    {
        case WriteMode::APPEND:
            return QIODevice::Append;
        case WriteMode::REWRITE:
        default:
            return QIODevice::WriteOnly;
    }

    return QIODevice::WriteOnly;
}