#ifndef DAILYLIQUIDS_H
#define DAILYLIQUIDS_H

#include <QObject>
#include <QDate>
#include <QSharedDataPointer>

class DailyLiquidsData;

class DailyLiquids
{
    Q_GADGET
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int personId READ personId CONSTANT)
    Q_PROPERTY(QDate day READ day CONSTANT)
    Q_PROPERTY(int input READ input CONSTANT)
    Q_PROPERTY(int output READ output CONSTANT)
    Q_PROPERTY(int difference READ difference CONSTANT)
    Q_PROPERTY(int entries READ entries CONSTANT)
public:
    DailyLiquids();
    DailyLiquids(int id, int personId, QDate day, int input, int output, int entries);
    DailyLiquids(const DailyLiquids &other);
    DailyLiquids(DailyLiquids &&other) noexcept;
    DailyLiquids& operator=(const DailyLiquids &other);
    DailyLiquids& operator=(DailyLiquids &&other) noexcept;
    ~DailyLiquids();

    void swap(DailyLiquids &other) noexcept;

    int id() const;
    int personId() const;
    QDate day() const;
    int input() const;
    int output() const;
    int difference() const;
    int entries() const;

private:
    friend class DailyLiquidListModel;
    QSharedDataPointer<DailyLiquidsData> d;
};

Q_DECLARE_METATYPE(DailyLiquids)
Q_DECLARE_TYPEINFO(DailyLiquids, Q_MOVABLE_TYPE);

#endif // DAILYLIQUIDS_H
