/*
 * SPDX-FileCopyrightText: (C) 2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>

class Configuration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(Configuration::BpClass bpClass READ bpClass WRITE setBpClass NOTIFY bpClassChanged)
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration() override;

    enum BpClass : int {
        AccAhh  = 0,
        Esc     = 1,
        EshIsh  = 2
    };
    Q_ENUM(BpClass);

    virtual QString language() const;
    virtual void setLanguage(const QString &language);

    virtual BpClass bpClass() const;
    virtual void setBpClass(BpClass bpClass);

signals:
    void languageChanged(const QString &language);
    void bpClassChanged(BpClass bpClass);

};

#endif // CONFIGURATION_H
