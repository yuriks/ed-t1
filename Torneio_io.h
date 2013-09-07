// Torneio_io.h
// Codigo de leitura/gravacao especifico para `Torneio`.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_TORNEIO_IO_H
#define YURIKS_T1_TORNEIO_IO_H

#include "Torneio.h"

#include <iostream>

namespace yuriks
{
	void grava_torneio(std::ostream& f, const Torneio& torneio);
	bool le_torneio(std::istream& f, Torneio& torneio);
} // namespace yuriks

#endif // YURIKS_T1_TORNEIO_IO_H
