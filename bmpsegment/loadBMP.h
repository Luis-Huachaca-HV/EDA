//
// Created by luishuachaca on 9/28/22.
//

#ifndef BMP2_LOADBMP_H
#define BMP2_LOADBMP_H
#include <cstddef>
#include <iostream>
#include <fstream>


bool loadBMP( const char * const filename, size_t & rows, size_t & columns, char * & image );


short readShort( std::istream & in );


int readLong( std::istream & in );


size_t bytesPerRow( size_t columns );

#endif //BMP2_LOADBMP_H
