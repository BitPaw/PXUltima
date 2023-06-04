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
        [DllImport("PXUltima.dll")] private static extern DayOfWeek PXTimeMonthFromID(byte monthID);
        [DllImport("PXUltima.dll")] private static extern Month PXTimeDayFromID(byte dayID);

        [DllImport("PXUltima.dll")] private static extern void PXTimeNow(ref PXTime time);
        [DllImport("PXUltima.dll")] private static extern UIntPtr PXTimeMillisecondsDelta(ref PXTime timeA, ref PXTime timeB);

        [DllImport("PXUltima.dll")] private static extern UIntPtr PXTimeCounterStampGet();
        [DllImport("PXUltima.dll")] private static extern UIntPtr PXTimeCounterFrequencyGet();

        public ushort Year;
        public ushort Milliseconds;
        public byte Second;
        public byte Minute;
        public byte Hour;
        public byte Day;
        public byte DayOfWeekID;
        public byte MonthID;

        public DayOfWeek DayOfWeek 
        {
            get
            {
                return PXTimeMonthFromID(DayOfWeekID);
            }
        }

        public Month Month
        {
            get
            {
                return PXTimeDayFromID(MonthID);
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
