#include <stdio.h>
#include <windows.h>
#include <versionhelpers.h>

const char* whatOS() {
    if (IsWindowsXPOrGreater() && !IsWindowsXPSP1OrGreater()) return "Windows XP (NT 5.1)";
    if (IsWindowsXPSP1OrGreater() && !IsWindowsXPSP2OrGreater()) return "Windows XP SP1 (NT 5.1)";
    if (IsWindowsXPSP2OrGreater() && !IsWindowsXPSP3OrGreater()) return "Windows XP SP2 (NT 5.1)";
    if (IsWindowsXPSP3OrGreater() && !IsWindowsVistaOrGreater()) return "Windows XP SP3 (NT 5.1)";
    
    if (IsWindowsVistaOrGreater() && !IsWindowsVistaSP1OrGreater()) return "Windows Vista (NT 6.0)";
    if (IsWindowsVistaSP1OrGreater() && !IsWindowsVistaSP2OrGreater()) return "Windows Vista SP1 (NT 6.0)";
    if (IsWindowsVistaSP2OrGreater() && !IsWindows7OrGreater()) return "Windows Vista SP2 (NT 6.0)";
    
    if (IsWindows7OrGreater() && !IsWindows7SP1OrGreater()) return "Windows 7 (NT 6.1)";
    if (IsWindows7SP1OrGreater() && !IsWindows8OrGreater()) return "Windows 7 SP1 (NT 6.1)";
    
    if (IsWindows8OrGreater() && !IsWindows8Point1OrGreater()) return "Windows 8 (NT 6.2)";
    if (IsWindows8Point1OrGreater() && !IsWindows10OrGreater()) return "Windows 8.1 (NT 6.3)";
    
    if (IsWindows10OrGreater()) return "Windows 10 (NT 10.0)";
    
    return "Unable to find OS version - unsupported Windows version";
}

int main() {
    const char* osVersion = whatOS();
    printf("OS Version: %s\n", osVersion);
    return 0;
}
