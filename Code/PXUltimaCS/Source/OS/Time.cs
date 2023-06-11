using System;
using System.Runtime.InteropServices;

namespace PX
{
    public enum DayOfWeek
    {
        Invalid,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    }

    public enum Month
    {
        Invalid,
        January,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        October,
        November,
        December
    }

    // 10 Byte
    internal struct PXTime
    {
        public ushort Year;
        public ushort Milliseconds; // 0-1000
        public byte Second;  // 0-60
        public byte Minute; // 0-60
        public byte Hour; // 0-24
        public byte Day; // 0-31
        public byte DayOfWeek; // enum
        public byte Month; // 0-12   
    }

    public class Time
    {
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern DayOfWeek PXTimeMonthFromID(byte monthID);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern Month PXTimeDayFromID(byte dayID);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXTimeNow(ref PXTime time);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern UIntPtr PXTimeMillisecondsDelta(ref PXTime timeA, ref PXTime timeB);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern UIntPtr PXTimeCounterStampGet();
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern UIntPtr PXTimeCounterFrequencyGet();

        public int Year;
        public int Milliseconds;
        public int Second;
        public int Minute;
        public int Hour;
        public int Day;
        public int DayOfWeekID;
        public int MonthID;

        public DayOfWeek DayOfWeek 
        {
            get
            {
                if (DayOfWeekID > 0xFF) 
                {
                    return DayOfWeek.Invalid;
                }

                return PXTimeMonthFromID((byte)DayOfWeekID);
            }
        }

        public Month Month
        {
            get
            {
                if (MonthID > 0xFF)
                {
                    return Month.Invalid;
                }

                return PXTimeDayFromID((byte)MonthID);
            }
        }

        public Time()
        {
            Year = 0;
            Milliseconds = 0;
            Second = 0;
            Minute = 0;
            Hour = 0;
            Day = 0;
            DayOfWeekID = 0;
            MonthID = 0;
        }
        internal Time(ref PXTime pXTime)
        {

        }

        internal void Set(ref PXTime pXTime)
        {
            Year = pXTime.Year;
            Milliseconds = pXTime.Milliseconds;
            Second = pXTime.Second;
            Minute = pXTime.Minute;
            Hour = pXTime.Hour;
            Day = pXTime.Day;
            DayOfWeekID = pXTime.DayOfWeek;
            MonthID = pXTime.Month;
        }

        public void Now()
        {
            PXTime pxText = new PXTime();

            PXTimeNow(ref pxText);

            Set(ref pxText);
        }

        public ulong MillisecondsDelta(Time time)
        {
            return 0;
        }

        public static ulong CounterStampGet()
        {
            return (ulong)PXTimeCounterStampGet();
        }
        public static ulong CounterFrequency()
        {
            return (ulong)PXTimeCounterFrequencyGet();      
        }
    }
}
