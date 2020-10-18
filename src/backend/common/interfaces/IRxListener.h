/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_IRXLISTENER_H
#define TENSERFLOW_IRXLISTENER_H


namespace tenserflow {


class IRxListener
{
public:
    virtual ~IRxListener() = default;

#   ifdef TENSERFLOW_ALGO_RANDOMX
    virtual void onDatasetReady() = 0;
#   endif
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IRXLISTENER_H
