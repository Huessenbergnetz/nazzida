#ifndef M20220130T123658_LIQUID_H
#define M20220130T123658_LIQUID_H

#include <Firfuorida/migration.h>

class M20220130T123658_Liquid : public Firfuorida::Migration
{
    Q_OBJECT
    Q_DISABLE_COPY(M20220130T123658_Liquid)
public:
    explicit M20220130T123658_Liquid(Firfuorida::Migrator *parent);
    ~M20220130T123658_Liquid() override;

    void up() override;
    void down() override;
};

#endif // M20220130T123658_LIQUID_H

