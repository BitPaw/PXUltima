using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace PX
{
    public class Processor
    {
        [StructLayout(LayoutKind.Sequential, Size = 160)]
        private unsafe struct PXProcessor
        {
            public fixed sbyte BrandName[64];
            public fixed sbyte IdentityString[13];

            public uint BrandID;
            public uint ModelNameID;

            public byte NumberOfProcessors;
            public byte BrandNameSize;
        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXProcessorFetchInfo(ref PXProcessor processor);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern uint PXProcessorFrequencyCurrent();
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern uint PXProcessorTimeReal();

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern uint PXProcessorTemperature();


        public static uint FrequencyCurrent { get { return PXProcessorFrequencyCurrent(); } }
        public static uint TimeReal { get { return PXProcessorFrequencyCurrent(); } }
        public static uint Temperature { get { return PXProcessorFrequencyCurrent(); } }

        private PXProcessor _pxProcessor = new PXProcessor();

        public unsafe string BrandName
        {
            get
            {
                fixed(sbyte* byteAdress = &_pxProcessor.BrandName[0])
                {
                    return new string(byteAdress, 0, _pxProcessor.BrandNameSize);
                }            
            }
        }


        public unsafe string IdentityString
        {
            get
            {
                fixed (sbyte* byteAdress = &_pxProcessor.IdentityString[0])
                {
                    return new string(byteAdress, 0, 12);
                }
            }
        }


        public void Fetch()
        {
            PXProcessorFetchInfo(ref _pxProcessor);
        }

        public override string ToString()
        {
            return BrandName;
        }
    }
}
