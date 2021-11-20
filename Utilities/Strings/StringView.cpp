#include "StringView.h"


StringView (const char *str, size_t size) : 
	beg_ (str), size_ (size)
{}