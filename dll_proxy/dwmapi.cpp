#include <cstdint>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

HMODULE SOriginalDll = nullptr;
extern "C" uintptr_t mProcs[143] = {0};

static void setup_functions()
{
    mProcs[0] = (uintptr_t)GetProcAddress(SOriginalDll, "DllCanUnloadNow");
    mProcs[1] = (uintptr_t)GetProcAddress(SOriginalDll, "DllGetClassObject");
    mProcs[2] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmAttachMilContent");
    mProcs[3] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmDefWindowProc");
    mProcs[4] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmDetachMilContent");
    mProcs[5] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmEnableBlurBehindWindow");
    mProcs[6] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmEnableComposition");
    mProcs[7] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmEnableMMCSS");
    mProcs[8] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmExtendFrameIntoClientArea");
    mProcs[9] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmFlush");
    mProcs[10] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetColorizationColor");
    mProcs[11] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetCompositionTimingInfo");
    mProcs[12] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetGraphicsStreamClient");
    mProcs[13] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetGraphicsStreamTransformHint");
    mProcs[14] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetTransportAttributes");
    mProcs[15] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetUnmetTabRequirements");
    mProcs[16] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmGetWindowAttribute");
    mProcs[17] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmInvalidateIconicBitmaps");
    mProcs[18] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmIsCompositionEnabled");
    mProcs[19] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmModifyPreviousDxFrameDuration");
    mProcs[20] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmQueryThumbnailSourceSize");
    mProcs[21] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmRegisterThumbnail");
    mProcs[22] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmRenderGesture");
    mProcs[23] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmSetDxFrameDuration");
    mProcs[24] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmSetIconicLivePreviewBitmap");
    mProcs[25] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmSetIconicThumbnail");
    mProcs[26] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmSetPresentParameters");
    mProcs[27] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmSetWindowAttribute");
    mProcs[28] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmShowContact");
    mProcs[29] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmTetherContact");
    mProcs[30] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmTetherTextContact");
    mProcs[31] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmTransitionOwnedWindow");
    mProcs[32] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmUnregisterThumbnail");
    mProcs[33] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmUpdateThumbnailProperties");
    mProcs[34] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpAllocateSecurityDescriptor");
    mProcs[35] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpDxGetWindowSharedSurface");
    mProcs[36] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpDxUpdateWindowSharedSurface");
    mProcs[37] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpDxgiIsThreadDesktopComposited");
    mProcs[38] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpEnableDDASupport");
    mProcs[39] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpFreeSecurityDescriptor");
    mProcs[40] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpGetColorizationParameters");
    mProcs[41] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpRenderFlick");
    mProcs[42] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpSetColorizationParameters");
    mProcs[43] = (uintptr_t)GetProcAddress(SOriginalDll, "DwmpUpdateProxyWindowForCapture");
    mProcs[44] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(100));
    mProcs[45] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(101));
    mProcs[46] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(102));
    mProcs[47] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(103));
    mProcs[48] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(104));
    mProcs[49] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(105));
    mProcs[50] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(106));
    mProcs[51] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(107));
    mProcs[52] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(108));
    mProcs[53] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(109));
    mProcs[54] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(110));
    mProcs[55] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(111));
    mProcs[56] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(112));
    mProcs[57] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(113));
    mProcs[58] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(114));
    mProcs[59] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(115));
    mProcs[60] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(116));
    mProcs[61] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(117));
    mProcs[62] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(118));
    mProcs[63] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(119));
    mProcs[64] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(120));
    mProcs[65] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(121));
    mProcs[66] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(122));
    mProcs[67] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(123));
    mProcs[68] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(124));
    mProcs[69] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(125));
    mProcs[70] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(126));
    mProcs[71] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(127));
    mProcs[72] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(128));
    mProcs[73] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(129));
    mProcs[74] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(130));
    mProcs[75] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(131));
    mProcs[76] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(132));
    mProcs[77] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(133));
    mProcs[78] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(134));
    mProcs[79] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(135));
    mProcs[80] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(136));
    mProcs[81] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(137));
    mProcs[82] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(138));
    mProcs[83] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(139));
    mProcs[84] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(140));
    mProcs[85] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(141));
    mProcs[86] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(142));
    mProcs[87] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(143));
    mProcs[88] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(144));
    mProcs[89] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(145));
    mProcs[90] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(146));
    mProcs[91] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(147));
    mProcs[92] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(148));
    mProcs[93] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(149));
    mProcs[94] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(150));
    mProcs[95] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(151));
    mProcs[96] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(152));
    mProcs[97] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(153));
    mProcs[98] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(154));
    mProcs[99] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(155));
    mProcs[100] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(156));
    mProcs[101] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(157));
    mProcs[102] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(158));
    mProcs[103] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(159));
    mProcs[104] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(160));
    mProcs[105] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(161));
    mProcs[106] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(162));
    mProcs[107] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(163));
    mProcs[108] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(164));
    mProcs[109] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(165));
    mProcs[110] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(166));
    mProcs[111] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(167));
    mProcs[112] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(168));
    mProcs[113] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(169));
    mProcs[114] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(170));
    mProcs[115] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(171));
    mProcs[116] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(172));
    mProcs[117] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(173));
    mProcs[118] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(174));
    mProcs[119] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(175));
    mProcs[120] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(176));
    mProcs[121] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(177));
    mProcs[122] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(178));
    mProcs[123] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(179));
    mProcs[124] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(180));
    mProcs[125] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(181));
    mProcs[126] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(182));
    mProcs[127] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(183));
    mProcs[128] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(184));
    mProcs[129] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(185));
    mProcs[130] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(186));
    mProcs[131] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(187));
    mProcs[132] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(188));
    mProcs[133] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(189));
    mProcs[134] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(190));
    mProcs[135] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(191));
    mProcs[136] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(192));
    mProcs[137] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(193));
    mProcs[138] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(194));
    mProcs[139] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(195));
    mProcs[140] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(196));
    mProcs[141] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(197));
    mProcs[142] = (uintptr_t)GetProcAddress(SOriginalDll, MAKEINTRESOURCEA(198));
}

static void load_original_dll()
{
    wchar_t path[MAX_PATH];
    GetSystemDirectoryW(path, MAX_PATH);

    std::wstring dll_path = std::wstring(path) + L"\\dwmapi.dll";

    SOriginalDll = LoadLibraryW(dll_path.c_str());
    if (!SOriginalDll)
    {
        MessageBoxW(nullptr, L"Failed to load proxy DLL", L"Proxy Loader Error", MB_OK | MB_ICONERROR);
        ExitProcess(0);
    }
}

#include "dll_proxy.h"

extern "C" void DoDllProxy()
{
    load_original_dll();
    setup_functions();
}
