using System;
using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Sequential, Size = 8)]
	internal struct PXRegistry
	{

    }

	public enum RegistrySpace
	{
		Invalid,
		LocalMachine,
		PerformanceData,
		Users
	}

    public class Registry : IDisposable
    {
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryConnectSpace(ref PXRegistry registry, RegistrySpace registrySpace);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryConnectRemote(ref PXRegistry registry, ref PXText computerName, RegistrySpace registrySpace);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryClose(ref PXRegistry registry);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryKeyCreate(ref PXRegistry registry);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryKeyLoad(ref PXRegistry registry, ref PXText keyName);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryKeySave(ref PXRegistry registry, ref PXText keyName);
		[DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXRegistryKeyDelete(ref PXRegistry registry, ref PXText keyName);

		private PXRegistry _pxRegistry = new PXRegistry();

		public Registry()
		{ 
			
		}
		public void Dispose()
		{
		
		}

		public PX.ActionResult ConnectSpace(RegistrySpace registrySpace)
		{
			return PXRegistryConnectSpace(ref _pxRegistry, registrySpace);
		}

		public unsafe PX.ActionResult ConnectRemote(string computerName, RegistrySpace registrySpace)
		{
			fixed(char* computerNameAdress = computerName.ToCharArray())
            {
				PXText pxText = PXText.MakeFromStringW(computerNameAdress, computerName.Length);

				return PXRegistryConnectRemote(ref _pxRegistry, ref pxText, registrySpace);
			}	
		}

		public void Close()
        {
			PXRegistryClose(ref _pxRegistry);
        }
		public void KeyCreate()
		{
			PXRegistryKeyCreate(ref _pxRegistry);
		}
		public void KeyLoad(string key)
		{
			PXRegistryKeyLoad(ref _pxRegistry);
		}
		public void KeySave(string key, )
		{
			PXRegistryKeySave(ref _pxRegistry);
		}
		public void KeyDelete()
		{
			PXRegistryKeyDelete(ref _pxRegistry);
		}
	}
}
