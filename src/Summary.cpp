/* TENSERflow
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


#include <cinttypes>
#include <cstdio>
#include <uv.h>


#include "backend/cpu/Cpu.h"
#include "base/io/log/Log.h"
#include "base/net/stratum/Pool.h"
#include "core/config/Config.h"
#include "core/Controller.h"
#include "crypto/common/Assembly.h"
#include "crypto/common/VirtualMemory.h"
#include "Summary.h"
#include "version.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "crypto/rx/RxConfig.h"
#endif


namespace tenserflow {


#ifdef TENSERFLOW_FEATURE_ASM
static const char *coloredAsmNames[] = {
    RED_BOLD("none"),
    "auto",
    GREEN_BOLD("intel"),
    GREEN_BOLD("ryzen"),
    GREEN_BOLD("bulldozer")
};


inline static const char *asmName(Assembly::Id assembly)
{
    return coloredAsmNames[assembly];
}
#endif


static void print_memory(Config *config)
{
#   ifdef TENSERFLOW_OS_WIN
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") "%s",
               "HUGE PAGES", config->cpu().isHugePages() ? (VirtualMemory::isHugepagesAvailable() ? GREEN_BOLD("permission granted") : RED_BOLD("unavailable")) : RED_BOLD("disabled"));
#   else
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") "%s", "HUGE PAGES",  config->cpu().isHugePages() ? GREEN_BOLD("supported") : RED_BOLD("disabled"));
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
#   ifdef TENSERFLOW_OS_LINUX
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") "%s",
               "1GB PAGES", (VirtualMemory::isOneGbPagesAvailable() ? (config->rx().isOneGbPages() ? GREEN_BOLD("supported") : YELLOW_BOLD("disabled")) : YELLOW_BOLD("unavailable")));
#   else
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") "%s", "1GB PAGES", YELLOW_BOLD("unavailable"));
#   endif
#   endif
}


static void print_cpu(Config *)
{
    const auto info = Cpu::info();

    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s%s (%zu)") " %sx64 %sAES",
               "CPU",
               info->brand(),
               info->packages(),
               info->isX64()          ? GREEN_BOLD_S : RED_BOLD_S "-",
               info->hasAES()         ? GREEN_BOLD_S : RED_BOLD_S "-"
               );
#   if defined(TENSERFLOW_FEATURE_LIBCPUID) || defined (TENSERFLOW_FEATURE_HWLOC)
    Log::print(WHITE_BOLD("   %-13s") BLACK_BOLD("L2:") WHITE_BOLD("%.1f MB") BLACK_BOLD(" L3:") WHITE_BOLD("%.1f MB")
               CYAN_BOLD(" %zu") "C" BLACK_BOLD("/") CYAN_BOLD("%zu") "T"
#              ifdef TENSERFLOW_FEATURE_HWLOC
               BLACK_BOLD(" NUMA:") CYAN_BOLD("%zu")
#              endif
               , "",
               info->L2() / 1048576.0,
               info->L3() / 1048576.0,
               info->cores(),
               info->threads()
#              ifdef TENSERFLOW_FEATURE_HWLOC
               , info->nodes()
#              endif
               );
#   else
    Log::print(WHITE_BOLD("   %-13s") BLACK_BOLD("threads:") CYAN_BOLD("%zu"),
               "",
               info->threads()
               );
#   endif
}


static void print_memory()
{
    constexpr size_t oneGiB = 1024U * 1024U * 1024U;
    const auto freeMem      = static_cast<double>(uv_get_free_memory());
    const auto totalMem     = static_cast<double>(uv_get_total_memory());

    const double percent = freeMem > 0 ? ((totalMem - freeMem) / totalMem * 100.0) : 100.0;

    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") CYAN_BOLD("%.1f/%.1f GB") BLACK_BOLD(" (%.0f%%)"),
               "MEMORY",
               (totalMem - freeMem) / oneGiB,
               totalMem / oneGiB,
               percent
               );
}


static void print_threads(Config *config)
{
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") WHITE_BOLD("%s%d%%"),
               "DONATE",
               config->pools().donateLevel() == 0 ? RED_BOLD_S : "",
               config->pools().donateLevel()
               );

#   ifdef TENSERFLOW_FEATURE_ASM
    if (config->cpu().assembly() == Assembly::AUTO) {
        const Assembly assembly = Cpu::info()->assembly();

        Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13sauto:%s"), "ASSEMBLY", asmName(assembly));
    }
    else {
        Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s%s"), "ASSEMBLY", asmName(config->cpu().assembly()));
    }
#   endif
}


static void print_commands(Config *)
{
    if (Log::isColors()) {
        Log::print(GREEN_BOLD(" * ") WHITE_BOLD("COMMANDS     ") MAGENTA_BG(WHITE_BOLD_S "h") WHITE_BOLD("ashrate, ")
                                                                 MAGENTA_BG(WHITE_BOLD_S "p") WHITE_BOLD("ause, ")
                                                                 MAGENTA_BG(WHITE_BOLD_S "r") WHITE_BOLD("esume, ")
                                                                 WHITE_BOLD("re") MAGENTA_BG(WHITE_BOLD_S "s") WHITE_BOLD("ults, ")
                                                                 MAGENTA_BG(WHITE_BOLD_S "c") WHITE_BOLD("onnection")
                   );
    }
    else {
        Log::print(" * COMMANDS     'h' hashrate, 'p' pause, 'r' resume, 's' results, 'c' connection");
    }
}


} // namespace tenserflow


void tenserflow::Summary::print(Controller *controller)
{
    controller->config()->printVersions();
    print_memory(controller->config());
    print_cpu(controller->config());
    print_memory();
    print_threads(controller->config());
    controller->config()->pools().print();

    print_commands(controller->config());
}



