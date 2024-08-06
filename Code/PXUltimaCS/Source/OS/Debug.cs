using System;
using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Sequential, Size = 410)]
    internal struct PXDebug
    {

    }

    public class Debug : IDisposable
    {
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugConstruct(ref PXDebug pxDebug);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugDestruct(ref PXDebug pxDebug);

        // VS Debugger functions-- maybe
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXDebugProcessBeeingDebugged(ref PXDebug pxDebug, ref byte isPresent);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern byte PXDebugProcessBeeingDebuggedCurrent();

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugDebuggerSendMessage(ref PXDebug pxDebug, ref PXText message);


        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern byte PXDebugDebuggerInitialize(ref PXDebug pxDebug);


        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXDebugStartProcess(ref PXDebug pxDebug, ref PXText applicationName);

        // Enables a debugger to continue a thread that previously reported a debugging event.
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugContinue(ref PXDebug pxDebug);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugPause(ref PXDebug pxDebug);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern byte PXDebugPauseOther(ref PXDebug pxDebug, UIntPtr pxProcessHandle);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXDebugAttach(ref PXDebug pxDebug);
        //PXPublic void PXDebugAttachToDebugger(ref PXDebug pxDebug);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXDebugDetach(ref PXDebug pxDebug);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXDebugStackTrace(ref PXDebug pxDebug);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern UIntPtr PXDebugMemoryRead(ref PXDebug pxDebug, UIntPtr adress, UIntPtr outputData, UIntPtr outputDataSize);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern UIntPtr PXDebugMemoryWrite(ref PXDebug pxDebug, UIntPtr adress, UIntPtr inputData, UIntPtr inputDataSize);

        // Only the thread that created the process being debugged can call this function.
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXDebugWaitForEvent(ref PXDebug pxDebug);

        private PXDebug _pxDebug = new PXDebug();

        public Debug()
        {
            PXDebugConstruct(ref _pxDebug);
        }

        public void Dispose()
        {
            PXDebugDestruct(ref _pxDebug);
        }

        public PX.ActionResult ProcessBeeingDebugged(ref bool isPresent)
        {
            byte isPresentData = 0;
            PX.ActionResult actionResult = PXDebugProcessBeeingDebugged(ref _pxDebug, ref isPresentData);

            isPresent = isPresentData != 0;

            return actionResult;
        }

        public bool ProcessBeeingDebuggedCurrent()
        {
            return PXDebugProcessBeeingDebuggedCurrent() != 0;
        }

        public unsafe void SendMessage(string message)
        {
            fixed(char* messageAdress = message.ToCharArray())
            {
                PXText pxText = PXText.MakeFromStringW(messageAdress, message.Length);

                PXDebugDebuggerSendMessage(ref _pxDebug, ref pxText);
            }        
        }

        public bool Initialize()
        {
            return PXDebugDebuggerInitialize(ref _pxDebug) != 0;
        }

        public unsafe PX.ActionResult StartProcess(string applicationName)
        {
            fixed (char* messageAdress = applicationName.ToCharArray())
            {
                PXText pxText = PXText.MakeFromStringW(messageAdress, applicationName.Length);

                return PXDebugStartProcess(ref _pxDebug, ref pxText);
            }        
        }

        public void Continue()
        {
            PXDebugContinue(ref _pxDebug);
        }

        public void Pause()
        {
            PXDebugPause(ref _pxDebug);
        }

        public bool PauseOther(UIntPtr pxProcessHandle)
        {
            return PXDebugPauseOther(ref _pxDebug, pxProcessHandle) != 0;
        }

        public PX.ActionResult Attach()
        {
            return PXDebugAttach(ref _pxDebug);
        }

        public PX.ActionResult Detach()
        {
            return PXDebugDetach(ref _pxDebug);
        }

        public void StackTrace()
        {
            PXDebugStackTrace(ref _pxDebug);
        }

        public UIntPtr MemoryRead(UIntPtr adress, UIntPtr outputData, UIntPtr outputDataSize)
        {
            return PXDebugMemoryRead(ref _pxDebug, adress, outputData, outputDataSize);
        }

        public UIntPtr MemoryWrite(UIntPtr adress, UIntPtr inputData, UIntPtr inputDataSize)
        {
            return PXDebugMemoryWrite(ref _pxDebug, adress, inputData, inputDataSize);
        }

        public PX.ActionResult WaitForEvent()
        {
            return PXDebugWaitForEvent(ref _pxDebug);
        }    
    }
}