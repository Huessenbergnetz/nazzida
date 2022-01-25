#include "configuration.h"

Configuration::Configuration(QObject *parent) : QObject(parent)
{

}

Configuration::~Configuration()
{

}

QString Configuration::language() const
{
    return QStringLiteral("C");
}

void Configuration::setLanguage(const QString &language)
{
    Q_UNUSED(language)
}

#include "moc_configuration.cpp"
