using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class DataBase : IDisposable
    {
        [StructLayout(LayoutKind.Explicit, Size = 200)]
        private struct PXSQLDataBaseConnection
        {
            
        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionConstruct(ref PXSQLDataBaseConnection dataBaseConnection);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionDestruct(ref PXSQLDataBaseConnection dataBaseConnection);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXDataBaseConnectionConnect(ref PXSQLDataBaseConnection dataBaseConnection, ref PXText source, ref PXText database, ref PXText user, ref PXText password);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionDisconnect(ref PXSQLDataBaseConnection dataBaseConnection);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionCleanup(ref PXSQLDataBaseConnection dataBaseConnection);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionScanForDrivers(ref PXSQLDataBaseConnection dataBaseConnection);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXDataBaseConnectionExecute(ref PXSQLDataBaseConnection dataBaseConnection, ref PXText PXSQLStatement);


        private PXSQLDataBaseConnection _pxSQLDataBaseConnection = new PXSQLDataBaseConnection();

        public DataBase()
        {
            PXDataBaseConnectionConstruct(ref _pxSQLDataBaseConnection);
        }

        public void Dispose()
        {
            PXDataBaseConnectionDestruct(ref _pxSQLDataBaseConnection);
        }

        public unsafe PX.ActionResult Connect(string source, string database, string user, string password)
        {
            fixed(char* sourceAdress = source.ToCharArray())
            {
                PX.PXText sourceText = PX.PXText.MakeFromStringW(sourceAdress, source.Length);

                fixed (char* databaseAdress = database.ToCharArray())
                {
                    PX.PXText databaseText = PX.PXText.MakeFromStringW(sourceAdress, database.Length);

                    fixed (char* userAdress = user.ToCharArray())
                    {
                        PX.PXText userText = PX.PXText.MakeFromStringW(sourceAdress, user.Length);

                        fixed (char* passwordAdress = password.ToCharArray())
                        {
                            PX.PXText passwordText = PX.PXText.MakeFromStringW(sourceAdress, password.Length);

                            return PXDataBaseConnectionConnect(ref _pxSQLDataBaseConnection, ref sourceText, ref databaseText, ref userText, ref passwordText);
                        }
                    }
                }
            }
        }

        public void Disconnect() 
        {
            PXDataBaseConnectionDisconnect(ref _pxSQLDataBaseConnection);
        }

        public unsafe void Execute(string sqlMessage)
        {
            fixed (char* sqlMessageAdress = sqlMessage.ToCharArray())
            {
                PX.PXText sqlMessageText = PX.PXText.MakeFromStringW(sqlMessageAdress, sqlMessage.Length);

                PXDataBaseConnectionExecute(ref _pxSQLDataBaseConnection, ref sqlMessageText);
            }
        }
    }
}