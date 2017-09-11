#ifndef IVIMANIFEST_H
#define IVIMANIFEST_H

#include <QtXml/QXmlDefaultHandler>

#include <IVI-Launcher/ivilauncherglobal.h>

QT_BEGIN_NAMESPACE

class QXmlParseException;

class Q_IVI_LAUNCHER_EXPORT IVIManifest : public QXmlDefaultHandler
{
public:
    IVIManifest(const QString& filename);

    enum ExceptionType
    {
        Warning,
        Error,
        FatalError
    };

    bool isSuccesful() const { return succes; }
    const QList<QPair<ExceptionType, QXmlParseException>>& getExceptionList() const { return exceptions; }

    const QString& getName() const {return name;}
    const QString& getDescription() const {return description;}
    const QString& getIcon() const {return icon;}
    const QString& getWebsite() const {return website;}
    const QList<QString>& getCategories() const {return categories;}
    const QString& getExecutable() const {return executable;}
    const QList<QString>& getArguments() const {return arguments;}

private:
    bool succes;
    enum ParseState
    {
        RootState,
        PackageState,
        NameState,
        DescriptionState,
        IconState,
        AuthorsState,
        AuthorState,
        AuthorNameState,
        AuthorEmailState,
        AuthorCompanyState,
        WebsiteState,
        CategoriesState,
        CategoryState,
        RunState,
        ExecutableState,
        ArgumentState,
    } state;

    QString name;
    QString description;
    QString icon;
    QString website;
    QList<QString> categories;
    QString executable;
    QList<QString> arguments;

    QList<QPair<ExceptionType, QXmlParseException>> exceptions;

    bool warning(const QXmlParseException& exception);
    bool error(const QXmlParseException& exception);
    bool fatalError(const QXmlParseException& exception);
    bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&);
    bool characters(const QString&);
    bool endElement(const QString&, const QString&, const QString&);
};

QT_END_NAMESPACE

#endif // IVIMANIFEST_H
