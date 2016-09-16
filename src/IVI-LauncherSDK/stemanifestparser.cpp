#include "stemanifestparser.h"

#include <QFile>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>

STEManifest::STEManifest(const QString& filename)
    : state(RootState)
{
    qDebug() << "Parsing filename:" << filename;

    QFile file(filename);
    file.open(QFile::ReadOnly);

    QXmlInputSource input(&file);
    QXmlSimpleReader reader;

    reader.setEntityResolver(this);
    reader.setDTDHandler(this);
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    reader.setLexicalHandler(this);
    reader.setDeclHandler(this);

    succes = reader.parse(input);

    file.close();
}

// ErrorHandler
bool STEManifest::warning(const QXmlParseException& exception)
{
    qDebug() << "[WARNING] line" << exception.lineNumber() << ":" << exception.message();
    return true;
}

bool STEManifest::error(const QXmlParseException& exception)
{
    qDebug() << "[ERROR] line" << exception.lineNumber() << ":" << exception.message();
    return true;
}

bool STEManifest::fatalError(const QXmlParseException& exception)
{
    qDebug() << "[FATAL] line" << exception.lineNumber() << ":" << exception.message();
    return true;
}

// ContentHandler
bool STEManifest::startElement(const QString& ns, const QString& localName, const QString& qName, const QXmlAttributes& atts)
{
    Q_UNUSED(ns)
    Q_UNUSED(localName)

    switch(state)
    {
    case RootState:
        {
            if(qName != "package")
            {
                qWarning() << "Root element was not a package element!";
                return false;
            }
            state = PackageState;
        }
        break;
    case PackageState:
        {
            if(qName == "name")
                state = NameState;
            else if(qName == "description")
                state = DescriptionState;
            else if(qName == "icon")
                state = IconState;
            else if(qName == "authors")
                state = AuthorsState;
            else if(qName == "website")
                state = WebsiteState;
            else if(qName == "categories")
                state = CategoriesState;
            else if(qName == "run")
                state = RunState;
        }
        break;
    case NameState:
        qWarning() << "Name element is not supposed to have child elements!";
        return false;
    case DescriptionState:
        qWarning() << "Name element is not supposed to have child elements!";
        return false;
    case IconState:
        qWarning() << "Name element is not supposed to have child elements!";
        return false;
    case AuthorsState:
        {
            if(qName != "author")
            {
                qWarning() << "Authors element can only have author elements as children";
                return false;
            }
            state = AuthorState;
        }
        break;
    case WebsiteState:
        qWarning() << "Name element is not supposed to have child elements!";
        return false;
    case AuthorState:
        {
            if(qName == "name")
                state = AuthorNameState;
            else if(qName == "email")
                state = AuthorEmailState;
            else if(qName == "company")
                state = AuthorCompanyState;
            else
            {
                qWarning() << "Unrecognized element" << qName << "in author element!";
                return false;
            }
        }
        break;
    case CategoriesState:
        {
            if(qName != "category")
            {
                qWarning() << "categories element can only have category elements as children";
                return false;
            }
            state = CategoryState;
        }
        break;
    case RunState:
        {
            if(qName == "executable")
                state = ExecutableState;
            else if(qName == "argument")
                state = ArgumentState;
            else
            {
                qWarning() << "Unrecognized element" << qName << "in author element!";
                return false;
            }
        }
        break;
    case AuthorNameState:
    case AuthorEmailState:
    case AuthorCompanyState:
    case CategoryState:
    case ExecutableState:
    case ArgumentState:
        return false;
        qWarning() << "Element" << qName << "cannot have any child elements!";
        return false;

    }

    return true;
}

bool STEManifest::characters(const QString& ch)
{
    switch(state)
    {
    case RootState:
    case PackageState:
    case AuthorsState:
    case AuthorState:
    case CategoriesState:
    case RunState:
        break;
    case NameState:
        name = ch;
        break;
    case DescriptionState:
        description = ch;
        break;
    case IconState:
        icon = ch;
        break;
    case WebsiteState:
        website = ch;
        break;
    case AuthorNameState:
    case AuthorEmailState:
    case AuthorCompanyState:
        break;
    case CategoryState:
        categories.append(ch);
        break;
    case ExecutableState:
        executable = ch;
        break;
    case ArgumentState:
        arguments.append(ch);
        break;
    }

    return true;
}

bool STEManifest::endElement(const QString&, const QString&, const QString&)
{
    switch(state)
    {
    case RootState:
        {
            qWarning() << "Extra end tag after root element closed!?!";
            return false;
        }
        break;
    case PackageState:
        state = RootState;
        break;
    case NameState:
    case DescriptionState:
    case IconState:
    case AuthorsState:
    case WebsiteState:
    case CategoriesState:
        state = PackageState;
        break;
    case AuthorState:
        state = AuthorsState;
        break;
    case AuthorNameState:
    case AuthorEmailState:
    case AuthorCompanyState:
        state = AuthorState;
        break;
    case CategoryState:
        state = CategoriesState;
        break;
    case ExecutableState:
    case ArgumentState:
        state = RunState;
        break;
    }

    return true;
}
