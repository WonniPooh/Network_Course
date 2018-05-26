#include "clientnames.h"
#include <QInputDialog>

ClientNames::ClientNames()
{

}

const QString ClientNames::getClientName(quint16 client_id)
{
    return names.value(client_id);
}

bool ClientNames::changeName(quint16 client_id, QString* new_name)
{
    bool exist = names.contains(client_id);

    if(exist)
    {
        if(!new_name)
        {
            QString name = QString("ID " + QString::number(client_id));
            names.insert(client_id, name);
        }
        else
            names.insert(client_id, *new_name);
        return true;
    }
    else
        return false;
}

void ClientNames::processNewConnection(quint16 client_id)
{
    if(names.contains(client_id))
        return;

    QInputDialog input_client_name;
    input_client_name.setTextValue(QString("ID " + QString::number(client_id)));
    input_client_name.setModal(true);
    input_client_name.setLabelText(QString("<H3 align = \"center\">Enter new client name please!</H3>"));
    input_client_name.exec();

    names.insert(client_id, input_client_name.textValue());
}

bool ClientNames::deleteClient(quint16 client_id)
{
    return names.remove(client_id);
}

ClientNames::~ClientNames()
{

}
