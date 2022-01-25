#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>

class Configuration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration() override;

    virtual QString language() const;
    virtual void setLanguage(const QString &language);

signals:
    void languageChanged(const QString &language);

};

#endif // CONFIGURATION_H
