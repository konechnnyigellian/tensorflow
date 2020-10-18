

#ifndef TENSERFLOW_ENTRY_H
#define TENSERFLOW_ENTRY_H


namespace tenserflow {


class Process;


class Entry
{
public:
    enum Id {
        Default,
        Usage,
        Version,
        Topo,
        Platforms
    };

    static Id get(const Process &process);
    static int exec(const Process &process, Id id);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_ENTRY_H */
