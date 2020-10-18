/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_FILELOGWRITER_H
#define TENSERFLOW_FILELOGWRITER_H


#include <cstddef>


namespace tenserflow {


class FileLogWriter
{
public:
    FileLogWriter() = default;
    FileLogWriter(const char *fileName) { open(fileName); }

    inline bool isOpen() const  { return m_file >= 0; }

    bool open(const char *fileName);
    bool write(const char *data, size_t size);
    bool writeLine(const char *data, size_t size);

private:
    int m_file = -1;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_FILELOGWRITER_H */
