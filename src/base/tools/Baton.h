

#ifndef TENSERFLOW_BATON_H
#define TENSERFLOW_BATON_H


namespace tenserflow {


template<typename REQ>
class Baton
{
public:
    inline Baton() { req.data = this; }

    REQ req;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_BATON_H */
