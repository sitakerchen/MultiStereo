#ifndef DELAYCALCULATOR_H
#define DELAYCALCULATOR_H

#include <QObject>

class calculator : public QObject
{
    Q_OBJECT
public:
    explicit calculator(QObject *parent = nullptr);

signals:

};

#endif // DELAYCALCULATOR_H
