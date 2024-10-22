#include <OS/Console/PXConsole.h>

#define PXLDAPTest 0
#define PXKeyBoardVirtualTest 0

int _fltused = 0;

#if 1

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Comctl32.lib")
#endif


#if 0
#include "OS/TestSystemInfo.h"
#include "Graphic/TestWindow.h"
#include "Graphic/TestOpenGL.h"
#include "Graphic/TestFont.h"
#include "Compiler/TestPXCompiler.h"
#include "Graphic/TestImage.h"
#include "Compiler/TestYAML.h"
#include "Sound/TestSound.h"
#include "OS/TestSocket.h"
#include "Service/TestFTP.h"
#include "Service/TestSBP.h"
#endif

#include "Network/PXNetworkTest.h"
#include "Window/PXWindowTest.h"

#include <OS/Console/PXConsole.h>
#include <Media/FastFile/PXFastFile.h>
#include <Media/C/PXC.h>
#include <Media/PXDocument.h>

/*

void OnFileElementDetected(PXFileElementInfo* pxFileElementInfo)
{
    switch (pxFileElementInfo->Type)
    {
        case PXFileElementInfoTypeInvalid:
        case PXFileElementInfoTypeFile:
        {
            printf("| %-4s | %-20ls | %i |\n", "File", pxFileElementInfo->Name, pxFileElementInfo->Size);        

            break;
        }
        case PXFileElementInfoTypeDictionary:
        {
            printf("| %-4s | %-20ls |\n", "DIR", pxFileElementInfo->Name);
            break;
        }

        default:
            break;
    }


}



void PXTextMatchTest()
{
    const PXBool a = PXTextMatchW(L"MyFile.h", 8, L"*.h", 3);
    printf("");

    const PXBool b = PXTextMatchW(L"MyFile.c", 8, L"*.h", 3);
    printf("");

    const PXBool c = PXTextMatchW(L"MyFile.*", 8, L"*.h", 3);
    printf("");
}*/

#include "Compiler/PXTestCompiler.h"
#include "Debug/PXTestDebug.h"
#include "Media/PXMediaTest.h"
#include "Window/PXWindowTest.h"

#include <OS/Hardware/PXHardware.h>
#include <OS/Hardware/PXVideo.h>
#include <OS/Hardware/PXProcessor.h>
#include <Math/PXMath.h>
#include <OS/Time/PXTime.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/Debug/PXDebug.h>
#include <OS/Memory/PXMemory.h>
#include <Media/BinaryWindows/PXBinaryWindows.h>



#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <OS/Memory/PXMemory.h>

#pragma comment(lib, "pdh.lib")


#include <wbemidl.h>
#include <oleauto.h>
#pragma comment(lib, "wbemuuid.lib")






#include <OS/File/PXDirectory.h>








void PXAPI Trace_File()
{
    PXText pxText;

    PXDirectoryIterator pxDirectoryIterator;

  //  PXConsoleWriteF(0, "File: %s\n", pxTextSubDir.TextA);
}

void PXAPI Trace_Folder(PXDirectoryIterator* parentDir, PXText* pxText)
{
    
}


void PXAPI Trace_FolderFiles(PXText* pxText)
{
    Trace_Folder(PXNull, pxText);
}


#include <OS/GUI/PXGUI.h>

int main()
{
    PXConsoleWrite(0, "[i] Starting testing...\n");



    PXGUIIconGetViaExtension("vob");



    PXText pxTextSubDir;
    PXTextMakeFixedA(&pxTextSubDir, "N:\\NAS\\");

    PXDirectorySearchCache pxDirectorySearchCache;

    PXDirectorySearch(&pxDirectorySearchCache, &pxTextSubDir);

    Trace_FolderFiles(&pxTextSubDir);


#if 000000000000000000000000















    /*

    HRESULT hres;

    // Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if(FAILED(hres)) {
        printf("Failed to initialize COM library. Error code = 0x%x\n", hres);
        return 1; // Program has failed.
    }

    // Set general COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if(FAILED(hres)) {
        printf("Failed to initialize security. Error code = 0x%x\n", hres);
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemAdministrativeLocator, //CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    if(FAILED(hres)) {
        printf("Failed to create IWbemLocator object. Error code = 0x%x\n", hres);
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Connect to WMI through the IWbemLocator::ConnectServer method
    IWbemServices* pSvc = NULL;

    hres = pLoc->ConnectServer(
        L"ROOT\\CIMV2", // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        WBEM_FLAG_CONNECT_USE_MAX_WAIT,                    // Security flags
        0,                       // Authority
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    if(FAILED(hres)) {
        printf("Could not connect. Error code = 0x%x\n", hres);
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    printf("Connected to ROOT\\CIMV2 WMI namespace\n");

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // Client identity
        EOAC_NONE                    // Proxy capabilities
    );

    if(FAILED(hres)) {
        printf("Could not set proxy blanket. Error code = 0x%x\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    IEnumWbemClassObject* enumWbemClassObject = PXNull;

    HRESULT SS = pSvc->CreateClassEnum
    (
        PXNull,
        WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
        PXNull,
        &enumWbemClassObject
    );


    {

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        HRESULT eeg = 0;

        do
        {
            eeg = enumWbemClassObject->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            VARIANT vtProp;
            PXClear(VARIANT, &vtProp);

            BSTR name = 0;

            // Get the value of the CurrentReading property
            HRESULT hr = pclsObj->Next(PXNull, &name, &vtProp, PXNull, PXNull);

            PXActionResult xxasew = PXWindowsErrorCurrent(0);

                printf("");

        } while(eeg);

       


    }




    // Use the IWbemServices pointer to make requests of WMI
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        TEXT("WQL"),
        TEXT("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY,
        NULL,
        &pEnumerator);

    if(FAILED(hres)) {
        printf("Query for temperature probe failed. Error code = 0x%x\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Get the data from the query
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while(pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        PXActionResult xxasew = PXWindowsErrorCurrent(0);

        if(0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        PXClear(VARIANT, &vtProp);

        // Get the value of the CurrentReading property
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if(SUCCEEDED(hr)) {

            switch(vtProp.vt)
            {
                case VT_BSTR:
                {
                    printf("- %ls\n", vtProp.bstrVal);
                    break;
                }

                case VT_INT:
                {
                    double tempCelsius = ((double)vtProp.intVal / 10.0) - 273.15;
                    printf("CPU Temperature: %.2f°C\n", tempCelsius);
                    break;
                }


                default:
                    break;
            }

       
        }
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();












    */









    CONST ULONG SAMPLE_INTERVAL_MS = 1000;
    CONST PWSTR BROWSE_DIALOG_CAPTION = L"Select a counter to monitor.";




    PDH_STATUS Status;
    HQUERY Query = NULL;
    HCOUNTER Counter;
    PDH_FMT_COUNTERVALUE DisplayValue;
    DWORD CounterType;
    SYSTEMTIME SampleTime;
    PDH_BROWSE_DLG_CONFIG BrowseDlgData;
    WCHAR CounterPathBuffer[512];

    //
    // Create a query.
    //

    Status = PdhOpenQuery(NULL, NULL, &Query);

    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhOpenQuery failed with status 0x%x.", Status);
    }

#if 0



    //
    // Initialize the browser dialog window settings.
    //

    ZeroMemory(&CounterPathBuffer, sizeof(CounterPathBuffer));
    ZeroMemory(&BrowseDlgData, sizeof(PDH_BROWSE_DLG_CONFIG));

    BrowseDlgData.bIncludeInstanceIndex = 0;
    BrowseDlgData.bSingleCounterPerAdd = 0;
    BrowseDlgData.bSingleCounterPerDialog = 0;
    BrowseDlgData.bLocalCountersOnly = 0;
    BrowseDlgData.bWildCardInstances = 0;
    BrowseDlgData.bHideDetailBox = 0;
    BrowseDlgData.bInitializePath = 0;
    BrowseDlgData.bDisableMachineSelection = 0;
    BrowseDlgData.bIncludeCostlyObjects = 0;
    BrowseDlgData.bShowObjectBrowser = 0;
    BrowseDlgData.hWndOwner = NULL;
    BrowseDlgData.szReturnPathBuffer = CounterPathBuffer;
    BrowseDlgData.cchReturnPathLength = 512;
    BrowseDlgData.pCallBack = NULL;
    BrowseDlgData.dwCallBackArg = 0;
    BrowseDlgData.CallBackStatus = ERROR_SUCCESS;
    BrowseDlgData.dwDefaultDetailLevel = PERF_DETAIL_WIZARD;
    BrowseDlgData.szDialogBoxCaption = BROWSE_DIALOG_CAPTION;

    //
    // Display the counter browser window. The dialog is configured
    // to return a single selection from the counter list.
    //

    Status = PdhBrowseCounters(&BrowseDlgData);


    if(Status != ERROR_SUCCESS)
    {
        if(Status == PDH_DIALOG_CANCELLED)
        {
            wprintf(L"\nDialog canceled by user. %ls", CounterPathBuffer);
        }
        else
        {
            wprintf(L"\nPdhBrowseCounters failed with status 0x%x.", Status);
        }
    }
    else if(wcslen(CounterPathBuffer) == 0)
    {
        wprintf(L"\nUser did not select any counter.");

        //wsprintf(CounterPathBuffer, "\\ProcessorInfo");
    }
    else
    {
        wprintf(L"\nCounter selected: %s\n", CounterPathBuffer);
    }

    //
    // Add the selected counter to the query.
    //

    Status = PdhAddCounter(Query, CounterPathBuffer, 0, &Counter);
    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhAddCounter failed with status 0x%x.", Status);
    }

    //
    // Most counters require two sample values to display a formatted value.
    // PDH stores the current sample value and the previously collected
    // sample value. This call retrieves the first value that will be used
    // by PdhGetFormattedCounterValue in the first iteration of the loop
    // Note that this value is lost if the counter does not require two
    // values to compute a displayable value.
    //

    Status = PdhCollectQueryData(Query);
    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhCollectQueryData failed with 0x%x.\n", Status);
    }

    //
    // Print counter values until a key is pressed.
    //

    while(!_kbhit())
    {
        Sleep(SAMPLE_INTERVAL_MS);

        GetLocalTime(&SampleTime);

        Status = PdhCollectQueryData(Query);
        if(Status != ERROR_SUCCESS)
        {
            wprintf(L"\nPdhCollectQueryData failed with status 0x%x.", Status);
        }

        wprintf(L"\n\"%2.2d/%2.2d/%4.4d %2.2d:%2.2d:%2.2d.%3.3d\"",
                SampleTime.wMonth,
                SampleTime.wDay,
                SampleTime.wYear,
                SampleTime.wHour,
                SampleTime.wMinute,
                SampleTime.wSecond,
                SampleTime.wMilliseconds);

        //
        // Compute a displayable value for the counter.
        //

        Status = PdhGetFormattedCounterValue(Counter,
                                             PDH_FMT_LONG,
                                             &CounterType,
                                             &DisplayValue);
        if(Status != ERROR_SUCCESS)
        {
            wprintf(L"\nPdhGetFormattedCounterValue failed with status 0x%x.", Status);
 
        }

        wprintf(L",\"%5d\"", DisplayValue.longValue);
    }


#elif 0




char* buffer = (char*)malloc(256);
DWORD bufferSIze = 256;

auto asas = PdhEnumObjectsA
(
    PXNull,
    PXNull,
    buffer,
    &bufferSIze,
    PERF_DETAIL_WIZARD,
    TRUE
);

buffer = (char*)malloc(bufferSIze);
PXClearList(char, buffer, bufferSIze);

asas = PdhEnumObjectsA
(
    PXNull,
    PXNull,
    buffer,
    &bufferSIze,
    PERF_DETAIL_WIZARD,
    TRUE
);


char* stringCurrent = buffer;

for(size_t i = 0; ; ++i)
{
    if(!*stringCurrent)
    {
        break;
    }

    PXSize size = PXTextLengthA(stringCurrent, 512);

    printf("%3i - %s\n", i, stringCurrent);

 


    char* mszCounterList = 0;
    DWORD mszCounterListSize = 0;
    char* mszInstanceList = 0;
    DWORD mszInstanceListSize = 0;

    auto xas = PdhEnumObjectItemsA
    (
        PXNull,
        PXNull,
        stringCurrent,
        mszCounterList,
        &mszCounterListSize,
        mszInstanceList,
        &mszInstanceListSize,
        PERF_DETAIL_WIZARD,
        TRUE
    );

    mszCounterList = (char*)malloc(mszCounterListSize);
    PXClearList(char, mszCounterList, mszCounterListSize);

    mszInstanceList = (char*)malloc(mszInstanceListSize);
    PXClearList(char, mszInstanceList, mszInstanceListSize);

    xas = PdhEnumObjectItemsA
    (
        PXNull,
        PXNull,
        stringCurrent,
        mszCounterList,
        &mszCounterListSize,
        mszInstanceList,
        &mszInstanceListSize,
        PERF_DETAIL_WIZARD,
        TRUE
    );


    char* stringCuursorAA = mszCounterList;

    for(size_t w = 0; ; w++)
    {
        if(!*stringCuursorAA)
        {
            break;
        }

        PXSize sizeww = PXTextLengthA(stringCuursorAA, 512);



        printf("         %s\n", stringCuursorAA);


        stringCuursorAA += sizeww + 1;
    }



    stringCurrent += size + 1;
}





// Add the CPU usage counter to the query
wchar_t text[] = L"\\System\\System Calls/sec";
//wchar_t text[] = L"\\Processor(_Total)\\% Processor Time";

if(PdhAddCounter(Query, text, 0, &Counter) != ERROR_SUCCESS) {
    fprintf(stderr, "PdhAddCounter failed\n");
    PdhCloseQuery(Query);
    return 1;
}

while(1)
{
    // Collect the initial data sample
    if(PdhCollectQueryData(Query) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhCollectQueryData failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    // Wait for a second to collect the next sample
    Sleep(1000);

    // Collect the next data sample and get the formatted counter value
    if(PdhCollectQueryData(Query) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhCollectQueryData failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    if(PdhGetFormattedCounterValue(Counter, PDH_FMT_LARGE, NULL, &DisplayValue) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhGetFormattedCounterValue failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    // Print the CPU usage percentage
    printf("CPU Usage: %li\n", DisplayValue.largeValue);
}





#endif
























    PXTestNetworkAll();




    PXFileOpenInfo pxFileOpenInfo;
    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
    pxFileOpenInfo.FilePathAdress = "TEST.txt";
    pxFileOpenInfo.FlagList = PXFileIOInfoFilePhysical | PXFileIOInfoAllowMapping;
    pxFileOpenInfo.FileSizeRequest = 420;
    pxFileOpenInfo.AccessMode = PXAccessModeWriteOnly;

    PXFile pxFile;    
    PXFileOpen(&pxFile, &pxFileOpenInfo);

    for(size_t i = 0; i < pxFileOpenInfo.FileSizeRequest; ++i)
    {
        PXByte x = i % 0xFF;

        PXFileWriteB(&pxFile, &x, 1);
    }

    PXFileClose(&pxFile);










    PXBinaryWindows pxBinaryWindows;

    PXResourceTransphereInfo pxResourceTransphereInfo;
    PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
    pxResourceTransphereInfo.ResourceTarget = &pxBinaryWindows;

    PXResourceLoadA(&pxResourceTransphereInfo, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\[Export]\\PXUltimaCTest\\64B-Windows-Debug\\D.zip");












    int* dsfdsf = 0;// PXMemoryMallocT(int, 420);

    PXThread* threads = 0;// PXMemoryMallocT(PXThread, 1234);
    const PXSize xx = 1234;
    PXSize oeoeo = 0;


    PXThread* refA = threads;
    PXThread** refB = &refA;

   // HeapAlloc();


    PXProcessThreadsListAll(PXNull, refB, xx, &oeoeo);


    float percent = 0;

    for(size_t i = 0; i < 60000; i++)
    {
        const DWORD max = 0x7FFF;
        const DWORD min = 0x25;
    
        percent += 0.0001;

        DWORD hz = (percent * max) + min;  // 0x25 through 0x7FFF)



        Beep(hz, 100);
        PXConsoleWriteF(0, "[Beep] %i\n", hz);
    }

    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONERROR);
    MessageBeep(MB_ICONHAND);
    MessageBeep(MB_ICONINFORMATION);
    MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_ICONSTOP);
    MessageBeep(MB_ICONWARNING);
    MessageBeep(MB_OK);


    const PXSize amount = 100000000;
    const float res = 1/25.0f;
    
    PXConsoleWriteF(0, "[%3i] --- Start ---\n", amount);
    
    PXInt64U startTime = PXTimeCounterStampGet();

    for(PXSize i = 0; i < amount; i++)
    {
        float input = i * res;
        float output = PXMathSinusF(input);

        //PXConsoleWriteF(0, "[%3i] %6.4f -> %6.4f\n", i, input, output);
    }

    PXInt64U endTime = PXTimeCounterStampGet();
    PXInt64U delta = endTime - startTime;
    float endTimeInS = PXTimeCounterStampToSecoundsF(delta);

    float avg = delta / (float)amount;

    PXConsoleWriteF(0, "[%3i] Took: %6.3fs. AVG: %6.3fus\n", amount, endTimeInS, avg);




    PXProcessor pxProcessor;

    PXProcessorFetchInfo(&pxProcessor);

    PXVideoCaptureDeviceList();





    
    PXResourceTransphereInfo pxResourceLoadInfo;
    pxResourceLoadInfo.Manager = 0;
   // pxResourceLoadInfo.Target = 0;
    pxResourceLoadInfo.FileReference = PXNull;
   // pxResourceLoadInfo.Type = 0;


    PXResourceLoadA(&pxResourceLoadInfo, "H:\\S.n64");


#if 0
    PXHardwareInfo pxHardwareInfo;
    PXHardwareInfoScan(&pxHardwareInfo, PXHardwareInfoAll);

    while(1)
    {
        PXInt32S temperature = 0;



        //PXProcessorTemperature(&temperature);


    }
#endif

    PXTestCompilerAll();

    PXTestWindowAll();
    PXMediaTestAll();
    
    PXTestDebugAll();






#if 0

    PXResourceLoadInfo pxResourceLoadInfo;
    pxResourceLoadInfo.Target = 0;
    pxResourceLoadInfo.FileReference = 0;
    pxResourceLoadInfo.Type = PXGraphicResourceTypeInvalid;


    PXResourceLoadA(&pxResourceLoadInfo, "N:\\NAS\\Games\\Steam\\steamapps\\common\\Call of Duty Black Ops II\\zone\\all\\afghanistan.ff");

#endif // 1



#if 0
    PXTestWindowAll();
#endif // 0

#if 0
    PXTestNetworkAll();
#endif // 0






#if 0
    {
        PXBinaryWindows binaryWindows;

        const PXActionResult result = PXResourceLoadA(&binaryWindows, "C:\\Data\\WorkSpace\\[GIT]\\BitFireEngine\\[Export]\\GameCleaved\\32B-Windows-Release\\GameCleaved2K.exe");

        printf("\n");
    }
#endif // 1




#if 0
    {
        PXImage pxImage;
        PXClear(PXImage, &pxImage);

        const PXActionResult pxLoadResult = PXResourceLoadA(&pxImage, "_TEST_DATA_INPUT_\\ImageInput.bmp");
        const PXActionResult pxSaveResult = PXResourceSaveA(&pxImage, "_TEST_DATA_INPUT_\\ImageInput_COPY.bmp", PXFileFormatBitMap);

        printf("\n");
    }
#endif // 1


#if 0 // XML -> Document -> Image
    {
        PXKnowlegeGraph pxKnowlegeGraph;
        PXDocument pxDocument;
        PXImage pxImage;

        const PXActionResult pxLoadResult = PXResourceLoadA(&pxDocument, "_TEST_DATA_INPUT_\\books.xml");

        const PXActionResult pxGraphResult = PXKnowlegeGraphLoadAndBuild(&pxKnowlegeGraph, &pxDocument, &pxImage);

        const PXActionResult pxSaveResult = PXResourceSaveA(&pxImage, "_TEST_DATA_INPUT_\\books.bmp", PXFileFormatBitMap);

        printf("\n");

        return 0;
    }
#endif // 1 // XML -> Document -> Image





#if 0
    PXFile pxFile;

    PXResourceLoadA(&pxFile, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaC\\Media\\PXImage.h");
#endif // 0




#if 0
    PXBinaryWindows pxBinaryWindows;

    PXResourceLoadA(&pxBinaryWindows, "C:\\Windows\\System32\\user32.dll");
#endif // 0


    
#if 0
    while (1)
    {
        PXInt32U temp;

        PXProcessorTemperature(&temp);

        PXonsoleGoToXY(0,0);
        printf("CPU temp : %i\n", temp);

    }
#endif


#if 0
    TestSoundAll();
#endif // 0

    


#if 0
    
    PXImage pxImage;

    PXResourceLoadA(&pxImage, "N:\\IMG_0147.CR3");
#endif


















    //PXBinaryLinux pxELF;
    //PXResourceLoadA(&pxELF, "N:\\NAS\\Games\\PC\\Re-Volt_Linux\\rvgl.64.elf");

#if 0
    
    PXBinaryWindows pxEXE;
    //PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/32B-Windows-Debug/PXUltimaCTest.exe");
    PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/64B-Windows-Debug/PXUltima.dll");
#endif


#if 0
    // X86
    {
        PXInt32U coolNumber = 0xAABBCCDD;

        //PXEndianSwapI32U(&coolNumber);

        PXBool result = coolNumber == 0xDDCCBBAA;

        printf("Hello, \n");

    }
#endif




    



#if 0
    // Direct X - Test
    PXWindow pxWindow;
    PXWindowConstruct(&pxWindow);

    PXWindowCreateA(&pxWindow, -1, -1, "Direct X - Test", PXTrue);

    PXAwaitChangeCU(&pxWindow.IsRunning);


    const float vertices[] =
    {
#if 1
        -0.5f, -0.5f, 1, 0xffff0000, // x, y, z, rhw, color
        0.0f,  0.5f, 1, 0xff00ffff,
         0.5f, -0.5f, 1, 0xff00ff00
    

#else 
        50.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, // x, y, z, rhw, color
        250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00,
        50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff,
#endif
};

    PXGraphicSelect(&pxWindow.GraphicInstance);


    PXModel pxModel;
    PXObjectClear(PXModel, &pxModel);
    pxModel.VertexBuffer.VertexData = vertices;
    pxModel.VertexBuffer.VertexDataSize = sizeof(vertices);// / sizeof(float);
    pxModel.VertexBuffer.VertexDataRowSize = sizeof(vertices) / 3;
    pxModel.VertexBuffer.Format = PXVertexBufferFormatXYZC; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

    PXGraphicModelRegister(&pxWindow.GraphicInstance, &pxModel);

    while (1)
    {
        const PXColorRGBAF pxColorRGBAF = { 0.3f,0.3f,0.3f,1.0f };

        PXGraphicClear(&pxWindow.GraphicInstance, &pxColorRGBAF);

        PXGraphicSceneBegin(&pxWindow.GraphicInstance);

        PXGraphicModelDraw(&pxWindow.GraphicInstance, &pxModel);

        PXGraphicSceneEnd(&pxWindow.GraphicInstance);
        PXGraphicSceneDeploy(&pxWindow.GraphicInstance);
    }

    PXWindowDestruct(&pxWindow);


    printf("\n");



#endif




#if 0

    vswprintf_s;

    PXText filePath;
    PXTextMakeFixedA(&filePath, "_TEST_DATA_INPUT_/A.fnt");

    PXSpriteFont pxSpriteFont;
    PXActionResult result = PXFontLoad(&pxSpriteFont, &filePath);

    printf("OK\n");

#endif



#if PXLDAPTest

    printf("Begin");

    PXLDAPClient pxLDAP;
    PXLDAPClientConstruct(&pxLDAP);

    PXLDAPConnectionInfo pxLDAPConnectionInfo;
    pxLDAPConnectionInfo.ConnectionOriented = PXTrue;
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.Host, Host, "ipa.demo1.freeipa.org");
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.ConnectionDomain, ConnectionDomain, "uid=admin,cn=users,cn=accounts,dc=demo1,dc=freeipa,dc=org");
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.AuthenticationCredentials, AuthenticationCredentials, "Secret123");
    pxLDAPConnectionInfo.Port = PXLDAPPortDefault;
    pxLDAPConnectionInfo.SSLUse = 0;
    pxLDAPConnectionInfo.ConnectTimeout = 2000;
    pxLDAPConnectionInfo.AuthenticationMethod = PXLDAPAuthenticationMethodInvalid;

    PXActionResult openResult = PXLDAPClientOpen(&pxLDAP, &pxLDAPConnectionInfo);
    

    PXLDAPSearchInfo pxLDAPSearchInfo;
    PXTextMakeFixedNamedA(&pxLDAPSearchInfo.EntryName, EntryName, "dc=demo1,dc=freeipa,dc=org");
    PXTextMakeFixedNamedA(&pxLDAPSearchInfo.Filter, Filter, "(objectclass=person)");
    pxLDAPSearchInfo.Async = PXFalse;
    pxLDAPSearchInfo.OnlyTypesRequired = PXFalse;
    pxLDAPSearchInfo.AttributeList = PXNull;
    pxLDAPSearchInfo.Scope = PXLDAPScopeSubTree;

    const PXActionResult searchResult = PXLDAPClientSearch(&pxLDAP, &pxLDAPSearchInfo);
    const PXBool successful = PXActionSuccessful == searchResult;

    if (successful)
    {
        

        for (LDAPMessage* msg = ldap_first_entry(pxLDAP.ID, pxLDAP.SearchResult); msg; msg = ldap_next_entry(pxLDAP.ID, msg))
        {
            ULONG ReturnCode;                            // returned by server
            PSTR MatchedDNs;         // free with ldap_memfree
            PSTR ErrorMessage;       // free with ldap_memfree
            PZPSTR Referrals;        // free with ldap_value_freeA
            PLDAPControlA* ServerControls;               // free with ldap_free_controlsA
            const ULONG result = ldap_parse_resultA(pxLDAP.ID, msg, &ReturnCode, &MatchedDNs,&ErrorMessage,&Referrals, &ServerControls, PXFalse);
            const PXBool success = LDAP_SUCCESS == result;

            if (success)
            {
                BerElement* berElement = 0;
                
                printf("[%p] ID:%i Type:%i\n", msg, msg->lm_msgid, msg->lm_msgtype);

                for (char* attribute = ldap_first_attributeA(pxLDAP.ID, msg, &berElement); attribute; attribute = ldap_next_attributeA(pxLDAP.ID, msg, berElement))
                {

                    struct berval** calxx = ldap_get_values_lenA(pxLDAP.ID, msg, attribute);
                    const PXBool valueFetchSuccess = PXNull != calxx;

                    if (valueFetchSuccess)
                    {
                        printf("\t- %-25s : %-25s\n", attribute, (*calxx)->bv_val);
                    
                    }
                    else
                    {
                        printf("\t- %-25s : %-25s\n", attribute, 0);
                    }                
                }

                printf("\n\n");
            }
            else
            {
                printf("[%p] Error\n", msg);
            }

            
        }
    }


    PXLDAPClientDestruct(&pxLDAP);

    printf("END");

#endif 


#if PXKeyBoardVirtualTest
    PXKeyBoardVirtualInput inputList[4];

    PXKeyBoardVirtualInputSet(&inputList[0], KeyWorld1, PXKeyPressStateDown);
    PXKeyBoardVirtualInputSet(&inputList[1], KeyD, PXKeyPressStateDown);

    PXKeyBoardVirtualInputSet(&inputList[2], KeyD, PXKeyPressStateUp);
    PXKeyBoardVirtualInputSet(&inputList[3], KeyWorld1, PXKeyPressStateUp);
    
    PXKeyBoardVirtualInsertAction(&inputList, 4);
#endif // 0










#if 0
    PXTextMatchTest();

    //PXActionResult res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*", OnFileElementDetected, 0x02, 0);
    //res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*png", OnFileElementDetected, 0x01, 0);


    printf("");



    char buffer[64];

    for (size_t i = 0; i <= 0xFF; i++)
    {
        TextFromIntToBinary64U(buffer, 64, i);

        printf("[%4i] %s\n", i, buffer);
    }
#endif
    

#if 0 // TEST FILE DIALOG

    PXText pxText;
    PXTextMakeFixedA(&pxText, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code");

    PXDialogFileOpen(&pxText);
    PXDialogFileSave(&pxText);

    PXColorRGBI8 pxColorRGBI8;

    PXDialogColorSelect(&pxColorRGBI8);
    PXDialogFontSelect();
    PXDialogPrint();

#endif // 0 // TEST FILE DIALOG


#define KeyBoardTest 0
#if KeyBoardTest

    PXWindow pxWindow;
    PXText pxText;
    PXTextMakeFixedA(&pxText, "Window input Test");

    PXWindowConstruct(&pxWindow);
    PXWindowCreate(&pxWindow, 600, 400, &pxText, PXTrue);

    while (1)
    {
        printf("\033[H\033[J");
        printf("\033[%d;%dH", 0, 0);
        PXKeyBoardInputPrint(&pxWindow.KeyBoardCurrentInput);
        PXThreadSleep(0, 100);
    }

#endif // 0



#if 0
    while (1)
    {
        const PXInt32U temperature = PXProcessorTemperature();

        printf("CPU Temp : %i\n", temperature);

        PXThreadSleep(0, 100);
    }
#endif // 1



#if 0
    PXImage image;

    //ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.bmp");
    ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.png");

    printf("EE\n");
    //TestImageAll();
#endif // 0


#if 0
    PXTestDebugAll();
#endif // 1


#if 0
    TestSoundAll();
#endif

#if 0
    TestFontAll();
#endif // 

#if 0 // PXOpenGL Test
    TestPXOpenGLAll();
#endif


#if 0
    TestSocket();
#endif // 1


#if 0
    TestSBPAll();
#endif // 1


#if 0
    TestFTPAll();
#endif // 0



    //Image image;

    //ImageLoadA(&image, "D:/_Data/Git/PXUltima/PXUltimaTest/_TEST_DATA_INPUT_/ImageInput.bmp");

    //ImageLoadA(&image, "A:/_WorkSpace/Download/at3_1m4_03.tif");

    //TestYAMLAll();
    //TestSystemInfoAll();
    //TestPXCompilerAll();
    //
    //TestWindowAll();
    //TestImageAll();
    // 
    // 
    // 
    //TestSBPAll();


#endif


    PXConsoleWrite(0, "[i] Finished testing...\n");

    return 0;
}
