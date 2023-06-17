using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class UnmanagedStructure : IDisposable
    {
        protected GCHandle _garbageCollectorHandle;

        public unsafe void* Adress        
        {
            get
            {
                if (_garbageCollectorHandle == null)
                {
                    return (void*)IntPtr.Zero;
                }

#if false
                return (void*)_garbageCollectorHandle.AddrOfPinnedObject();
#else
                return (void*)GCHandle.ToIntPtr(_garbageCollectorHandle);
#endif

            }
        }

        public UnmanagedStructure()
        {
            
        }

        public void OwnerSet<T>(T ownerObject)
        {
 
            _garbageCollectorHandle = GCHandle.Alloc(ownerObject);
        }

        public static unsafe T ObjectFromAdress<T>(void* adress) where T : class
        {
            if (adress == null)
            {
                return null;
            }

#if true
            IntPtr intAddress = new IntPtr(adress);
            GCHandle objectAddress = GCHandle.FromIntPtr(intAddress);
#else
            GCHandle objectAddress = (GCHandle)(IntPtr)adress;
#endif
            T objectReference = objectAddress.Target as T;
            return objectReference;
        }

        public void Dispose()
        {
            _garbageCollectorHandle.Free();
        }

        public static unsafe void* DelegateToAdress<TDelegate>(TDelegate function)
        {
            return Marshal.GetFunctionPointerForDelegate(function).ToPointer();
        }    
    }
}
