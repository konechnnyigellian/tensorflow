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

#ifndef TENSERFLOW_IAPILISTENER_H
#define TENSERFLOW_IAPILISTENER_H


namespace tenserflow {


class IApiRequest;


class IApiListener
{
public:
    virtual ~IApiListener() = default;

#   ifdef TENSERFLOW_FEATURE_API
    virtual void onRequest(IApiRequest &request) = 0;
#   endif
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IAPILISTENER_H
