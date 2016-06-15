#ifndef CNS_H
#define CNS_H

#include<QMap>
#include<QPair>

class CNS
{
public:
    CNS();
    QMap<int,QPair<QString,QPair<QString,QString>>> substitutions;

};

#endif // CNS_H
