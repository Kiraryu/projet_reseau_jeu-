#pragma once
#include "Communication.h"

class Param {
public :
	Communication* m_global_com_ptr;
	int* m_socket_ptr;
	Param(Communication* global_com_ptr, int* socket_ptr);
//~Param();
};
