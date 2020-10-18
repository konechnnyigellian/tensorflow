

#ifndef TENSERFLOW_ARGON2_IMPL_H
#define TENSERFLOW_ARGON2_IMPL_H


namespace tenserflow {


class String;


namespace argon2 {


class Impl
{
public:
    static bool select(const String &nameHint, bool benchmark = false);
    static const String &name();
};


}} // namespace tenserflow::argon2


#endif /* TENSERFLOW_ARGON2_IMPL_H */
