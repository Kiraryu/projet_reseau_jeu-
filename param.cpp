#include "param.h"

Param::Param(Communication* global_com_ptr,int* socket_ptr){
	m_global_com_ptr = global_com_ptr;
	m_socket_ptr = socket_ptr;
}
