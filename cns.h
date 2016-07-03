#ifndef CNS_H
#define CNS_H

#include<QMap>
#include<QPair>
#include<QList>
namespace CNS{
    using Triggers = QList<QString>;
    using Substitutions = QList<QPair<QString,QString>>;
    class StateBody{
    public:
        Triggers triggers;
        Substitutions  substitutions;
        QString Comment;
        QString Type;
    };
    class StateDef{
    public:
        Substitutions  substitutions;
        QString Comment;
        QString Type;
    };
    class CNS{
    public:
        QMap<int,QPair<StateDef,QList<StateBody>>> cns;
    };
}

#endif // CNS_H
