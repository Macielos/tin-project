/**
 *  ### main.cpp ###
 *
 *      Funkcja główna aplikacji klienciej.
 *
 */
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ClientInterface.h"

using namespace std;

int main(int argc, char* argv[])
{
    ClientInterface clientInterface;

    if (argc == 4) // wczytano argumenty połączenia z poleceniem
    {
        clientInterface.changeParameter(HOST, argv[1]);
        clientInterface.changeParameter(MESSAGE_PORT, argv[2]);
        clientInterface.changeParameter(DATA_PORT, argv[3]);
    }

    while (clientInterface.getCommand()); // główna pętla programu
    return 0;
}
