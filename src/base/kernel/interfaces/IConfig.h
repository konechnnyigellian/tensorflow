

#ifndef TENSERFLOW_ICONFIG_H
#define TENSERFLOW_ICONFIG_H


#include "3rdparty/rapidjson/fwd.h"


namespace tenserflow {


class IJsonReader;
class String;


class IConfig
{
public:
    enum Keys {
        // common
        AlgorithmKey         = 'a',
        CoinKey              = 1025,
        ApiWorkerIdKey       = 4002,
        ApiIdKey             = 4005,
        HttpPort             = 4100,
        HttpAccessTokenKey   = 4101,
        HttpRestrictedKey    = 4104,
        HttpEnabledKey       = 4106,
        HttpHostKey          = 4107,
        BackgroundKey        = 'B',
        ColorKey             = 1002,
        ConfigKey            = 'c',
        DonateLevelKey       = 1003,
        KeepAliveKey         = 'k',
        LogFileKey           = 'l',
        PasswordKey          = 'p',
        RetriesKey           = 'r',
        RetryPauseKey        = 'R',
        RigIdKey             = 1012,
        SyslogKey            = 'S',
        UrlKey               = 'o',
        UserAgentKey         = 1008,
        UserKey              = 'u',
        UserpassKey          = 'O',
        ProxyKey             = 'x',
        VerboseKey           = 1100,
        TlsKey               = 1013,
        FingerprintKey       = 1014,
        ProxyDonateKey       = 1017,
        DaemonKey            = 1018,
        DaemonPollKey        = 1019,
        SelfSelectKey        = 1028,
        DataDirKey           = 1035,
        TitleKey             = 1037,
        NoTitleKey           = 1038,
        PauseOnBatteryKey    = 1041,

        // tenserflow common
        CPUPriorityKey       = 1021,
        NicehashKey          = 1006,
        PrintTimeKey         = 1007,

        // tenserflow cpu
        CPUKey               = 1024,
        AVKey                = 'v',
        CPUAffinityKey       = 1020,
        DryRunKey            = 5000,
        HugePagesKey         = 1009,
        ThreadsKey           = 't',
        AssemblyKey          = 1015,
        RandomXInitKey       = 1022,
        RandomXNumaKey       = 1023,
        RandomXModeKey       = 1029,
        RandomX1GbPagesKey   = 1031,
        RandomXWrmsrKey      = 1032,
        RandomXRdmsrKey      = 1033,
        CPUMaxThreadsKey     = 1026,
        MemoryPoolKey        = 1027,
        YieldKey             = 1030,
        AstroBWTMaxSizeKey   = 1034,
        AstroBWTAVX2Key      = 1036,
        Argon2ImplKey        = 1039,
        RandomXCacheQoSKey   = 1040,

        // tenserflow amd
        OclPlatformKey       = 1400,
        OclAffinityKey       = 1401,
        OclDevicesKey        = 1402,
        OclLaunchKey         = 1403,
        OclCacheKey          = 1404,
        OclPrintKey          = 1405,
        OclLoaderKey         = 1406,
        OclSridedIndexKey    = 1407,
        OclMemChunkKey       = 1408,
        OclUnrollKey         = 1409,
        OclCompModeKey       = 1410,
        OclKey               = 1411,

        // tenserflow-proxy
        AccessLogFileKey     = 'A',
        BindKey              = 'b',
        CustomDiffKey        = 1102,
        CustomDiffStatsKey   = 1104,
        DebugKey             = 1101,
        ModeKey              = 'm',
        PoolCoinKey          = 'C',
        ReuseTimeoutKey      = 1106,
        WorkersKey           = 1103,
        WorkersAdvKey        = 1107,
        TlsBindKey           = 1108,
        TlsCertKey           = 1109,
        TlsCertKeyKey        = 1110,
        TlsDHparamKey        = 1111,
        TlsCiphersKey        = 1112,
        TlsCipherSuitesKey   = 1113,
        TlsProtocolsKey      = 1114,
        TlsGenKey            = 1117,
        AlgoExtKey           = 1115,
        ProxyPasswordKey     = 1116,
        LoginFileKey         = 'L',

        // tenserflow nvidia
        CudaMaxThreadsKey    = 1200,
        CudaBFactorKey       = 1201,
        CudaBSleepKey        = 1202,
        CudaDevicesKey       = 1203,
        CudaLaunchKey        = 1204,
        CudaAffinityKey      = 1205,
        CudaMaxUsageKey      = 1206,
        CudaKey              = 1207,
        CudaLoaderKey        = 1208,
        NvmlKey              = 1209,
        HealthPrintTimeKey   = 1210,
    };

    virtual ~IConfig() = default;

    virtual bool isWatch() const                                       = 0;
    virtual bool read(const IJsonReader &reader, const char *fileName) = 0;
    virtual bool save()                                                = 0;
    virtual const String &fileName() const                             = 0;
    virtual void getJSON(rapidjson::Document &doc) const               = 0;
    virtual void setFileName(const char *fileName)                     = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ICONFIG_H
