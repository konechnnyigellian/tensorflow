/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_FILELOG_H
#define TENSERFLOW_FILELOG_H


#include "base/io/log/FileLogWriter.h"
#include "base/kernel/interfaces/ILogBackend.h"


namespace tenserflow {


class FileLog : public ILogBackend
{
public:
    FileLog(const char *fileName);

protected:
    void print(int level, const char *line, size_t offset, size_t size, bool colors) override;

private:
    FileLogWriter m_writer;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_FILELOG_H */
