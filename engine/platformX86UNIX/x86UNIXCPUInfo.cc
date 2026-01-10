//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
// Modified for 64-bit compatibility
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "platformX86UNIX/platformX86UNIX.h"
#include "console/console.h"
#include "core/stringTable.h"
#include <math.h>
#include <stdint.h>

Platform::SystemInfo_struct Platform::SystemInfo;

extern void PlatformBlitInit();
extern void SetProcessorInfo(Platform::SystemInfo_struct::Processor& pInfo,
   char* vendor, U32 processor, U32 properties);

static inline uint64_t rdtsc_portable() {
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void Processor::init()
{
   // Get vendor string and features using cpuid
   uint32_t eax, ebx, ecx, edx;
   char vendor[13];

   // cpuid with eax=0 returns vendor string
   __asm__ volatile(
      "cpuid"
      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "a"(0)
   );
   *(uint32_t*)&vendor[0] = ebx;
   *(uint32_t*)&vendor[4] = edx;
   *(uint32_t*)&vendor[8] = ecx;
   vendor[12] = 0;

   // cpuid with eax=1 returns feature flags in edx
   uint32_t processor_type;
   __asm__ volatile(
      "cpuid"
      : "=a"(processor_type), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "a"(1)
   );

   U32 properties = CPU_PROP_C;
   if (edx & (1 << 0))  properties |= CPU_PROP_FPU;
   if (edx & (1 << 23)) properties |= CPU_PROP_MMX;
   if (edx & (1 << 25)) properties |= CPU_PROP_SSE;

   // Check for 3DNow (AMD extended features)
   __asm__ volatile(
      "cpuid"
      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "a"(0x80000001)
   );
   if (edx & (1 << 31)) properties |= CPU_PROP_3DNOW;

   SetProcessorInfo(Platform::SystemInfo.processor, vendor, processor_type, properties);

   // Estimate MHz using rdtsc
   const U32 MS_INTERVAL = 750;
   uint64_t time1 = rdtsc_portable();
   U32 ms = Platform::getRealMilliseconds();
   while (Platform::getRealMilliseconds() < ms + MS_INTERVAL)
   { /* empty */ }
   ms = Platform::getRealMilliseconds() - ms;
   uint64_t time2 = rdtsc_portable();

   uint64_t clockticks = time2 - time1;
   U32 mhz = static_cast<U32>((double)clockticks / (double)ms / 1000.0);

   // Round to nearest standard frequency
   U32 bucket25 = mhz % 25;
   U32 bucket33 = mhz % 33;
   U32 bucket50 = mhz % 50;

   if (bucket50 < 8 || bucket50 > 42)
      Platform::SystemInfo.processor.mhz = U32((mhz + 25.0f) / 50.0f) * 50;
   else if (bucket25 < 5 || bucket25 > 20)
      Platform::SystemInfo.processor.mhz = U32((mhz + 12.5f) / 25.0f) * 25;
   else if (bucket33 < 5 || bucket33 > 28)
      Platform::SystemInfo.processor.mhz = U32((mhz + 16.5f) / 33.0f) * 33;
   else
      Platform::SystemInfo.processor.mhz = mhz;

   Con::printf("Processor Init:");
   Con::printf("   %s, ~%d Mhz", Platform::SystemInfo.processor.name, Platform::SystemInfo.processor.mhz);
   if (Platform::SystemInfo.processor.properties & CPU_PROP_FPU)
      Con::printf("   FPU detected");
   if (Platform::SystemInfo.processor.properties & CPU_PROP_MMX)
      Con::printf("   MMX detected");
   if (Platform::SystemInfo.processor.properties & CPU_PROP_3DNOW)
      Con::printf("   3DNow detected");
   if (Platform::SystemInfo.processor.properties & CPU_PROP_SSE)
      Con::printf("   SSE detected");
   Con::printf(" ");

   PlatformBlitInit();
}
